#include <string.h>

#include <vega/engine/vulkan/buffer.h>
#include <vega/engine/vulkan/instance.h>
#include <vega/engine/vulkan/vulkan.h>

#ifndef TRACY_ZONE_BEGIN
	#define TRACY_ZONE_BEGIN TracyCZoneC(ctx, TRACY_COLOR_GREEN, 1U);
#endif // TRACY_ZONE_BEGIN

#ifndef TRACY_ZONE_END
	#define TRACY_ZONE_END TracyCZoneEnd(ctx);
#endif // TRACY_ZONE_END

buffer_t vulkan_buffer_alloc(uint64_t size, VkBufferUsageFlags usage, VkMemoryPropertyFlags memory_properties)
{
	TRACY_ZONE_BEGIN

	buffer_t buffer;
	memset(&buffer, 0, sizeof(buffer_t));

	buffer.size = size;

	VkBufferCreateInfo buffer_create_info;
	memset(&buffer_create_info, 0, sizeof(VkBufferCreateInfo));

	buffer_create_info.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
	buffer_create_info.size = size;
	buffer_create_info.usage = usage;
	buffer_create_info.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

	vkCreateBuffer(g_vulkan_device, &buffer_create_info, 0, &buffer.buffer);

	VkMemoryRequirements memory_requirements;
	vkGetBufferMemoryRequirements(g_vulkan_device, buffer.buffer, &memory_requirements);

	VkMemoryAllocateInfo memory_allocate_info;
	memset(&memory_allocate_info, 0, sizeof(VkMemoryAllocateInfo));

	memory_allocate_info.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
	memory_allocate_info.allocationSize = memory_requirements.size;
	memory_allocate_info.memoryTypeIndex = vulkan_find_memory_type(memory_requirements.memoryTypeBits, memory_properties);

	vkAllocateMemory(g_vulkan_device, &memory_allocate_info, 0, &buffer.device_memory);
	vkBindBufferMemory(g_vulkan_device, buffer.buffer, buffer.device_memory, 0);

	TRACY_ZONE_END

	return buffer;
}
void vulkan_buffer_copy_to_buffer(buffer_t* buffer, buffer_t* target, VkCommandBuffer command_buffer)
{
	TRACY_ZONE_BEGIN

	VkBufferCopy buffer_copy;
	memset(&buffer_copy, 0, sizeof(VkBufferCopy));

	buffer_copy.size = buffer->size;

	vkCmdCopyBuffer(command_buffer, buffer->buffer, target->buffer, 1, &buffer_copy);

	TRACY_ZONE_END
}
void vulkan_buffer_copy_to_image(buffer_t* buffer, image_t* target, VkCommandBuffer command_buffer, uint32_t width, uint32_t height, uint32_t depth, VkImageAspectFlags aspect_flags)
{
	TRACY_ZONE_BEGIN

	VkBufferImageCopy buffer_image_copy;
	memset(&buffer_image_copy, 0, sizeof(VkBufferImageCopy));

	buffer_image_copy.bufferOffset = 0;
	buffer_image_copy.bufferRowLength = 0;
	buffer_image_copy.bufferImageHeight = 0;
	buffer_image_copy.imageSubresource.aspectMask = aspect_flags;
	buffer_image_copy.imageSubresource.mipLevel = 0;
	buffer_image_copy.imageSubresource.baseArrayLayer = 0;
	buffer_image_copy.imageSubresource.layerCount = 1;
	buffer_image_copy.imageOffset.x = 0;
	buffer_image_copy.imageOffset.y = 0;
	buffer_image_copy.imageOffset.z = 0;
	buffer_image_copy.imageExtent.width = width;
	buffer_image_copy.imageExtent.height = height;
	buffer_image_copy.imageExtent.depth = depth;

	vkCmdCopyBufferToImage(command_buffer, buffer->buffer, target->image, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, 1, &buffer_image_copy);

	TRACY_ZONE_END
}
void vulkan_buffer_map(buffer_t* buffer)
{
	TRACY_ZONE_BEGIN

	vkMapMemory(g_vulkan_device, buffer->device_memory, 0, buffer->size, 0, &buffer->mapped_buffer);

	TRACY_ZONE_END
}
void vulkan_buffer_unmap(buffer_t* buffer)
{
	TRACY_ZONE_BEGIN

	vkUnmapMemory(g_vulkan_device, buffer->device_memory);

	buffer->mapped_buffer = 0;

	TRACY_ZONE_END
}
void vulkan_buffer_free(buffer_t* buffer)
{
	TRACY_ZONE_BEGIN

	if (buffer->mapped_buffer)
	{
		vkUnmapMemory(g_vulkan_device, buffer->mapped_buffer);
	}

	vkFreeMemory(g_vulkan_device, buffer->device_memory, 0);

	vkDestroyBuffer(g_vulkan_device, buffer->buffer, 0);

	TRACY_ZONE_END
}
