#ifndef VEGA_ENGINE_VULKAN_INSTANCE_H
#define VEGA_ENGINE_VULKAN_INSTANCE_H

#include <stdint.h>

#include <vega/core/core.h>

#include <vulkan/vulkan.h>

extern VkInstance g_instance;

extern VkSurfaceKHR g_surface;
extern VkSurfaceCapabilitiesKHR g_surface_capabilities;

extern VkPhysicalDevice g_physical_device;
extern VkPhysicalDeviceProperties g_physical_device_properties;
extern VkPhysicalDeviceFeatures g_physical_device_features;

extern int32_t g_graphic_and_compute_queue_index;
extern int32_t g_present_queue_index;

extern VkDevice g_device;

extern VkQueue g_graphic_queue;
extern VkQueue g_compute_queue;
extern VkQueue g_present_queue;

extern VkSurfaceFormatKHR g_prefered_surface_format;
extern VkPresentModeKHR g_prefered_present_mode;

extern VkCommandPool g_command_pool;

///////////////////////////////////////////////////////////////
// Public API
///////////////////////////////////////////////////////////////

void vulkan_instance_alloc(void);
void vulkan_instance_resize(void);
void vulkan_instance_free(void);

///////////////////////////////////////////////////////////////
// Internal API
///////////////////////////////////////////////////////////////

void vulkan_instance_surface_alloc(void);
void vulkan_instance_device_alloc(void);
void vulkan_instance_command_pool_alloc(void);

void vulkan_instance_find_physical_device(void);
void vulkan_instance_find_physical_device_queue_families(void);
VkFormat vulkan_instance_find_depth_format(void);
VkFormat vulkan_instance_find_supported_format(VkFormat* formats, uint64_t format_count, VkImageTiling tiling, VkFormatFeatureFlags features);

void vulkan_instance_check_physical_device_extensions(void);
void vulkan_instance_check_surface_capabilities(void);

void vulkan_instance_surface_free(void);
void vulkan_instance_device_free(void);
void vulkan_instance_command_pool_free(void);

#endif // VEGA_ENGINE_VULKAN_INSTANCE_H
