#ifndef VEGA_ENGINE_VULKAN_DEVICE_H
#define VEGA_ENGINE_VULKAN_DEVICE_H

#include <stdint.h>

#include <vega/core/core.h>

#include <vulkan/vulkan.h>

VEGA_EXTERN_C_BEGIN

extern VkPhysicalDevice g_vulkan_device_physical;
extern VkPhysicalDeviceProperties g_vulkan_device_physical_properties;
extern VkPhysicalDeviceFeatures g_vulkan_device_physical_features;

extern VkDevice g_vulkan_device;

extern int32_t g_vulkan_device_graphic_queue_index;
extern int32_t g_vulkan_device_present_queue_index;

extern VkQueue g_vulkan_device_graphic_queue;
extern VkQueue g_vulkan_device_compute_queue;
extern VkQueue g_vulkan_device_present_queue;

///////////////////////////////////////////////////////////////
// Public API
///////////////////////////////////////////////////////////////

void vulkan_device_alloc(void);
void vulkan_device_find_physical(void);
void vulkan_device_find_physical_queue_families(void);
void vulkan_device_check_physical_extensions(void);
void vulkan_device_free(void);

VEGA_EXTERN_C_END

#endif // VEGA_ENGINE_VULKAN_DEVICE_H
