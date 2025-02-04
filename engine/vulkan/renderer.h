#ifndef VEGA_ENGINE_VULKAN_RENDERER_H
#define VEGA_ENGINE_VULKAN_RENDERER_H

#include <stdint.h>

#include <vega/core/core.h>

#include <vulkan/vulkan.h>

VEGA_EXTERN_C_BEGIN

typedef struct _time_info_t
{
	double time;
	double delta_time;
} time_info_t;

typedef struct _screen_info_t
{
	vector2_t size;
} screen_info_t;

typedef struct _camera_info_t
{
	matrix4_t view;
	matrix4_t projection;
} camera_info_t;

typedef struct _entity_info_t
{
	matrix4_t model;
} entity_info_t;

typedef struct _index_info_t
{
	uint32_t entity_info_index;
} index_info_t;

///////////////////////////////////////////////////////////////
// Public API
///////////////////////////////////////////////////////////////

void vulkan_renderer_alloc(void);
void vulkan_renderer_free(void);

void vulkan_renderer_begin(void);
void vulkan_renderer_end(void);

void vulkan_renderer_compute_begin(void);
void vulkan_renderer_compute_end(void);

void vulkan_renderer_graphic_begin(void);
void vulkan_renderer_graphic_end(void);

void vulkan_renderer_geometry_pass_begin(void);
void vulkan_renderer_geometry_pass(void);
void vulkan_renderer_imgui_pass(void); // TODO: move this past post-processing pass..
void vulkan_renderer_geometry_pass_end(void);

void vulkan_renderer_resize_before(void);
void vulkan_renderer_resize(void);
void vulkan_renderer_resize_after(void);

void vulkan_renderer_build_pbr_descriptor_sets(uint64_t descriptor_count);
void vulkan_renderer_update_pbr_descriptor_sets(void);

///////////////////////////////////////////////////////////////
// Internal API
///////////////////////////////////////////////////////////////

void vulkan_renderer_command_buffer_alloc(void);
void vulkan_renderer_sync_objects_alloc(void);
void vulkan_renderer_render_pass_alloc(void);
void vulkan_renderer_frame_buffer_alloc(void);
void vulkan_renderer_imgui_context_alloc(void);

void vulkan_renderer_update_uniform_buffers(void);

void vulkan_renderer_command_buffer_free(void);
void vulkan_renderer_sync_objects_free(void);
void vulkan_renderer_render_pass_free(void);
void vulkan_renderer_frame_buffer_free(void);
void vulkan_renderer_imgui_context_free(void);

VEGA_EXTERN_C_END

#endif // VEGA_ENGINE_VULKAN_RENDERER_H
