#include <vega/engine/vulkan/command_pool.h>
#include <vega/engine/vulkan/device.h>

#ifndef TRACY_ZONE_BEGIN
	#define TRACY_ZONE_BEGIN TracyCZoneC(ctx, TRACY_COLOR_GREEN, 1U);
#endif // TRACY_ZONE_BEGIN

#ifndef TRACY_ZONE_END
	#define TRACY_ZONE_END TracyCZoneEnd(ctx);
#endif // TRACY_ZONE_END

VkCommandPool g_vulkan_command_pool = 0;

void vulkan_command_pool_alloc(void)
{
	TRACY_ZONE_BEGIN

	VkCommandPoolCreateInfo command_pool_create_info = { 0 };
	command_pool_create_info.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
	command_pool_create_info.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;
	command_pool_create_info.queueFamilyIndex = g_vulkan_device_graphic_queue_index;

	vkCreateCommandPool(g_vulkan_device, &command_pool_create_info, 0, &g_vulkan_command_pool);

	TRACY_ZONE_END
}
void vulkan_command_pool_free(void)
{
	TRACY_ZONE_BEGIN

	vkDestroyCommandPool(g_vulkan_device, g_vulkan_command_pool, 0);

	TRACY_ZONE_END
}
