#ifndef VEGA_ENGINE_VULKAN_SURFACE_H
#define VEGA_ENGINE_VULKAN_SURFACE_H

#include <stdint.h>

#include <vega/core/core.h>

#include <vulkan/vulkan.h>

VEGA_EXTERN_C_BEGIN

extern VkSurfaceKHR g_vulkan_surface;
extern VkSurfaceCapabilitiesKHR g_vulkan_surface_capabilities;
extern VkSurfaceFormatKHR g_vulkan_surface_prefered_format;

extern VkPresentModeKHR g_vulkan_surface_prefered_present_mode;

extern uint32_t g_vulkan_surface_width;
extern uint32_t g_vulkan_surface_height;

///////////////////////////////////////////////////////////////
// Public API
///////////////////////////////////////////////////////////////

void vulkan_surface_alloc(void);
void vulkan_surface_check_capabilities(void);
void vulkan_surface_resize(void);
void vulkan_surface_free(void);

VEGA_EXTERN_C_END

#endif // VEGA_ENGINE_VULKAN_SURFACE_H
