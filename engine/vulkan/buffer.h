#ifndef VEGA_ENGINE_VULKAN_BUFFER_H
#define VEGA_ENGINE_VULKAN_BUFFER_H

#include <stdint.h>

#include <vega/core/core.h>

#include <vega/engine/vulkan/forward.h>

#include <vulkan/vulkan.h>

///////////////////////////////////////////////////////////////
// Public API
///////////////////////////////////////////////////////////////

buffer_t vulkan_buffer_alloc(uint64_t size, VkBufferUsageFlags usage, VkMemoryPropertyFlags memory_properties);
void vulkan_buffer_copy_to_buffer(buffer_t* buffer, buffer_t* target, VkCommandBuffer command_buffer);
void vulkan_buffer_copy_to_image(buffer_t* buffer, image_t* target, VkCommandBuffer command_buffer, uint32_t width, uint32_t height, uint32_t depth, VkImageAspectFlags aspect_flags);
void vulkan_buffer_map(buffer_t* buffer);
void vulkan_buffer_unmap(buffer_t* buffer);
void vulkan_buffer_free(buffer_t* buffer);

#endif // VEGA_ENGINE_VULKAN_BUFFER_H
