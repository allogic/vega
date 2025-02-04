#include <string.h>

#include <vega/engine/vulkan/buffer.h>
#include <vega/engine/vulkan/command_buffer.h>
#include <vega/engine/vulkan/device.h>
#include <vega/engine/vulkan/vulkan.h>

#ifndef TRACY_ZONE_BEGIN
	#define TRACY_ZONE_BEGIN TracyCZoneC(ctx, TRACY_COLOR_GREEN, 1U);
#endif // TRACY_ZONE_BEGIN

#ifndef TRACY_ZONE_END
	#define TRACY_ZONE_END TracyCZoneEnd(ctx);
#endif // TRACY_ZONE_END

buffer_t vulkan_buffer_vertex_alloc(void* buffer, uint64_t size)
{
	TRACY_ZONE_BEGIN

	buffer_t staging_buffer = vulkan_buffer_alloc(size, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);

	vulkan_buffer_map(&staging_buffer);
	memcpy(staging_buffer.mapped_buffer, buffer, staging_buffer.size);
	vulkan_buffer_unmap(&staging_buffer);

	buffer_t target_buffer = vulkan_buffer_alloc(size, VK_BUFFER_USAGE_VERTEX_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);

	VkCommandBuffer command_buffer = vulkan_command_buffer_begin();

	vulkan_buffer_copy_to_buffer(&staging_buffer, &target_buffer, command_buffer);

	vulkan_command_buffer_end(command_buffer);

	vulkan_buffer_free(&staging_buffer);

	TRACY_ZONE_END

	return target_buffer;
}
buffer_t vulkan_buffer_index_alloc(void* buffer, uint64_t size)
{
	TRACY_ZONE_BEGIN

	buffer_t staging_buffer = vulkan_buffer_alloc(size, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);

	vulkan_buffer_map(&staging_buffer);
	memcpy(staging_buffer.mapped_buffer, buffer, staging_buffer.size);
	vulkan_buffer_unmap(&staging_buffer);

	buffer_t target_buffer = vulkan_buffer_alloc(size, VK_BUFFER_USAGE_INDEX_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);

	VkCommandBuffer command_buffer = vulkan_command_buffer_begin();

	vulkan_buffer_copy_to_buffer(&staging_buffer, &target_buffer, command_buffer);

	vulkan_command_buffer_end(command_buffer);

	vulkan_buffer_free(&staging_buffer);

	TRACY_ZONE_END

	return target_buffer;
}
buffer_t vulkan_buffer_uniform_alloc(void* buffer, uint64_t size)
{
	TRACY_ZONE_BEGIN

	buffer_t staging_buffer = vulkan_buffer_alloc(size, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);

	vulkan_buffer_map(&staging_buffer);
	memcpy(staging_buffer.mapped_buffer, buffer, staging_buffer.size);
	vulkan_buffer_unmap(&staging_buffer);

	buffer_t target_buffer = vulkan_buffer_alloc(size, VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);

	VkCommandBuffer command_buffer = vulkan_command_buffer_begin();

	vulkan_buffer_copy_to_buffer(&staging_buffer, &target_buffer, command_buffer);

	vulkan_command_buffer_end(command_buffer);

	vulkan_buffer_free(&staging_buffer);

	TRACY_ZONE_END

	return target_buffer;
}
buffer_t vulkan_buffer_storage_alloc(void* buffer, uint64_t size)
{
	TRACY_ZONE_BEGIN

	buffer_t staging_buffer = vulkan_buffer_alloc(size, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);

	vulkan_buffer_map(&staging_buffer);
	memcpy(staging_buffer.mapped_buffer, buffer, staging_buffer.size);
	vulkan_buffer_unmap(&staging_buffer);

	buffer_t target_buffer = vulkan_buffer_alloc(size, VK_BUFFER_USAGE_STORAGE_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);

	VkCommandBuffer command_buffer = vulkan_command_buffer_begin();

	vulkan_buffer_copy_to_buffer(&staging_buffer, &target_buffer, command_buffer);

	vulkan_command_buffer_end(command_buffer);

	vulkan_buffer_free(&staging_buffer);

	TRACY_ZONE_END

	return target_buffer;
}
buffer_t vulkan_buffer_vertex_coherent_alloc(uint64_t size)
{
	TRACY_ZONE_BEGIN

	buffer_t target_buffer = vulkan_buffer_alloc(size, VK_BUFFER_USAGE_VERTEX_BUFFER_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);

	vulkan_buffer_map(&target_buffer);

	TRACY_ZONE_END

	return target_buffer;
}
buffer_t vulkan_buffer_index_coherent_alloc(uint64_t size)
{
	TRACY_ZONE_BEGIN

	buffer_t target_buffer = vulkan_buffer_alloc(size, VK_BUFFER_USAGE_INDEX_BUFFER_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);

	vulkan_buffer_map(&target_buffer);

	TRACY_ZONE_END

	return target_buffer;
}
buffer_t vulkan_buffer_uniform_coherent_alloc(uint64_t size)
{
	TRACY_ZONE_BEGIN

	buffer_t target_buffer = vulkan_buffer_alloc(size, VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);

	vulkan_buffer_map(&target_buffer);

	TRACY_ZONE_END

	return target_buffer;
}
buffer_t vulkan_buffer_storage_coherent_alloc(uint64_t size)
{
	TRACY_ZONE_BEGIN

	buffer_t target_buffer = vulkan_buffer_alloc(size, VK_BUFFER_USAGE_STORAGE_BUFFER_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);

	vulkan_buffer_map(&target_buffer);

	TRACY_ZONE_END

	return target_buffer;
}
buffer_t vulkan_buffer_alloc(uint64_t size, VkBufferUsageFlags buffer_usage, VkMemoryPropertyFlags memory_properties)
{
	TRACY_ZONE_BEGIN

	buffer_t buffer = { 0 };
	buffer.size = size;
	buffer.buffer_usage = buffer_usage;
	buffer.memory_properties = memory_properties;

	VkBufferCreateInfo buffer_create_info = { 0 };
	buffer_create_info.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
	buffer_create_info.size = size;
	buffer_create_info.usage = buffer_usage;
	buffer_create_info.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

	vkCreateBuffer(g_vulkan_device, &buffer_create_info, 0, &buffer.buffer);

	VkMemoryRequirements memory_requirements = { 0 };

	vkGetBufferMemoryRequirements(g_vulkan_device, buffer.buffer, &memory_requirements);

	VkMemoryAllocateInfo memory_allocate_info = { 0 };
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

	VkBufferCopy buffer_copy = { 0 };
	buffer_copy.size = buffer->size;

	vkCmdCopyBuffer(command_buffer, buffer->buffer, target->buffer, 1, &buffer_copy);

	TRACY_ZONE_END
}
void vulkan_buffer_copy_to_image(buffer_t* buffer, image_t* target, VkCommandBuffer command_buffer, uint32_t width, uint32_t height, uint32_t depth, VkImageAspectFlags aspect_flags)
{
	TRACY_ZONE_BEGIN

	VkBufferImageCopy buffer_image_copy = { 0 };
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
void vulkan_buffer_resize(buffer_t* buffer, uint64_t size)
{
	TRACY_ZONE_BEGIN

	uint64_t old_size = buffer->size;
	VkBufferUsageFlags old_buffer_usage = buffer->buffer_usage;
	VkMemoryPropertyFlags old_memory_properties = buffer->memory_properties;
	VkBuffer old_buffer = buffer->buffer;
	VkDeviceMemory old_device_memory = buffer->device_memory;
	void* old_mapped_buffer = buffer->mapped_buffer;

	uint64_t new_size = size;
	VkBufferUsageFlags new_buffer_usage = buffer->buffer_usage;
	VkMemoryPropertyFlags new_memory_properties = buffer->memory_properties;
	VkBuffer new_buffer = 0;
	VkDeviceMemory new_device_memory = 0;
	void* new_mapped_buffer = 0;

	VkBufferCreateInfo buffer_create_info = { 0 };
	buffer_create_info.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
	buffer_create_info.size = size;
	buffer_create_info.usage = new_buffer_usage;
	buffer_create_info.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

	vkCreateBuffer(g_vulkan_device, &buffer_create_info, 0, &new_buffer);

	VkMemoryRequirements memory_requirements = { 0 };

	vkGetBufferMemoryRequirements(g_vulkan_device, new_buffer, &memory_requirements);

	VkMemoryAllocateInfo memory_allocate_info = { 0 };
	memory_allocate_info.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
	memory_allocate_info.allocationSize = memory_requirements.size;
	memory_allocate_info.memoryTypeIndex = vulkan_find_memory_type(memory_requirements.memoryTypeBits, new_memory_properties);

	vkAllocateMemory(g_vulkan_device, &memory_allocate_info, 0, &new_device_memory);
	vkBindBufferMemory(g_vulkan_device, new_buffer, new_device_memory, 0);

	if (old_mapped_buffer == 0)
	{
		vkMapMemory(g_vulkan_device, old_device_memory, 0, old_size, 0, &old_mapped_buffer);
	}

	if (new_mapped_buffer == 0)
	{
		vkMapMemory(g_vulkan_device, new_device_memory, 0, new_size, 0, &new_mapped_buffer);
	}

	memcpy(new_mapped_buffer, old_mapped_buffer, VEGA_MIN(old_size, new_size));

	if (old_mapped_buffer)
	{
		vkUnmapMemory(g_vulkan_device, old_device_memory);

		old_mapped_buffer = 0;
	}

	vkFreeMemory(g_vulkan_device, old_device_memory, 0);

	vkDestroyBuffer(g_vulkan_device, old_buffer, 0);

	if (VEGA_IS_BIT_SET(new_memory_properties, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT))
	{
		vkUnmapMemory(g_vulkan_device, new_device_memory);

		new_mapped_buffer = 0;
	}

	buffer->size = new_size;
	buffer->buffer_usage = new_buffer_usage;
	buffer->memory_properties = new_memory_properties;
	buffer->buffer = new_buffer;
	buffer->device_memory = new_device_memory;
	buffer->mapped_buffer = new_mapped_buffer;

	TRACY_ZONE_END
}
void vulkan_buffer_free(buffer_t* buffer)
{
	TRACY_ZONE_BEGIN

	if (buffer->mapped_buffer)
	{
		vkUnmapMemory(g_vulkan_device, buffer->device_memory);
	}

	vkFreeMemory(g_vulkan_device, buffer->device_memory, 0);

	vkDestroyBuffer(g_vulkan_device, buffer->buffer, 0);

	TRACY_ZONE_END
}
