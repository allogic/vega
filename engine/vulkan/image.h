#ifndef VEGA_ENGINE_VULKAN_IMAGE_H
#define VEGA_ENGINE_VULKAN_IMAGE_H

#include <stdint.h>

#include <vega/core/core.h>

#include <vulkan/vulkan.h>

typedef struct _image_t
{
	uint64_t size;
	uint32_t width;
	uint32_t height;
	uint32_t depth;
	uint32_t channels;
	VkImageType type;
	VkImageViewType view_type;
	VkFormat format;
	VkImageTiling tiling;
	VkFilter filter;
	VkImage image;
	VkImageView image_view;
	VkSampler sampler;
	VkDeviceMemory device_memory;
	void* mapped_buffer;
} image_t;

///////////////////////////////////////////////////////////////
// Public API
///////////////////////////////////////////////////////////////

image_t vulkan_image_2d_r_alloc(void* buffer, uint32_t width, uint32_t height, VkFormat format);
image_t vulkan_image_2d_rg_alloc(void* buffer, uint32_t width, uint32_t height, VkFormat format);
image_t vulkan_image_2d_rgb_alloc(void* buffer, uint32_t width, uint32_t height, VkFormat format);
image_t vulkan_image_2d_rgba_alloc(void* buffer, uint32_t width, uint32_t height, VkFormat format);
image_t vulkan_image_2d_depth_alloc(uint32_t width, uint32_t height, VkFormat format);

void vulkan_image_free(image_t* image);

VkImageView vulkan_image_view_alloc(VkImage image, VkImageViewType view_type, VkFormat format, VkImageAspectFlags aspect_flags);
void vulkan_image_view_free(VkImageView image_view);

#endif // VEGA_ENGINE_VULKAN_IMAGE_H
