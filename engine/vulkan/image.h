#ifndef VEGA_ENGINE_VULKAN_IMAGE_H
#define VEGA_ENGINE_VULKAN_IMAGE_H

#include <stdint.h>

#include <vega/core/core.h>

#include <vega/engine/vulkan/forward.h>

#include <vulkan/vulkan.h>

VEGA_EXTERN_C_BEGIN

///////////////////////////////////////////////////////////////
// Public API
///////////////////////////////////////////////////////////////

image_t vulkan_image_2d_r_alloc(void* buffer, uint32_t width, uint32_t height, VkFormat format);
image_t vulkan_image_2d_rg_alloc(void* buffer, uint32_t width, uint32_t height, VkFormat format);
image_t vulkan_image_2d_rgb_alloc(void* buffer, uint32_t width, uint32_t height, VkFormat format);
image_t vulkan_image_2d_rgba_alloc(void* buffer, uint32_t width, uint32_t height, VkFormat format);
image_t vulkan_image_2d_depth_alloc(uint32_t width, uint32_t height, VkFormat format);

image_t vulkan_image_alloc(uint32_t width, uint32_t height, uint32_t depth, uint32_t channels, VkImageType type, VkImageViewType view_type, VkImageUsageFlags image_usage, VkMemoryPropertyFlags memory_properties, VkImageAspectFlags aspect_flags, VkFormat format, VkImageTiling tiling, VkFilter filter);
void vulkan_image_copy_to_image(image_t* image, image_t* target, VkCommandBuffer command_buffer);
void vulkan_image_layout_transition(image_t* image, VkCommandBuffer command_buffer, VkImageLayout old_layout, VkImageLayout new_layout);
void vulkan_image_map(image_t* image);
void vulkan_image_unmap(image_t* image);
VkFormat vulkan_image_find_depth_format(void);
VkFormat vulkan_image_find_supported_format(VkFormat* formats, uint64_t format_count, VkImageTiling tiling, VkFormatFeatureFlags features);
VkImageView vulkan_image_view_alloc(VkImage image, VkImageViewType view_type, VkFormat format, VkImageAspectFlags aspect_flags);
VkSampler vulkan_image_sampler_alloc(VkFilter filter);
void vulkan_image_view_free(VkImageView image_view);
void vulkan_image_sampler_free(VkSampler sampler);
void vulkan_image_free(image_t* image);

VEGA_EXTERN_C_END

#endif // VEGA_ENGINE_VULKAN_IMAGE_H
