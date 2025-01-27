#include <string.h>

#include <vega/engine/vulkan/buffer.h>
#include <vega/engine/vulkan/command_buffer.h>
#include <vega/engine/vulkan/instance.h>
#include <vega/engine/vulkan/image.h>
#include <vega/engine/vulkan/vulkan.h>

#ifndef TRACY_ZONE_BEGIN
	#define TRACY_ZONE_BEGIN TracyCZoneC(ctx, TRACY_COLOR_GREEN, 1U);
#endif // TRACY_ZONE_BEGIN

#ifndef TRACY_ZONE_END
	#define TRACY_ZONE_END TracyCZoneEnd(ctx);
#endif // TRACY_ZONE_END

image_t vulkan_image_2d_r_alloc(void* buffer, uint32_t width, uint32_t height, VkFormat format)
{
	TRACY_ZONE_BEGIN

	buffer_t staging_buffer = vulkan_buffer_alloc(width * height * 1, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);

	vulkan_buffer_map(&staging_buffer);
	memcpy(staging_buffer.mapped_buffer, buffer, staging_buffer.size);
	vulkan_buffer_unmap(&staging_buffer);

	image_t image = vulkan_image_alloc(width, height, 1, 1, VK_IMAGE_TYPE_2D, VK_IMAGE_VIEW_TYPE_2D, VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, VK_IMAGE_ASPECT_COLOR_BIT, format, VK_IMAGE_TILING_OPTIMAL, VK_FILTER_NEAREST);

	VkCommandBuffer command_buffer = vulkan_command_buffer_begin();

	vulkan_image_layout_transition(&image, command_buffer, VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL);
	vulkan_buffer_copy_to_image(&staging_buffer, &image, command_buffer, width, height, 1, VK_IMAGE_ASPECT_COLOR_BIT);
	vulkan_image_layout_transition(&image, command_buffer, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, VK_IMAGE_LAYOUT_GENERAL);

	vulkan_command_buffer_end(command_buffer);

	vulkan_buffer_free(&staging_buffer);

	TRACY_ZONE_END

	return image;
}
image_t vulkan_image_2d_rg_alloc(void* buffer, uint32_t width, uint32_t height, VkFormat format)
{
	TRACY_ZONE_BEGIN

	buffer_t staging_buffer = vulkan_buffer_alloc(width * height * 2, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);

	vulkan_buffer_map(&staging_buffer);
	memcpy(staging_buffer.mapped_buffer, buffer, staging_buffer.size);
	vulkan_buffer_unmap(&staging_buffer);

	image_t image = vulkan_image_alloc(width, height, 1, 2, VK_IMAGE_TYPE_2D, VK_IMAGE_VIEW_TYPE_2D, VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, VK_IMAGE_ASPECT_COLOR_BIT, format, VK_IMAGE_TILING_OPTIMAL, VK_FILTER_NEAREST);

	VkCommandBuffer command_buffer = vulkan_command_buffer_begin();

	vulkan_image_layout_transition(&image, command_buffer, VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL);
	vulkan_buffer_copy_to_image(&staging_buffer, &image, command_buffer, width, height, 1, VK_IMAGE_ASPECT_COLOR_BIT);
	vulkan_image_layout_transition(&image, command_buffer, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, VK_IMAGE_LAYOUT_GENERAL);

	vulkan_command_buffer_end(command_buffer);

	vulkan_buffer_free(&staging_buffer);

	TRACY_ZONE_END

	return image;
}
image_t vulkan_image_2d_rgb_alloc(void* buffer, uint32_t width, uint32_t height, VkFormat format)
{
	TRACY_ZONE_BEGIN

	buffer_t staging_buffer = vulkan_buffer_alloc(width * height * 3, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);

	vulkan_buffer_map(&staging_buffer);
	memcpy(staging_buffer.mapped_buffer, buffer, staging_buffer.size);
	vulkan_buffer_unmap(&staging_buffer);

	image_t image = vulkan_image_alloc(width, height, 1, 3, VK_IMAGE_TYPE_2D, VK_IMAGE_VIEW_TYPE_2D, VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, VK_IMAGE_ASPECT_COLOR_BIT, format, VK_IMAGE_TILING_OPTIMAL, VK_FILTER_NEAREST);

	VkCommandBuffer command_buffer = vulkan_command_buffer_begin();

	vulkan_image_layout_transition(&image, command_buffer, VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL);
	vulkan_buffer_copy_to_image(&staging_buffer, &image, command_buffer, width, height, 1, VK_IMAGE_ASPECT_COLOR_BIT);
	vulkan_image_layout_transition(&image, command_buffer, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, VK_IMAGE_LAYOUT_GENERAL);

	vulkan_command_buffer_end(command_buffer);

	vulkan_buffer_free(&staging_buffer);

	TRACY_ZONE_END

	return image;
}
image_t vulkan_image_2d_rgba_alloc(void* buffer, uint32_t width, uint32_t height, VkFormat format)
{
	TRACY_ZONE_BEGIN

	buffer_t staging_buffer = vulkan_buffer_alloc(width * height * 4, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);

	vulkan_buffer_map(&staging_buffer);
	memcpy(staging_buffer.mapped_buffer, buffer, staging_buffer.size);
	vulkan_buffer_unmap(&staging_buffer);

	image_t image = vulkan_image_alloc(width, height, 1, 4, VK_IMAGE_TYPE_2D, VK_IMAGE_VIEW_TYPE_2D, VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, VK_IMAGE_ASPECT_COLOR_BIT, format, VK_IMAGE_TILING_OPTIMAL, VK_FILTER_NEAREST);

	VkCommandBuffer command_buffer = vulkan_command_buffer_begin();

	vulkan_image_layout_transition(&image, command_buffer, VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL);
	vulkan_buffer_copy_to_image(&staging_buffer, &image, command_buffer, width, height, 1, VK_IMAGE_ASPECT_COLOR_BIT);
	vulkan_image_layout_transition(&image, command_buffer, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, VK_IMAGE_LAYOUT_GENERAL);

	vulkan_command_buffer_end(command_buffer);

	vulkan_buffer_free(&staging_buffer);

	TRACY_ZONE_END

	return image;
}
image_t vulkan_image_2d_depth_alloc(uint32_t width, uint32_t height, VkFormat format)
{
	TRACY_ZONE_BEGIN

	image_t image = vulkan_image_alloc(width, height, 1, 1, VK_IMAGE_TYPE_2D, VK_IMAGE_VIEW_TYPE_2D, VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, VK_IMAGE_ASPECT_DEPTH_BIT, format, VK_IMAGE_TILING_OPTIMAL, VK_FILTER_NEAREST);

	TRACY_ZONE_END

	return image;
}
image_t vulkan_image_alloc(uint32_t width, uint32_t height, uint32_t depth, uint32_t channels, VkImageType type, VkImageViewType view_type, VkImageUsageFlags usage, VkMemoryPropertyFlags memory_properties, VkImageAspectFlags aspect_flags, VkFormat format, VkImageTiling tiling, VkFilter filter)
{
	TRACY_ZONE_BEGIN

	image_t image;
	memset(&image, 0, sizeof(image_t));

	image.size = width * height * depth * channels;
	image.width = width;
	image.height = height;
	image.depth = depth;
	image.channels = channels;
	image.type = type;
	image.view_type = view_type;
	image.format = format;
	image.tiling = tiling;
	image.filter = filter;

	VkImageCreateInfo image_create_info;
	memset(&image_create_info, 0, sizeof(VkImageCreateInfo));

	image_create_info.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
	image_create_info.imageType = type;
	image_create_info.extent.width = width;
	image_create_info.extent.height = height;
	image_create_info.extent.depth = depth;
	image_create_info.mipLevels = 1;
	image_create_info.arrayLayers = 1;
	image_create_info.format = format;
	image_create_info.tiling = tiling;
	image_create_info.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
	image_create_info.usage = usage;
	image_create_info.samples = VK_SAMPLE_COUNT_1_BIT;
	image_create_info.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

	vkCreateImage(g_vulkan_instance_device, &image_create_info, 0, &image.image);

	VkMemoryRequirements memory_requirements;
	vkGetImageMemoryRequirements(g_vulkan_instance_device, image.image, &memory_requirements);

	VkMemoryAllocateInfo memory_allocate_info;
	memset(&memory_allocate_info, 0, sizeof(VkMemoryAllocateInfo));

	memory_allocate_info.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
	memory_allocate_info.allocationSize = memory_requirements.size;
	memory_allocate_info.memoryTypeIndex = vulkan_find_memory_type(memory_requirements.memoryTypeBits, memory_properties);

	vkAllocateMemory(g_vulkan_instance_device, &memory_allocate_info, 0, &image.device_memory);
	vkBindImageMemory(g_vulkan_instance_device, image.image, image.device_memory, 0);

	VkImageViewCreateInfo image_view_create_info;
	memset(&image_view_create_info, 0, sizeof(VkImageViewCreateInfo));

	image_view_create_info.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
	image_view_create_info.image = image.image;
	image_view_create_info.viewType = view_type;
	image_view_create_info.format = format;
	image_view_create_info.subresourceRange.aspectMask = aspect_flags;
	image_view_create_info.subresourceRange.baseMipLevel = 0;
	image_view_create_info.subresourceRange.levelCount = 1;
	image_view_create_info.subresourceRange.baseArrayLayer = 0;
	image_view_create_info.subresourceRange.layerCount = 1;

	vkCreateImageView(g_vulkan_instance_device, &image_view_create_info, 0, &image.image_view);

	VkSamplerCreateInfo sampler_create_info;
	memset(&sampler_create_info, 0, sizeof(VkSamplerCreateInfo));

	sampler_create_info.sType = VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO;
	sampler_create_info.magFilter = filter;
	sampler_create_info.minFilter = filter;
	sampler_create_info.addressModeU = VK_SAMPLER_ADDRESS_MODE_REPEAT;
	sampler_create_info.addressModeV = VK_SAMPLER_ADDRESS_MODE_REPEAT;
	sampler_create_info.addressModeW = VK_SAMPLER_ADDRESS_MODE_REPEAT;
	sampler_create_info.anisotropyEnable = 1;
	sampler_create_info.maxAnisotropy = g_vulkan_instance_physical_device_properties.limits.maxSamplerAnisotropy;
	sampler_create_info.borderColor = VK_BORDER_COLOR_INT_OPAQUE_BLACK;
	sampler_create_info.unnormalizedCoordinates = 0;
	sampler_create_info.compareEnable = 0;
	sampler_create_info.compareOp = VK_COMPARE_OP_ALWAYS;
	sampler_create_info.mipmapMode = VK_SAMPLER_MIPMAP_MODE_LINEAR;
	sampler_create_info.mipLodBias = 0.0F;
	sampler_create_info.minLod = 0.0F;
	sampler_create_info.maxLod = 0.0F;

	vkCreateSampler(g_vulkan_instance_device, &sampler_create_info, 0, &image.sampler);

	TRACY_ZONE_END

	return image;
}
void vulkan_image_copy_to_image(image_t* image, image_t* target, VkCommandBuffer command_buffer)
{
	TRACY_ZONE_BEGIN

	VkImageCopy image_copy;
	memset(&image_copy, 0, sizeof(VkImageCopy));

	image_copy.extent.width = image->width;
	image_copy.extent.height = image->height;
	image_copy.extent.depth = image->depth;

	vkCmdCopyImage(command_buffer, image->image, VK_IMAGE_LAYOUT_UNDEFINED, target->image, VK_IMAGE_LAYOUT_UNDEFINED, 1, &image_copy);

	TRACY_ZONE_END
}
void vulkan_image_layout_transition(image_t* image, VkCommandBuffer command_buffer, VkImageLayout old_layout, VkImageLayout new_layout)
{
	TRACY_ZONE_BEGIN

	VkImageMemoryBarrier image_memory_barrier;
	memset(&image_memory_barrier, 0, sizeof(VkImageMemoryBarrier));

	image_memory_barrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
	image_memory_barrier.oldLayout = old_layout;
	image_memory_barrier.newLayout = new_layout;
	image_memory_barrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
	image_memory_barrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
	image_memory_barrier.image = image->image;
	image_memory_barrier.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
	image_memory_barrier.subresourceRange.baseMipLevel = 0;
	image_memory_barrier.subresourceRange.levelCount = 1;
	image_memory_barrier.subresourceRange.baseArrayLayer = 0;
	image_memory_barrier.subresourceRange.layerCount = 1;
	image_memory_barrier.srcAccessMask = 0;
	image_memory_barrier.dstAccessMask = 0;

	VkPipelineStageFlags pipeline_src_stage_flags = VK_PIPELINE_STAGE_NONE;
	VkPipelineStageFlags pipeline_dst_stage_flags = VK_PIPELINE_STAGE_NONE;

	if ((old_layout == VK_IMAGE_LAYOUT_UNDEFINED) && (new_layout == VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL))
	{
		image_memory_barrier.srcAccessMask = 0;
		image_memory_barrier.dstAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;

		pipeline_src_stage_flags = VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT;
		pipeline_dst_stage_flags = VK_PIPELINE_STAGE_TRANSFER_BIT;
	}
	else if ((old_layout == VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL) && (new_layout == VK_IMAGE_LAYOUT_UNDEFINED))
	{
		image_memory_barrier.srcAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
		image_memory_barrier.dstAccessMask = 0;

		pipeline_src_stage_flags = VK_PIPELINE_STAGE_TRANSFER_BIT;
		pipeline_dst_stage_flags = VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT;
	}
	else if ((old_layout == VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL) && (new_layout == VK_IMAGE_LAYOUT_GENERAL))
	{
		image_memory_barrier.srcAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
		image_memory_barrier.dstAccessMask = 0;

		pipeline_src_stage_flags = VK_PIPELINE_STAGE_TRANSFER_BIT;
		pipeline_dst_stage_flags = VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT;
	}
	else if ((old_layout == VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL) && (new_layout == VK_IMAGE_LAYOUT_UNDEFINED))
	{
		image_memory_barrier.srcAccessMask = VK_ACCESS_SHADER_READ_BIT;
		image_memory_barrier.dstAccessMask = 0;

		pipeline_src_stage_flags = VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT;
		pipeline_dst_stage_flags = VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT;
	}
	else if ((old_layout == VK_IMAGE_LAYOUT_UNDEFINED) && (new_layout == VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL))
	{
		image_memory_barrier.srcAccessMask = 0;
		image_memory_barrier.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;

		pipeline_src_stage_flags = VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT;
		pipeline_dst_stage_flags = VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT;
	}
	else if ((old_layout == VK_IMAGE_LAYOUT_GENERAL) && (new_layout == VK_IMAGE_LAYOUT_UNDEFINED))
	{
		image_memory_barrier.srcAccessMask = 0;
		image_memory_barrier.dstAccessMask = 0;

		pipeline_src_stage_flags = VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT;
		pipeline_dst_stage_flags = VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT;
	}
	else if ((old_layout == VK_IMAGE_LAYOUT_UNDEFINED) && (new_layout == VK_IMAGE_LAYOUT_GENERAL))
	{
		image_memory_barrier.srcAccessMask = 0;
		image_memory_barrier.dstAccessMask = 0;

		pipeline_src_stage_flags = VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT;
		pipeline_dst_stage_flags = VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT;
	}
	else if ((old_layout == VK_IMAGE_LAYOUT_GENERAL) && (new_layout == VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL))
	{
		image_memory_barrier.srcAccessMask = 0;
		image_memory_barrier.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;

		pipeline_src_stage_flags = VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT;
		pipeline_dst_stage_flags = VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT;
	}
	else if ((old_layout == VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL) && (new_layout == VK_IMAGE_LAYOUT_GENERAL))
	{
		image_memory_barrier.srcAccessMask = VK_ACCESS_SHADER_READ_BIT;
		image_memory_barrier.dstAccessMask = 0;

		pipeline_src_stage_flags = VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT;
		pipeline_dst_stage_flags = VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT;
	}

	vkCmdPipelineBarrier(command_buffer, pipeline_src_stage_flags, pipeline_dst_stage_flags, 0, 0, 0, 0, 0, 1, &image_memory_barrier);

	TRACY_ZONE_END
}
void vulkan_image_map(image_t* image)
{
	TRACY_ZONE_BEGIN

	vkMapMemory(g_vulkan_instance_device, image->device_memory, 0, image->size, 0, &image->mapped_buffer);

	TRACY_ZONE_END
}
void vulkan_image_unmap(image_t* image)
{
	TRACY_ZONE_BEGIN

	vkUnmapMemory(g_vulkan_instance_device, image->device_memory);

	image->mapped_buffer = 0;

	TRACY_ZONE_END
}
void vulkan_image_free(image_t* image)
{
	TRACY_ZONE_BEGIN

	if (image->mapped_buffer)
	{
		vkUnmapMemory(g_vulkan_instance_device, image->device_memory);
	}

	vkFreeMemory(g_vulkan_instance_device, image->device_memory, 0);

	vkDestroySampler(g_vulkan_instance_device, image->sampler, 0);
	vkDestroyImageView(g_vulkan_instance_device, image->image_view, 0);
	vkDestroyImage(g_vulkan_instance_device, image->image, 0);

	TRACY_ZONE_END
}
VkFormat vulkan_image_find_depth_format(void)
{
	TRACY_ZONE_BEGIN

	VkFormat formats[] = { VK_FORMAT_D32_SFLOAT, VK_FORMAT_D32_SFLOAT_S8_UINT, VK_FORMAT_D24_UNORM_S8_UINT };

	VkFormat depth_format = vulkan_image_find_supported_format(formats, ARRAY_COUNT(formats), VK_IMAGE_TILING_OPTIMAL, VK_FORMAT_FEATURE_DEPTH_STENCIL_ATTACHMENT_BIT);

	TRACY_ZONE_END

	return depth_format;
}
VkFormat vulkan_image_find_supported_format(VkFormat* formats, uint64_t format_count, VkImageTiling tiling, VkFormatFeatureFlags features)
{
	TRACY_ZONE_BEGIN

	VkFormat format = VK_FORMAT_UNDEFINED;

	uint64_t format_index = 0;
	while (format_index < format_count)
	{
		VkFormatProperties format_properties;
		memset(&format_properties, 0, sizeof(VkFormatProperties));

		vkGetPhysicalDeviceFormatProperties(g_vulkan_instance_physical_device, formats[format_index], &format_properties);

		if (tiling == VK_IMAGE_TILING_LINEAR && (format_properties.linearTilingFeatures & features) == features)
		{
			format = formats[format_index];

			break;
		}
		else if (tiling == VK_IMAGE_TILING_OPTIMAL && (format_properties.optimalTilingFeatures & features) == features)
		{
			format = formats[format_index];

			break;
		}

		format_index++;
	}

	TRACY_ZONE_END

	return format;
}
VkImageView vulkan_image_view_alloc(VkImage image, VkImageViewType view_type, VkFormat format, VkImageAspectFlags aspect_flags)
{
	TRACY_ZONE_BEGIN

	VkImageView image_view;

	VkImageViewCreateInfo image_view_create_info;
	memset(&image_view_create_info, 0, sizeof(VkImageViewCreateInfo));

	image_view_create_info.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
	image_view_create_info.image = image;
	image_view_create_info.viewType = view_type;
	image_view_create_info.format = format;
	image_view_create_info.subresourceRange.aspectMask = aspect_flags;
	image_view_create_info.subresourceRange.baseMipLevel = 0;
	image_view_create_info.subresourceRange.levelCount = 1;
	image_view_create_info.subresourceRange.baseArrayLayer = 0;
	image_view_create_info.subresourceRange.layerCount = 1;

	vkCreateImageView(g_vulkan_instance_device, &image_view_create_info, 0, &image_view);

	TRACY_ZONE_END

	return image_view;
}
VkSampler vulkan_image_sampler_alloc(VkFilter filter)
{
	TRACY_ZONE_BEGIN

	VkSampler sampler;

	VkSamplerCreateInfo sampler_create_info;
	memset(&sampler_create_info, 0, sizeof(VkSamplerCreateInfo));

	sampler_create_info.sType = VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO;
	sampler_create_info.magFilter = filter;
	sampler_create_info.minFilter = filter;
	sampler_create_info.addressModeU = VK_SAMPLER_ADDRESS_MODE_REPEAT;
	sampler_create_info.addressModeV = VK_SAMPLER_ADDRESS_MODE_REPEAT;
	sampler_create_info.addressModeW = VK_SAMPLER_ADDRESS_MODE_REPEAT;
	sampler_create_info.anisotropyEnable = 1;
	sampler_create_info.maxAnisotropy = g_vulkan_instance_physical_device_properties.limits.maxSamplerAnisotropy;
	sampler_create_info.borderColor = VK_BORDER_COLOR_INT_OPAQUE_BLACK;
	sampler_create_info.unnormalizedCoordinates = 0;
	sampler_create_info.compareEnable = 0;
	sampler_create_info.compareOp = VK_COMPARE_OP_ALWAYS;
	sampler_create_info.mipmapMode = VK_SAMPLER_MIPMAP_MODE_LINEAR;
	sampler_create_info.mipLodBias = 0.0F;
	sampler_create_info.minLod = 0.0F;
	sampler_create_info.maxLod = 0.0F;

	vkCreateSampler(g_vulkan_instance_device, &sampler_create_info, 0, &sampler);

	TRACY_ZONE_END

	return sampler;
}
void vulkan_image_view_free(VkImageView image_view)
{
	TRACY_ZONE_BEGIN

	vkDestroyImageView(g_vulkan_instance_device, image_view, 0);

	TRACY_ZONE_END
}
void vulkan_image_sampler_free(VkSampler sampler)
{
	TRACY_ZONE_BEGIN

	vkDestroySampler(g_vulkan_instance_device, sampler, 0);

	TRACY_ZONE_END
}
