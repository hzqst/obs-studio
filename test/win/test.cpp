#include <stdio.h>
#include <time.h>
#include <windows.h>

#include <util/base.h>
#include <graphics/vec2.h>
#include <media-io/audio-resampler.h>
#include <obs.h>
#include <obs-module.h>

#include <intrin.h>
#include <vector>

static const int cx = 800;
static const int cy = 600;

/* --------------------------------------------------- */

class SourceContext {
	obs_source_t *source;

public:
	inline SourceContext(obs_source_t *source) : source(source) {}
	inline ~SourceContext() { obs_source_release(source); }
	inline operator obs_source_t *() { return source; }
};

/* --------------------------------------------------- */

class SceneContext {
	obs_scene_t *scene;

public:
	inline SceneContext(obs_scene_t *scene) : scene(scene) {}
	inline ~SceneContext() { obs_scene_release(scene); }
	inline operator obs_scene_t *() { return scene; }
};

/* --------------------------------------------------- */

class DisplayContext {
	obs_display_t *display;

public:
	inline DisplayContext(obs_display_t *display) : display(display) {}
	inline ~DisplayContext() { obs_display_destroy(display); }
	inline operator obs_display_t *() { return display; }
};

/* --------------------------------------------------- */

static LRESULT CALLBACK sceneProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message) {

	case WM_CLOSE: {
		PostQuitMessage(0);
		break;
	}
	default: {
		return DefWindowProc(hwnd, message, wParam, lParam);
	}
	}

	return 0;
}

static void do_log(int log_level, const char *msg, va_list args, void *param)
{
	char bla[4096];
	vsnprintf(bla, 4095, msg, args);

	OutputDebugStringA(bla);
	OutputDebugStringA("\n");

	if (log_level < LOG_WARNING)
		__debugbreak();

	UNUSED_PARAMETER(param);
}

static void CreateOBS(HWND hwnd)
{
	RECT rc;
	GetClientRect(hwnd, &rc);

	if (!obs_startup("en-US", nullptr, nullptr))
		throw "Couldn't create OBS";
}

static void ResetVideo(uint32_t width, uint32_t height) {
	struct obs_video_info ovi;
	ovi.adapter = 0;
	ovi.base_width = width;
	ovi.base_height = height;
	ovi.fps_num = 30000;
	ovi.fps_den = 1001;
	ovi.graphics_subsystem = "d3d11";
	ovi.output_format = VIDEO_FORMAT_RGBA;
	ovi.output_width = width;
	ovi.output_height = height;
	ovi.colorspace = VIDEO_CS_SRGB;
	ovi.range = VIDEO_RANGE_FULL;
	ovi.gpu_conversion = true;
	ovi.scale_type = OBS_SCALE_BILINEAR;

	if (obs_reset_video(&ovi) != OBS_VIDEO_SUCCESS)
		throw "Couldn't initialize video";
}


obs_display_t * CreateDisplayContext(HWND hwnd)
{
	RECT rc;
	GetClientRect(hwnd, &rc);

	gs_init_data info = {};
	info.cx = rc.right;
	info.cy = rc.bottom;
	info.format = GS_RGBA;
	info.zsformat = GS_ZS_NONE;
	info.window.hwnd = hwnd;

	return obs_display_create(&info, 0);
}

static void AddTestItems(obs_scene_t *scene, obs_source_t *source)
{
	obs_sceneitem_t *item = NULL;
	struct vec2 scale;

	vec2_set(&scale, 1.0f, 1.0f);

	item = obs_scene_add(scene, source);
	obs_sceneitem_set_scale(item, &scale);
	//obs_sceneitem_set_alignment(item, OBS_ALIGN_CENTER);
}

static HWND CreateTestWindow(HINSTANCE instance)
{
	WNDCLASS wc;

	memset(&wc, 0, sizeof(wc));
	wc.lpszClassName = TEXT("test");
	wc.hbrBackground = (HBRUSH)COLOR_WINDOW;
	wc.hInstance = instance;
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.lpfnWndProc = (WNDPROC)sceneProc;

	if (!RegisterClass(&wc))
		return 0;

	return CreateWindow(TEXT("test"), TEXT("test"), WS_OVERLAPPEDWINDOW | WS_VISIBLE, 1920 / 2 - cx / 2,
			    1080 / 2 - cy / 2, cx, cy, NULL, NULL, instance, NULL);
}

/* --------------------------------------------------- */

static void RenderWindow(void *data, uint32_t cx, uint32_t cy)
{
	obs_render_main_texture();

	UNUSED_PARAMETER(data);
	UNUSED_PARAMETER(cx);
	UNUSED_PARAMETER(cy);
}

#define SCREENSHOT_STAGE_START 0
#define SCREENSHOT_STAGE_DOWNLOAD 1
#define SCREENSHOT_STAGE_COPY_AND_SAVE 2
#define SCREENSHOT_STAGE_FINISH 3

static void ScreenshotTick(void *param, float time);

class CScreenshotRequest {
public:
	CScreenshotRequest(obs_source_t *s) {

		m_source = obs_source_get_ref(s);

		obs_add_tick_callback(ScreenshotTick, this);
	}

	~CScreenshotRequest() {

		obs_enter_graphics();
		gs_stagesurface_destroy(m_stagesurf);
		gs_texrender_destroy(m_texrender);
		obs_leave_graphics();

		obs_remove_tick_callback(ScreenshotTick, this);

		if (m_source) {
			obs_source_release(m_source);
		}
	}

	int Start()
	{
		m_width = obs_source_get_width(m_source);
		m_height = obs_source_get_height(m_source);

		if (!m_width || !m_height)
		{
			//throw "Invalid source width or height";
			return SCREENSHOT_STAGE_START;
		}

#ifdef _WIN32
		enum gs_color_space space = obs_source_get_color_space(m_source, 0, nullptr);
		if (space == GS_CS_709_EXTENDED) {
			/* Convert for JXR */
			space = GS_CS_709_SCRGB;
		}
#else
		/* Tonemap to SDR if HDR */
		const enum gs_color_space space = GS_CS_SRGB;
#endif
		const enum gs_color_format format = gs_get_format_from_space(space);

		m_texrender = gs_texrender_create(format, GS_ZS_NONE);
		m_stagesurf = gs_stagesurface_create(m_width, m_height, format);

		if (gs_texrender_begin_with_color_space(m_texrender, m_width, m_height, space)) {
			vec4 zero;
			vec4_zero(&zero);

			gs_clear(GS_CLEAR_COLOR, &zero, 0.0f, 0);
			gs_ortho(0.0f, (float)m_width, 0.0f, (float)m_height, -100.0f, 100.0f);

			gs_blend_state_push();
			gs_blend_function(GS_BLEND_ONE, GS_BLEND_ZERO);

			if (m_source) {
				obs_source_inc_showing(m_source);
				obs_source_video_render(m_source);
				obs_source_dec_showing(m_source);
			} else {
				obs_render_main_texture();
			}

			gs_blend_state_pop();
			gs_texrender_end(m_texrender);
		}

		return SCREENSHOT_STAGE_DOWNLOAD;
	}

	int Download()
	{
		gs_stage_texture(m_stagesurf, gs_texrender_get_texture(m_texrender));

		return SCREENSHOT_STAGE_COPY_AND_SAVE;
	}

	int CopyAndSave()
	{
		uint8_t *videoData = nullptr;
		uint32_t videoLinesize = 0;

		if (gs_stagesurface_map(m_stagesurf, &videoData, &videoLinesize)) {
			if (gs_stagesurface_get_color_format(m_stagesurf) == GS_RGBA16F) {
				const uint32_t linesize = m_width * 8;
				half_bytes.reserve(m_width * m_height * 8);

				for (uint32_t y = 0; y < m_height; y++) {
					const uint8_t *const line = videoData + (y * videoLinesize);
					half_bytes.insert(half_bytes.end(), line, line + linesize);
				}
			} else {
				//TODO
			}

			gs_stagesurface_unmap(m_stagesurf);
		}

		return SCREENSHOT_STAGE_FINISH;
	}

	void RunTick()
	{
		if (m_stage == SCREENSHOT_STAGE_FINISH) {
			//TODO delete me
			return;
		}

		if (m_tick > 1) {
			obs_enter_graphics();

			switch (m_stage) {
			case SCREENSHOT_STAGE_START: {
				m_stage = Start();
				break;
			}
			case SCREENSHOT_STAGE_DOWNLOAD: {
				m_stage = Download();
				break;
			}
			case SCREENSHOT_STAGE_COPY_AND_SAVE: {
				m_stage = CopyAndSave();
				break;
			}
			}

			obs_leave_graphics();
		}

		m_tick++;
	}

	gs_texrender_t *m_texrender = nullptr;
	gs_stagesurf_t *m_stagesurf = nullptr;
	obs_source_t *m_source = nullptr;
	uint32_t m_width = 0;
	uint32_t m_height = 0;
	std::vector<byte> half_bytes;
	int m_stage = 0;
	int m_tick = 0;
};


static void ScreenshotTick(void *param, float time)
{
	auto pScreenRequest = (CScreenshotRequest *)param;

	pScreenRequest->RunTick();
}

/* --------------------------------------------------- */

extern "C" {
OBS_DECLARE_MODULE(test_input);
OBS_DECLARE_MODULE(win_capture);
}

int WINAPI WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPSTR lpCmdLine, _In_ int nShowCmd)
{
	HWND hwnd = NULL;
	base_set_log_handler(do_log, nullptr);

	try {
		hwnd = CreateTestWindow(hInstance);

		if (!hwnd)
			throw "Couldn't create main window";

		/* ------------------------------------------------------ */
		/* Create OBS instance */
		CreateOBS(hwnd);

		ResetVideo(3440, 1440);

		/* ------------------------------------------------------ */
		/* load dynamic modules */
		//obs_load_all_modules();

		/* load static modules */
		//obs_current_module_test_input()->load();

		obs_current_module_win_capture()->load();

		/* ------------------------------------------------------ */
		/* create source */
		auto props = obs_get_source_properties("monitor_capture");

		if (!props)
			throw "Couldn't get monitor_capture properties";

		auto monitor_ids = obs_properties_get(props, "monitor_id");

		if (!monitor_ids)
			throw "Couldn't get monitor_ids";

		const char *main_monitor_id = nullptr;

		for (int i = 0; i < obs_property_list_item_count(monitor_ids); ++i)
		{
			auto monitor_id = obs_property_list_item_string(monitor_ids, i);
			if (0 != strcmp(monitor_id, "DUMMY")) {
				main_monitor_id = monitor_id;
			}
		}

		if (!main_monitor_id)
			throw "Couldn't get main_monitor_id";

		auto settings = obs_data_create();

		obs_data_set_int(settings, "method", 1);
		obs_data_set_string(settings, "monitor_id", main_monitor_id);

		auto source = obs_source_create("monitor_capture", "monitor_capture source", settings, nullptr);

		if (!source)
			throw "Couldn't create source";

		obs_data_release(settings);

		obs_properties_destroy(props);

		auto scene = obs_scene_create("main");

		if (!scene)
			throw "Couldn't create main scene";

		obs_sceneitem_t *item = NULL;
		struct vec2 scale;

		vec2_set(&scale, 1.0f, 1.0f);

		//item = obs_scene_add(scene, source);

		//obs_sceneitem_set_scale(item, &scale);

		//obs_sceneitem_set_visible(item, false);

		/* ------------------------------------------------------ */
		/* set the scene as the primary draw source and go */
		obs_set_output_source(0, obs_scene_get_source(scene));

		//obs_scene_release(scene);

		//auto pScreenShotRequest = new CScreenshotRequest(source);

		/* ------------------------------------------------------ */
		/* create display for output and set the output render callback */

		auto display = CreateDisplayContext(hwnd);
		obs_display_add_draw_callback(display, RenderWindow, nullptr);
		
		MSG msg;
		while (GetMessage(&msg, NULL, 0, 0)) {
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}

		obs_display_remove_draw_callback(display, RenderWindow, nullptr);
		
		obs_display_destroy(display);

		obs_source_release(source);

		obs_scene_release(scene);
		//delete pScreenShotRequest;

	} catch (char *error) {
		MessageBoxA(NULL, error, NULL, 0);
	}
	
	obs_shutdown();

	blog(LOG_INFO, "Number of memory leaks: %ld", bnum_allocs());

	DestroyWindow(hwnd);

	return 0;
}
