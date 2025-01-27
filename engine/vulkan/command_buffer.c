#include <string.h>

#include <vega/engine/vulkan/command_buffer.h>
#include <vega/engine/vulkan/instance.h>

#ifndef TRACY_ZONE_BEGIN
	#define TRACY_ZONE_BEGIN TracyCZoneC(ctx, TRACY_COLOR_GREEN, 1U);
#endif // TRACY_ZONE_BEGIN

#ifndef TRACY_ZONE_END
	#define TRACY_ZONE_END TracyCZoneEnd(ctx);
#endif // TRACY_ZONE_END

VkCommandBuffer vulkan_command_buffer_begin(void)
{
	TRACY_ZONE_BEGIN

	VkCommandBuffer command_buffer;

	VkCommandBufferAllocateInfo command_buffer_allocate_info;
	memset(&command_buffer_allocate_info, 0, sizeof(VkCommandBufferAllocateInfo));

	command_buffer_allocate_info.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
	command_buffer_allocate_info.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
	command_buffer_allocate_info.commandPool = g_vulkan_command_pool;;
	command_buffer_allocate_info.commandBufferCount = 1;

	vkAllocateCommandBuffers(g_vulkan_device, &command_buffer_allocate_info, &command_buffer);

	VkCommandBufferBeginInfo command_buffer_begin_info;
	memset(&command_buffer_begin_info, 0, sizeof(VkCommandBufferBeginInfo));

	command_buffer_begin_info.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
	command_buffer_begin_info.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;

	vkBeginCommandBuffer(command_buffer, &command_buffer_begin_info);

	TRACY_ZONE_END

	return command_buffer;
}
void vulkan_command_buffer_end(VkCommandBuffer command_buffer)
{
	TRACY_ZONE_BEGIN

	vkEndCommandBuffer(command_buffer);

	VkSubmitInfo submit_info;
	memset(&submit_info, 0, sizeof(VkSubmitInfo));

	submit_info.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
	submit_info.commandBufferCount = 1;
	submit_info.pCommandBuffers = &command_buffer;

	// TODO: proper synchronization..
	vkQueueSubmit(g_vulkan_graphic_queue, 1, &submit_info, 0);
	vkQueueWaitIdle(g_vulkan_graphic_queue);

	vkFreeCommandBuffers(g_vulkan_device, g_vulkan_command_pool, 1, &command_buffer);

	TRACY_ZONE_END
}
