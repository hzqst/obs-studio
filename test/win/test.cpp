#include <stdio.h>
#include <time.h>
#include <windows.h>

#include <util/base.h>
#include <graphics/vec2.h>
#include <media-io/audio-resampler.h>
#include <obs.h>
#include <obs-module.h>

#include <intrin.h>

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

	case WM_CLOSE:
		PostQuitMessage(0);
		break;

	default:
		return DefWindowProc(hwnd, message, wParam, lParam);
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

	struct obs_video_info ovi;
	ovi.adapter = 0;
	ovi.base_width = rc.right;
	ovi.base_height = rc.bottom;
	ovi.fps_num = 30000;
	ovi.fps_den = 1001;
	ovi.graphics_subsystem = "d3d11";
	ovi.output_format = VIDEO_FORMAT_RGBA;
	ovi.output_width = rc.right;
	ovi.output_height = rc.bottom;

	if (obs_reset_video(&ovi) != 0)
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

	vec2_set(&scale, 20.0f, 20.0f);

	item = obs_scene_add(scene, source);
	obs_sceneitem_set_scale(item, &scale);
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

		/* ------------------------------------------------------ */
		/* load modules */
		//obs_load_all_modules();

		/* load static modules */
		obs_current_module_test_input()->load();
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

		/* ------------------------------------------------------ */
		/* create filter */
		//SourceContext filter = obs_source_create("test_filter", "a nice green filter", NULL, nullptr);
		//if (!filter)
		//	throw "Couldn't create test filter";
		//obs_source_filter_add(source, filter);

		/* ------------------------------------------------------ */
		/* create scene and add source to scene (twice) */
		auto scene = obs_scene_create("test scene");
		if (!scene)
			throw "Couldn't create scene";

		AddTestItems(scene, source);

		//obs_source_release(source);

		/* ------------------------------------------------------ */
		/* set the scene as the primary draw source and go */
		obs_set_output_source(0, obs_scene_get_source(scene));

		obs_scene_release(scene);

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
		
	} catch (char *error) {
		MessageBoxA(NULL, error, NULL, 0);
	}
	
	obs_shutdown();

	blog(LOG_INFO, "Number of memory leaks: %ld", bnum_allocs());

	DestroyWindow(hwnd);

	return 0;
}
