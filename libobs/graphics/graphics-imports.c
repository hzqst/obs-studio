/******************************************************************************
    Copyright (C) 2023 by Lain Bailey <lain@obsproject.com>

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 2 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
******************************************************************************/

#include "../util/base.h"
#include "../util/dstr.h"
#include "../util/platform.h"
#include "graphics-internal.h"

#define GRAPHICS_IMPORT_D3D11(func) exports->func = func##_d3d11;

const char *device_get_name_d3d11(void);
int device_get_type_d3d11(void);
const char *device_preprocessor_name_d3d11(void);
bool device_enum_adapters_d3d11(gs_device_t *device, bool (*callback)(void *param, const char *name, uint32_t id),
				void *param);

int device_create_d3d11(gs_device_t **p_device, uint32_t adapter);
void device_destroy_d3d11(gs_device_t *device);
void device_enter_context_d3d11(gs_device_t *device);
void device_leave_context_d3d11(gs_device_t *device);
void *device_get_device_obj_d3d11(gs_device_t *device);
gs_swapchain_t *device_swapchain_create_d3d11(gs_device_t *device, const struct gs_init_data *data);
void device_resize_d3d11(gs_device_t *device, uint32_t cx, uint32_t cy);
enum gs_color_space device_get_color_space_d3d11(gs_device_t *device);
void device_update_color_space_d3d11(gs_device_t *device);
void device_get_size_d3d11(const gs_device_t *device, uint32_t *cx, uint32_t *cy);
uint32_t device_get_width_d3d11(const gs_device_t *device);
uint32_t device_get_height_d3d11(const gs_device_t *device);
gs_texture_t *device_texture_create_d3d11(gs_device_t *device, uint32_t width, uint32_t height,
				    enum gs_color_format color_format, uint32_t levels, const uint8_t **data,
				    uint32_t flags);
gs_texture_t *device_cubetexture_create_d3d11(gs_device_t *device, uint32_t size, enum gs_color_format color_format,
					uint32_t levels, const uint8_t **data, uint32_t flags);
gs_texture_t *device_voltexture_create_d3d11(gs_device_t *device, uint32_t width, uint32_t height, uint32_t depth,
				       enum gs_color_format color_format, uint32_t levels, const uint8_t *const *data,
				       uint32_t flags);
gs_zstencil_t *device_zstencil_create_d3d11(gs_device_t *device, uint32_t width, uint32_t height,
					    enum gs_zstencil_format format);
gs_stagesurf_t *device_stagesurface_create_d3d11(gs_device_t *device, uint32_t width, uint32_t height,
						 enum gs_color_format color_format);
gs_samplerstate_t *device_samplerstate_create_d3d11(gs_device_t *device, const struct gs_sampler_info *info);
gs_shader_t *device_vertexshader_create_d3d11(gs_device_t *device, const char *shader_string, const char *file,
					      char **error_string);
gs_shader_t *device_pixelshader_create_d3d11(gs_device_t *device, const char *shader_string, const char *file,
				       char **error_string);
gs_vertbuffer_t *device_vertexbuffer_create_d3d11(gs_device_t *device, struct gs_vb_data *data, uint32_t flags);
gs_indexbuffer_t *device_indexbuffer_create_d3d11(gs_device_t *device, enum gs_index_type type, void *indices,
						  size_t num, uint32_t flags);
gs_timer_t *device_timer_create_d3d11(gs_device_t *device);
gs_timer_range_t *device_timer_range_create_d3d11(gs_device_t *device);
enum gs_texture_type device_get_texture_type_d3d11(const gs_texture_t *texture);
void device_load_vertexbuffer_d3d11(gs_device_t *device, gs_vertbuffer_t *vertbuffer);
void device_load_indexbuffer_d3d11(gs_device_t *device, gs_indexbuffer_t *indexbuffer);
void device_load_texture_d3d11(gs_device_t *device, gs_texture_t *tex, int unit);
void device_load_texture_srgb_d3d11(gs_device_t *device, gs_texture_t *tex, int unit);
void device_load_samplerstate_d3d11(gs_device_t *device, gs_samplerstate_t *samplerstate, int unit);
void device_load_vertexshader_d3d11(gs_device_t *device, gs_shader_t *vertshader);
void device_load_pixelshader_d3d11(gs_device_t *device, gs_shader_t *pixelshader);
void device_load_default_samplerstate_d3d11(gs_device_t *device, bool b_3d,
					    int unit);
gs_shader_t *device_get_vertex_shader_d3d11(const gs_device_t *device);
gs_shader_t *device_get_pixel_shader_d3d11(const gs_device_t *device);
gs_texture_t *device_get_render_target_d3d11(const gs_device_t *device);
gs_zstencil_t *device_get_zstencil_target_d3d11(const gs_device_t *device);
void device_set_render_target_d3d11(gs_device_t *device, gs_texture_t *tex, gs_zstencil_t *zstencil);
void device_set_render_target_with_color_space_d3d11(gs_device_t *device, gs_texture_t *tex, gs_zstencil_t *zstencil,
						     enum gs_color_space space);
void device_set_cube_render_target_d3d11(gs_device_t *device, gs_texture_t *tex, int side, gs_zstencil_t *zstencil);
void device_enable_framebuffer_srgb_d3d11(gs_device_t *device, bool enable);
bool device_framebuffer_srgb_enabled_d3d11(gs_device_t *device);
void device_copy_texture_region_d3d11(gs_device_t *device, gs_texture_t *dst, uint32_t dst_x, uint32_t dst_y,
				      gs_texture_t *src, uint32_t src_x, uint32_t src_y, uint32_t src_w,
				      uint32_t src_h);
void device_copy_texture_d3d11(gs_device_t *device, gs_texture_t *dst, gs_texture_t *src);
void device_stage_texture_d3d11(gs_device_t *device, gs_stagesurf_t *dst,
				gs_texture_t *src);
void device_begin_frame_d3d11(gs_device_t *device);
void device_begin_scene_d3d11(gs_device_t *device);
void device_draw_d3d11(gs_device_t *device, enum gs_draw_mode draw_mode, uint32_t start_vert, uint32_t num_verts);
void device_end_scene_d3d11(gs_device_t *device);
void device_load_swapchain_d3d11(gs_device_t *device, gs_swapchain_t *swapchain);
void device_clear_d3d11(gs_device_t *device, uint32_t clear_flags, const struct vec4 *color, float depth,
			uint8_t stencil);
bool device_is_present_ready_d3d11(gs_device_t *device);
void device_present_d3d11(gs_device_t *device);
void device_flush_d3d11(gs_device_t *device);
void device_set_cull_mode_d3d11(gs_device_t *device, enum gs_cull_mode mode);
enum gs_cull_mode device_get_cull_mode_d3d11(const gs_device_t *device);
void device_enable_blending_d3d11(gs_device_t *device, bool enable);
void device_enable_depth_test_d3d11(gs_device_t *device, bool enable);
void device_enable_stencil_test_d3d11(gs_device_t *device, bool enable);
void device_enable_stencil_write_d3d11(gs_device_t *device, bool enable);
void device_enable_color_d3d11(gs_device_t *device, bool red, bool green, bool blue, bool alpha);
void device_blend_function_d3d11(gs_device_t *device, enum gs_blend_type src, enum gs_blend_type dest);
void device_blend_function_separate_d3d11(gs_device_t *device, enum gs_blend_type src_c, enum gs_blend_type dest_c,
					  enum gs_blend_type src_a, enum gs_blend_type dest_a);
void device_blend_op_d3d11(gs_device_t *device,
			   enum gs_blend_op_type op);
void device_depth_function_d3d11(gs_device_t *device, enum gs_depth_test test);
void device_stencil_function_d3d11(gs_device_t *device, enum gs_stencil_side side,
				   enum gs_depth_test test);
void device_stencil_op_d3d11(gs_device_t *device, enum gs_stencil_side side, enum gs_stencil_op_type fail,
			     enum gs_stencil_op_type zfail, enum gs_stencil_op_type zpass);
void device_set_viewport_d3d11(gs_device_t *device, int x, int y, int width, int height);
void device_get_viewport_d3d11(const gs_device_t *device, struct gs_rect *rect);
void device_set_scissor_rect_d3d11(gs_device_t *device, const struct gs_rect *rect);
void device_ortho_d3d11(gs_device_t *device, float left, float right, float top, float bottom, float zNear, float zFar);
void device_frustum_d3d11(gs_device_t *device, float left, float right, float top, float bottom, float zNear,
			  float zFar);
void device_projection_push_d3d11(gs_device_t *device);
void device_projection_pop_d3d11(gs_device_t *device);
void gs_swapchain_destroy_d3d11(gs_swapchain_t *swapchain);
void gs_texture_destroy_d3d11(gs_texture_t *tex);
uint32_t gs_texture_get_width_d3d11(const gs_texture_t *tex);
uint32_t gs_texture_get_height_d3d11(const gs_texture_t *tex);
enum gs_color_format gs_texture_get_color_format_d3d11(const gs_texture_t *tex);
bool gs_texture_map_d3d11(gs_texture_t *tex, uint8_t **ptr, uint32_t *linesize);
void gs_texture_unmap_d3d11(gs_texture_t *tex);
bool gs_texture_is_rect_d3d11(const gs_texture_t *tex);
void *gs_texture_get_obj_d3d11(const gs_texture_t *tex);
void gs_cubetexture_destroy_d3d11(gs_texture_t *cubetex);
uint32_t gs_cubetexture_get_size_d3d11(const gs_texture_t *cubetex);
enum gs_color_format gs_cubetexture_get_color_format_d3d11(const gs_texture_t *cubetex);
void gs_voltexture_destroy_d3d11(gs_texture_t *voltex);
uint32_t gs_voltexture_get_width_d3d11(const gs_texture_t *voltex);
uint32_t gs_voltexture_get_height_d3d11(const gs_texture_t *voltex);
uint32_t gs_voltexture_get_depth_d3d11(const gs_texture_t *voltex);
enum gs_color_format gs_voltexture_get_color_format_d3d11(const gs_texture_t *voltex);
void gs_stagesurface_destroy_d3d11(gs_stagesurf_t *stagesurf);
uint32_t gs_stagesurface_get_width_d3d11(const gs_stagesurf_t *stagesurf);
uint32_t gs_stagesurface_get_height_d3d11(const gs_stagesurf_t *stagesurf);
enum gs_color_format gs_stagesurface_get_color_format_d3d11(const gs_stagesurf_t *stagesurf);
bool gs_stagesurface_map_d3d11(gs_stagesurf_t *stagesurf, uint8_t **data, uint32_t *linesize);
void gs_stagesurface_unmap_d3d11(gs_stagesurf_t *stagesurf);
void gs_zstencil_destroy_d3d11(gs_zstencil_t *zstencil);
void gs_samplerstate_destroy_d3d11(gs_samplerstate_t *samplerstate);
void gs_vertexbuffer_destroy_d3d11(gs_vertbuffer_t *vertbuffer);
void gs_vertexbuffer_flush_d3d11(gs_vertbuffer_t *vertbuffer);
void gs_vertexbuffer_flush_direct_d3d11(gs_vertbuffer_t *vertbuffer, const struct gs_vb_data *data);
struct gs_vb_data *gs_vertexbuffer_get_data_d3d11(const gs_vertbuffer_t *vertbuffer);
void gs_indexbuffer_destroy_d3d11(gs_indexbuffer_t *indexbuffer);
void gs_indexbuffer_flush_d3d11(gs_indexbuffer_t *indexbuffer);
void gs_indexbuffer_flush_direct_d3d11(gs_indexbuffer_t *indexbuffer, const void *data);
void *gs_indexbuffer_get_data_d3d11(const gs_indexbuffer_t *indexbuffer);
size_t gs_indexbuffer_get_num_indices_d3d11(const gs_indexbuffer_t *indexbuffer);
enum gs_index_type gs_indexbuffer_get_type_d3d11(const gs_indexbuffer_t *indexbuffer);
void gs_timer_destroy_d3d11(gs_timer_t *timer);
void gs_timer_begin_d3d11(gs_timer_t *timer);
void gs_timer_end_d3d11(gs_timer_t *timer);
bool gs_timer_get_data_d3d11(gs_timer_t *timer, uint64_t *ticks);
void gs_timer_range_destroy_d3d11(gs_timer_range_t *range);
void gs_timer_range_begin_d3d11(gs_timer_range_t *range);
void gs_timer_range_end_d3d11(gs_timer_range_t *range);
bool gs_timer_range_get_data_d3d11(gs_timer_range_t *range, bool *disjoint, uint64_t *frequency);
bool device_gdi_texture_available_d3d11(void);
bool device_shared_texture_available_d3d11(void);
bool device_nv12_available_d3d11(gs_device_t *device);
bool device_p010_available_d3d11(gs_device_t *device);
bool device_is_monitor_hdr_d3d11(gs_device_t *device, void *monitor);
void device_debug_marker_begin_d3d11(gs_device_t *, const char *markername, const float color[4]);
void device_debug_marker_end_d3d11(gs_device_t *);
gs_texture_t *device_texture_create_gdi_d3d11(gs_device_t *device, uint32_t width, uint32_t height);
void *gs_texture_get_dc_d3d11(gs_texture_t *tex);
void gs_texture_release_dc_d3d11(gs_texture_t *tex);
gs_texture_t *device_texture_open_shared_d3d11(gs_device_t *device, uint32_t handle);
gs_texture_t *device_texture_open_nt_shared_d3d11(gs_device_t *device, uint32_t handle);
uint32_t device_texture_get_shared_handle_d3d11(gs_texture_t *tex);
gs_texture_t *device_texture_wrap_obj_d3d11(gs_device_t *device, void *obj);
int device_texture_acquire_sync_d3d11(gs_texture_t *tex, uint64_t key, uint32_t ms);
int device_texture_release_sync_d3d11(gs_texture_t *tex, uint64_t key);
bool device_texture_create_nv12_d3d11(gs_device_t *device, gs_texture_t **p_tex_y, gs_texture_t **p_tex_uv,
			      uint32_t width, uint32_t height, uint32_t flags);
bool device_texture_create_p010_d3d11(gs_device_t *device, gs_texture_t **p_tex_y, gs_texture_t **p_tex_uv,
			      uint32_t width, uint32_t height, uint32_t flags);
gs_stagesurf_t *device_stagesurface_create_nv12_d3d11(gs_device_t *device, uint32_t width, uint32_t height);
gs_stagesurf_t *device_stagesurface_create_p010_d3d11(gs_device_t *device, uint32_t width, uint32_t height);
void device_register_loss_callbacks_d3d11(gs_device_t *device, const struct gs_device_loss *callbacks);
void device_unregister_loss_callbacks_d3d11(gs_device_t *device, void *data);
uint32_t gs_get_adapter_count_d3d11(void);
bool device_can_adapter_fast_clear_d3d11(gs_device_t *device);

void gs_shader_destroy_d3d11(gs_shader_t *shader);
int gs_shader_get_num_params_d3d11(const gs_shader_t *shader);
gs_sparam_t *gs_shader_get_param_by_idx_d3d11(gs_shader_t *shader, uint32_t param);
gs_sparam_t *gs_shader_get_param_by_name_d3d11(gs_shader_t *shader, const char *name);
gs_sparam_t *gs_shader_get_viewproj_matrix_d3d11(const gs_shader_t *shader);
gs_sparam_t *gs_shader_get_world_matrix_d3d11(const gs_shader_t *shader);
void gs_shader_get_param_info_d3d11(const gs_sparam_t *param, struct gs_shader_param_info *info);
void gs_shader_set_bool_d3d11(gs_sparam_t *param, bool val);
void gs_shader_set_float_d3d11(gs_sparam_t *param, float val);
void gs_shader_set_int_d3d11(gs_sparam_t *param, int val);
void gs_shader_set_matrix3_d3d11(gs_sparam_t *param, const struct matrix3 *val);
void gs_shader_set_matrix4_d3d11(gs_sparam_t *param, const struct matrix4 *val);
void gs_shader_set_vec2_d3d11(gs_sparam_t *param, const struct vec2 *val);
void gs_shader_set_vec3_d3d11(gs_sparam_t *param, const struct vec3 *val);
void gs_shader_set_vec4_d3d11(gs_sparam_t *param, const struct vec4 *val);
void gs_shader_set_texture_d3d11(gs_sparam_t *param, gs_texture_t *val);
void gs_shader_set_val_d3d11(gs_sparam_t *param, const void *val, size_t size);
void gs_shader_set_default_d3d11(gs_sparam_t *param);
void gs_shader_set_next_sampler_d3d11(gs_sparam_t *param, gs_samplerstate_t *sampler);

bool device_get_duplicator_monitor_info_d3d11(gs_device_t *device, int monitor_idx, struct gs_monitor_info *info);
int device_duplicator_get_monitor_index_d3d11(gs_device_t *device, void *monitor);
gs_duplicator_t *device_duplicator_create_d3d11(gs_device_t *device, int monitor_idx);
void gs_duplicator_destroy_d3d11(gs_duplicator_t *duplicator);
bool gs_duplicator_update_frame_d3d11(gs_duplicator_t *d);
gs_texture_t *gs_duplicator_get_texture_d3d11(gs_duplicator_t *duplicator);
enum gs_color_space gs_duplicator_get_color_space_d3d11(gs_duplicator_t *duplicator);
float gs_duplicator_get_sdr_white_level_d3d11(gs_duplicator_t *duplicator);

bool load_graphics_imports(struct gs_exports *exports, const char *subsystem_name)
{
	bool success = true;

	memset(exports, 0, sizeof(*exports));

	if (!strcmp(subsystem_name, "d3d11")) {

		GRAPHICS_IMPORT_D3D11(device_get_name);
		GRAPHICS_IMPORT_D3D11(device_get_type);
		GRAPHICS_IMPORT_D3D11(device_enum_adapters);
		GRAPHICS_IMPORT_D3D11(device_preprocessor_name);
		GRAPHICS_IMPORT_D3D11(device_create);
		GRAPHICS_IMPORT_D3D11(device_destroy);
		GRAPHICS_IMPORT_D3D11(device_enter_context);
		GRAPHICS_IMPORT_D3D11(device_leave_context);
		GRAPHICS_IMPORT_D3D11(device_get_device_obj);
		GRAPHICS_IMPORT_D3D11(device_swapchain_create);
		GRAPHICS_IMPORT_D3D11(device_resize);
		GRAPHICS_IMPORT_D3D11(device_get_color_space);
		GRAPHICS_IMPORT_D3D11(device_update_color_space);
		GRAPHICS_IMPORT_D3D11(device_get_size);
		GRAPHICS_IMPORT_D3D11(device_get_width);
		GRAPHICS_IMPORT_D3D11(device_get_height);
		GRAPHICS_IMPORT_D3D11(device_texture_create);
		GRAPHICS_IMPORT_D3D11(device_cubetexture_create);
		GRAPHICS_IMPORT_D3D11(device_voltexture_create);
		GRAPHICS_IMPORT_D3D11(device_zstencil_create);
		GRAPHICS_IMPORT_D3D11(device_stagesurface_create);
		GRAPHICS_IMPORT_D3D11(device_samplerstate_create);
		GRAPHICS_IMPORT_D3D11(device_vertexshader_create);
		GRAPHICS_IMPORT_D3D11(device_pixelshader_create);
		GRAPHICS_IMPORT_D3D11(device_vertexbuffer_create);
		GRAPHICS_IMPORT_D3D11(device_indexbuffer_create);
		GRAPHICS_IMPORT_D3D11(device_timer_create);
		GRAPHICS_IMPORT_D3D11(device_timer_range_create);
		GRAPHICS_IMPORT_D3D11(device_get_texture_type);
		GRAPHICS_IMPORT_D3D11(device_load_vertexbuffer);
		GRAPHICS_IMPORT_D3D11(device_load_indexbuffer);
		GRAPHICS_IMPORT_D3D11(device_load_texture);
		GRAPHICS_IMPORT_D3D11(device_load_samplerstate);
		GRAPHICS_IMPORT_D3D11(device_load_vertexshader);
		GRAPHICS_IMPORT_D3D11(device_load_pixelshader);
		GRAPHICS_IMPORT_D3D11(device_load_default_samplerstate);
		GRAPHICS_IMPORT_D3D11(device_get_vertex_shader);
		GRAPHICS_IMPORT_D3D11(device_get_pixel_shader);
		GRAPHICS_IMPORT_D3D11(device_get_render_target);
		GRAPHICS_IMPORT_D3D11(device_get_zstencil_target);
		GRAPHICS_IMPORT_D3D11(device_set_render_target);
		GRAPHICS_IMPORT_D3D11(device_set_render_target_with_color_space);
		GRAPHICS_IMPORT_D3D11(device_set_cube_render_target);
		GRAPHICS_IMPORT_D3D11(device_enable_framebuffer_srgb);
		GRAPHICS_IMPORT_D3D11(device_framebuffer_srgb_enabled);
		GRAPHICS_IMPORT_D3D11(device_copy_texture_region);
		GRAPHICS_IMPORT_D3D11(device_copy_texture);
		GRAPHICS_IMPORT_D3D11(device_stage_texture);
		GRAPHICS_IMPORT_D3D11(device_begin_frame);
		GRAPHICS_IMPORT_D3D11(device_begin_scene);
		GRAPHICS_IMPORT_D3D11(device_draw);
		GRAPHICS_IMPORT_D3D11(device_load_swapchain);
		GRAPHICS_IMPORT_D3D11(device_end_scene);
		GRAPHICS_IMPORT_D3D11(device_clear);
		GRAPHICS_IMPORT_D3D11(device_is_present_ready);
		GRAPHICS_IMPORT_D3D11(device_present);
		GRAPHICS_IMPORT_D3D11(device_flush);
		GRAPHICS_IMPORT_D3D11(device_set_cull_mode);
		GRAPHICS_IMPORT_D3D11(device_get_cull_mode);
		GRAPHICS_IMPORT_D3D11(device_enable_blending);
		GRAPHICS_IMPORT_D3D11(device_enable_depth_test);
		GRAPHICS_IMPORT_D3D11(device_enable_stencil_test);
		GRAPHICS_IMPORT_D3D11(device_enable_stencil_write);
		GRAPHICS_IMPORT_D3D11(device_enable_color);
		GRAPHICS_IMPORT_D3D11(device_blend_function);
		GRAPHICS_IMPORT_D3D11(device_blend_function_separate);
		GRAPHICS_IMPORT_D3D11(device_blend_op);
		GRAPHICS_IMPORT_D3D11(device_depth_function);
		GRAPHICS_IMPORT_D3D11(device_stencil_function);
		GRAPHICS_IMPORT_D3D11(device_stencil_op);
		GRAPHICS_IMPORT_D3D11(device_set_viewport);
		GRAPHICS_IMPORT_D3D11(device_get_viewport);
		GRAPHICS_IMPORT_D3D11(device_set_scissor_rect);
		GRAPHICS_IMPORT_D3D11(device_ortho);
		GRAPHICS_IMPORT_D3D11(device_frustum);
		GRAPHICS_IMPORT_D3D11(device_projection_push);
		GRAPHICS_IMPORT_D3D11(device_projection_pop);

		GRAPHICS_IMPORT_D3D11(gs_swapchain_destroy);

		GRAPHICS_IMPORT_D3D11(gs_texture_destroy);
		GRAPHICS_IMPORT_D3D11(gs_texture_get_width);
		GRAPHICS_IMPORT_D3D11(gs_texture_get_height);
		GRAPHICS_IMPORT_D3D11(gs_texture_get_color_format);
		GRAPHICS_IMPORT_D3D11(gs_texture_map);
		GRAPHICS_IMPORT_D3D11(gs_texture_unmap);
		GRAPHICS_IMPORT_D3D11(gs_texture_is_rect);
		GRAPHICS_IMPORT_D3D11(gs_texture_get_obj);

		GRAPHICS_IMPORT_D3D11(gs_cubetexture_destroy);
		GRAPHICS_IMPORT_D3D11(gs_cubetexture_get_size);
		GRAPHICS_IMPORT_D3D11(gs_cubetexture_get_color_format);

		GRAPHICS_IMPORT_D3D11(gs_voltexture_destroy);
		GRAPHICS_IMPORT_D3D11(gs_voltexture_get_width);
		GRAPHICS_IMPORT_D3D11(gs_voltexture_get_height);
		GRAPHICS_IMPORT_D3D11(gs_voltexture_get_depth);
		GRAPHICS_IMPORT_D3D11(gs_voltexture_get_color_format);

		GRAPHICS_IMPORT_D3D11(gs_stagesurface_destroy);
		GRAPHICS_IMPORT_D3D11(gs_stagesurface_get_width);
		GRAPHICS_IMPORT_D3D11(gs_stagesurface_get_height);
		GRAPHICS_IMPORT_D3D11(gs_stagesurface_get_color_format);
		GRAPHICS_IMPORT_D3D11(gs_stagesurface_map);
		GRAPHICS_IMPORT_D3D11(gs_stagesurface_unmap);

		GRAPHICS_IMPORT_D3D11(gs_zstencil_destroy);

		GRAPHICS_IMPORT_D3D11(gs_samplerstate_destroy);

		GRAPHICS_IMPORT_D3D11(gs_vertexbuffer_destroy);
		GRAPHICS_IMPORT_D3D11(gs_vertexbuffer_flush);
		GRAPHICS_IMPORT_D3D11(gs_vertexbuffer_flush_direct);
		GRAPHICS_IMPORT_D3D11(gs_vertexbuffer_get_data);

		GRAPHICS_IMPORT_D3D11(gs_indexbuffer_destroy);
		GRAPHICS_IMPORT_D3D11(gs_indexbuffer_flush);
		GRAPHICS_IMPORT_D3D11(gs_indexbuffer_flush_direct);
		GRAPHICS_IMPORT_D3D11(gs_indexbuffer_get_data);
		GRAPHICS_IMPORT_D3D11(gs_indexbuffer_get_num_indices);
		GRAPHICS_IMPORT_D3D11(gs_indexbuffer_get_type);

		GRAPHICS_IMPORT_D3D11(gs_timer_destroy);
		GRAPHICS_IMPORT_D3D11(gs_timer_begin);
		GRAPHICS_IMPORT_D3D11(gs_timer_end);
		GRAPHICS_IMPORT_D3D11(gs_timer_get_data);
		GRAPHICS_IMPORT_D3D11(gs_timer_range_destroy);
		GRAPHICS_IMPORT_D3D11(gs_timer_range_begin);
		GRAPHICS_IMPORT_D3D11(gs_timer_range_end);
		GRAPHICS_IMPORT_D3D11(gs_timer_range_get_data);

		GRAPHICS_IMPORT_D3D11(gs_shader_destroy);
		GRAPHICS_IMPORT_D3D11(gs_shader_get_num_params);
		GRAPHICS_IMPORT_D3D11(gs_shader_get_param_by_idx);
		GRAPHICS_IMPORT_D3D11(gs_shader_get_param_by_name);
		GRAPHICS_IMPORT_D3D11(gs_shader_get_viewproj_matrix);
		GRAPHICS_IMPORT_D3D11(gs_shader_get_world_matrix);
		GRAPHICS_IMPORT_D3D11(gs_shader_get_param_info);
		GRAPHICS_IMPORT_D3D11(gs_shader_set_bool);
		GRAPHICS_IMPORT_D3D11(gs_shader_set_float);
		GRAPHICS_IMPORT_D3D11(gs_shader_set_int);
		GRAPHICS_IMPORT_D3D11(gs_shader_set_matrix3);
		GRAPHICS_IMPORT_D3D11(gs_shader_set_matrix4);
		GRAPHICS_IMPORT_D3D11(gs_shader_set_vec2);
		GRAPHICS_IMPORT_D3D11(gs_shader_set_vec3);
		GRAPHICS_IMPORT_D3D11(gs_shader_set_vec4);
		GRAPHICS_IMPORT_D3D11(gs_shader_set_texture);
		GRAPHICS_IMPORT_D3D11(gs_shader_set_val);
		GRAPHICS_IMPORT_D3D11(gs_shader_set_default);
		GRAPHICS_IMPORT_D3D11(gs_shader_set_next_sampler);

		GRAPHICS_IMPORT_D3D11(device_nv12_available);
		GRAPHICS_IMPORT_D3D11(device_p010_available);
		GRAPHICS_IMPORT_D3D11(device_texture_create_nv12);
		GRAPHICS_IMPORT_D3D11(device_texture_create_p010);

		GRAPHICS_IMPORT_D3D11(device_is_monitor_hdr);

		GRAPHICS_IMPORT_D3D11(device_debug_marker_begin);
		GRAPHICS_IMPORT_D3D11(device_debug_marker_end);

		GRAPHICS_IMPORT_D3D11(gs_get_adapter_count);

		/* OSX/Cocoa specific functions */
#ifdef __APPLE__
		GRAPHICS_IMPORT(device_shared_texture_available);
		GRAPHICS_IMPORT(device_texture_open_shared);
		GRAPHICS_IMPORT(device_texture_create_from_iosurface);
		GRAPHICS_IMPORT(gs_texture_rebind_iosurface);

		/* win32 specific functions */
#elif _WIN32
		GRAPHICS_IMPORT_D3D11(device_gdi_texture_available);
		GRAPHICS_IMPORT_D3D11(device_shared_texture_available);
		GRAPHICS_IMPORT_D3D11(device_get_duplicator_monitor_info);
		GRAPHICS_IMPORT_D3D11(device_duplicator_get_monitor_index);
		GRAPHICS_IMPORT_D3D11(device_duplicator_create);
		GRAPHICS_IMPORT_D3D11(gs_duplicator_destroy);
		GRAPHICS_IMPORT_D3D11(gs_duplicator_update_frame);
		GRAPHICS_IMPORT_D3D11(gs_duplicator_get_texture);
		GRAPHICS_IMPORT_D3D11(gs_duplicator_get_color_space);
		GRAPHICS_IMPORT_D3D11(gs_duplicator_get_sdr_white_level);
		GRAPHICS_IMPORT_D3D11(device_can_adapter_fast_clear);
		GRAPHICS_IMPORT_D3D11(device_texture_create_gdi);
		GRAPHICS_IMPORT_D3D11(gs_texture_get_dc);
		GRAPHICS_IMPORT_D3D11(gs_texture_release_dc);
		GRAPHICS_IMPORT_D3D11(device_texture_open_shared);
		GRAPHICS_IMPORT_D3D11(device_texture_open_nt_shared);
		GRAPHICS_IMPORT_D3D11(device_texture_get_shared_handle);
		GRAPHICS_IMPORT_D3D11(device_texture_wrap_obj);
		GRAPHICS_IMPORT_D3D11(device_texture_acquire_sync);
		GRAPHICS_IMPORT_D3D11(device_texture_release_sync);
		GRAPHICS_IMPORT_D3D11(device_stagesurface_create_nv12);
		GRAPHICS_IMPORT_D3D11(device_stagesurface_create_p010);
		GRAPHICS_IMPORT_D3D11(device_register_loss_callbacks);
		GRAPHICS_IMPORT_D3D11(device_unregister_loss_callbacks);
#elif defined(__linux__) || defined(__FreeBSD__) || defined(__DragonFly__)
		GRAPHICS_IMPORT(device_texture_create_from_dmabuf);
		GRAPHICS_IMPORT(device_query_dmabuf_capabilities);
		GRAPHICS_IMPORT(device_query_dmabuf_modifiers_for_format);
		GRAPHICS_IMPORT(device_texture_create_from_pixmap);
#endif

	}

	return success;
}
