#ifndef VEGA_ENGINE_VULKAN_RENDERER_H
#define VEGA_ENGINE_VULKAN_RENDERER_H

#include <stdint.h>

#include <vega/core/core.h>

#include <vulkan/vulkan.h>

typedef struct _time_info_t
{
	double time;
	double delta_time;
} time_info_t;

typedef struct _screen_info_t
{
	vector2_t size;
} screen_info_t;

typedef struct _projection_info_t
{
	matrix4_t view;
	matrix4_t projection;
} projection_info_t;

///////////////////////////////////////////////////////////////
// Public API
///////////////////////////////////////////////////////////////

void vulkan_renderer_alloc(void);
void vulkan_renderer_render(void);
void vulkan_renderer_free(void);

void vulkan_renderer_resize_before(void);
void vulkan_renderer_resize_after(void);

///////////////////////////////////////////////////////////////
// Internal API
///////////////////////////////////////////////////////////////

void vulkan_renderer_command_buffer_alloc(void);
void vulkan_renderer_sync_objects_alloc(void);
void vulkan_renderer_render_pass_alloc(void);
void vulkan_renderer_frame_buffer_alloc(void);

void vulkan_renderer_update_uniform_buffers(void);

void vulkan_renderer_record_compute_command_buffer(void);
void vulkan_renderer_record_graphic_command_buffer(uint32_t image_index);

void vulkan_renderer_command_buffer_free(void);
void vulkan_renderer_sync_objects_free(void);
void vulkan_renderer_render_pass_free(void);
void vulkan_renderer_frame_buffer_free(void);

#endif // VEGA_ENGINE_VULKAN_RENDERER_H
