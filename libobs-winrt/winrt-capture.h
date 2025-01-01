#pragma once

#define WIN32_LEAN_AND_MEAN
#include <windows.h>

#include <obs-module.h>

#ifdef __cplusplus
extern "C" {
#endif

BOOL winrt_capture_supported();
BOOL winrt_capture_cursor_toggle_supported();
struct winrt_capture *winrt_capture_init_window(BOOL cursor, HWND window, BOOL client_area, BOOL force_sdr);
struct winrt_capture *winrt_capture_init_monitor(BOOL cursor, HMONITOR monitor, BOOL force_sdr);
void winrt_capture_free(struct winrt_capture *capture);

BOOL winrt_capture_active(const struct winrt_capture *capture);
BOOL winrt_capture_show_cursor(struct winrt_capture *capture, BOOL visible);
enum gs_color_space winrt_capture_get_color_space(const struct winrt_capture *capture);
void winrt_capture_render(struct winrt_capture *capture);
uint32_t winrt_capture_width(const struct winrt_capture *capture);
uint32_t winrt_capture_height(const struct winrt_capture *capture);

void winrt_capture_thread_start();
void winrt_capture_thread_stop();

#ifdef __cplusplus
}
#endif
