#ifndef VEGA_ENGINE_VULKAN_INSTANCE_H
#define VEGA_ENGINE_VULKAN_INSTANCE_H

#include <stdint.h>

#include <vega/core/core.h>

#include <vega/engine/vulkan/forward.h>

#include <vulkan/vulkan.h>

extern VkInstance g_vulkan_instance;

extern VkSurfaceKHR g_vulkan_instance_surface;
extern VkSurfaceCapabilitiesKHR g_vulkan_instance_surface_capabilities;

extern VkPhysicalDevice g_vulkan_instance_physical_device;
extern VkPhysicalDeviceProperties g_vulkan_instance_physical_device_properties;
extern VkPhysicalDeviceFeatures g_vulkan_instance_physical_device_features;

extern int32_t g_vulkan_instance_graphic_and_compute_queue_index;
extern int32_t g_vulkan_instance_present_queue_index;

extern VkDevice g_vulkan_instance_device;

extern VkQueue g_vulkan_instance_graphic_queue;
extern VkQueue g_vulkan_instance_compute_queue;
extern VkQueue g_vulkan_instance_present_queue;

extern VkSurfaceFormatKHR g_vulkan_instance_prefered_surface_format;
extern VkPresentModeKHR g_vulkan_instance_prefered_present_mode;

extern VkCommandPool g_vulkan_instance_command_pool;

///////////////////////////////////////////////////////////////
// Public API
///////////////////////////////////////////////////////////////

void vulkan_instance_alloc(void);
void vulkan_instance_free(void);

///////////////////////////////////////////////////////////////
// Internal API
///////////////////////////////////////////////////////////////

void vulkan_instance_surface_alloc(void);
void vulkan_instance_device_alloc(void);
void vulkan_instance_command_pool_alloc(void);

void vulkan_instance_find_physical_device(void);
void vulkan_instance_find_physical_device_queue_families(void);

void vulkan_instance_check_physical_device_extensions(void);
void vulkan_instance_check_surface_capabilities(void);

void vulkan_instance_surface_free(void);
void vulkan_instance_device_free(void);
void vulkan_instance_command_pool_free(void);

#endif // VEGA_ENGINE_VULKAN_INSTANCE_H
