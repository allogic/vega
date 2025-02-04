#ifndef VEGA_ENGINE_VULKAN_FORWARD_H
#define VEGA_ENGINE_VULKAN_FORWARD_H

#include <stdint.h>

#include <vulkan/vulkan.h>

VEGA_EXTERN_C_BEGIN

typedef struct _buffer_t
{
	uint64_t size;
	VkBufferUsageFlags buffer_usage;
	VkMemoryPropertyFlags memory_properties;
	VkBuffer buffer;
	VkDeviceMemory device_memory;
	void* mapped_buffer;
} buffer_t;

typedef struct _image_t
{
	uint64_t size;
	uint32_t width;
	uint32_t height;
	uint32_t depth;
	uint32_t channels;
	VkImageType type;
	VkImageViewType view_type;
	VkImageUsageFlags image_usage;
	VkFormat format;
	VkImageTiling tiling;
	VkFilter filter;
	VkImage image;
	VkImageView image_view;
	VkSampler sampler;
	VkDeviceMemory device_memory;
	void* mapped_buffer;
} image_t;

VEGA_EXTERN_C_END

#endif // VEGA_ENGINE_VULKAN_FORWARD_H
