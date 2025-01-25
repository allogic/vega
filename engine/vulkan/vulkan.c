#include <vega/engine/vulkan/instance.h>
#include <vega/engine/vulkan/vulkan.h>

#ifndef TRACY_ZONE_BEGIN
	#define TRACY_ZONE_BEGIN TracyCZoneC(ctx, TRACY_COLOR_BLUE, 1U);
#endif // TRACY_ZONE_BEGIN

#ifndef TRACY_ZONE_END
	#define TRACY_ZONE_END TracyCZoneEnd(ctx);
#endif // TRACY_ZONE_END

int32_t vulkan_find_memory_type(uint32_t type_filter, VkMemoryPropertyFlags memory_property_flags)
{
	TRACY_ZONE_BEGIN

	int32_t memory_type = -1;

	VkPhysicalDeviceMemoryProperties physical_device_memory_properties;
	vkGetPhysicalDeviceMemoryProperties(g_vulkan_physical_device, &physical_device_memory_properties);

	uint32_t memory_type_index = 0;
	while (memory_type_index < physical_device_memory_properties.memoryTypeCount)
	{
		if ((type_filter & (1 << memory_type_index)) && ((physical_device_memory_properties.memoryTypes[memory_type_index].propertyFlags & memory_property_flags) == memory_property_flags))
		{
			memory_type = (int32_t)memory_type_index;

			break;
		}
	}

	TRACY_ZONE_END

	return memory_type;
}
