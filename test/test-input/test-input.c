#include <obs-module.h>

OBS_DECLARE_MODULE(win_capture);

extern struct obs_source_info test_random;
extern struct obs_source_info test_sinewave;
extern struct obs_source_info test_filter;
extern struct obs_source_info async_sync_test;
extern struct obs_source_info buffering_async_sync_test;
extern struct obs_source_info sync_video;
extern struct obs_source_info sync_audio;

static bool obs_module_load(void)
{
	obs_register_source(&test_random);
	obs_register_source(&test_sinewave);
	obs_register_source(&test_filter);
	obs_register_source(&async_sync_test);
	obs_register_source(&buffering_async_sync_test);
	obs_register_source(&sync_video);
	obs_register_source(&sync_audio);
	return true;
}

static void obs_module_unload(void) {

}

static void obs_module_post_load(void) {

}

static void obs_module_set_locale(const char *locale) {

}

static const char* obs_module_text_impl(const char* val)
{
	return NULL;
}

static bool obs_module_get_string_impl(const char *lookup_string, const char **translated_string)
{
	return false;
}

static void obs_module_free_locale(void) {

}

static const char *obs_module_name(void)
{
	return "test_input";
}

static const char *obs_module_description(void)
{
	return "";
}

static const char *obs_module_author(void)
{
	return "";
}

OBS_DEFINE_MODULE(test_input);
