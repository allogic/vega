#include <stdio.h>
#include <string.h>

#include <vega/engine/vulkan/device.h>
#include <vega/engine/vulkan/instance.h>
#include <vega/engine/vulkan/surface.h>

#ifndef TRACY_ZONE_BEGIN
	#define TRACY_ZONE_BEGIN TracyCZoneC(ctx, TRACY_COLOR_GREEN, 1U);
#endif // TRACY_ZONE_BEGIN

#ifndef TRACY_ZONE_END
	#define TRACY_ZONE_END TracyCZoneEnd(ctx);
#endif // TRACY_ZONE_END

VkPhysicalDevice g_vulkan_device_physical = 0;
VkPhysicalDeviceProperties g_vulkan_device_physical_properties = { 0 };
VkPhysicalDeviceFeatures g_vulkan_device_physical_features = { 0 };

VkDevice g_vulkan_device = 0;

int32_t g_vulkan_device_graphic_queue_index = -1;
int32_t g_vulkan_device_present_queue_index = -1;

VkQueue g_vulkan_device_graphic_queue = 0;
VkQueue g_vulkan_device_compute_queue = 0;
VkQueue g_vulkan_device_present_queue = 0;

static char const* s_vulkan_device_extensions[] =
{
	"VK_KHR_swapchain",
	"VK_EXT_descriptor_indexing",
};

void vulkan_device_alloc(void)
{
	TRACY_ZONE_BEGIN

	float queue_priority = 1.0F;

	vector_t device_queue_create_infos = std_vector_alloc(sizeof(VkDeviceQueueCreateInfo));

	{
		VkDeviceQueueCreateInfo device_queue_create_info = { 0 };
		device_queue_create_info.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
		device_queue_create_info.queueFamilyIndex = g_vulkan_device_graphic_queue_index;
		device_queue_create_info.queueCount = 1;
		device_queue_create_info.pQueuePriorities = &queue_priority;

		std_vector_push(&device_queue_create_infos, &device_queue_create_info);
	}

	{
		VkDeviceQueueCreateInfo device_queue_create_info = { 0 };
		device_queue_create_info.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
		device_queue_create_info.queueFamilyIndex = g_vulkan_device_present_queue_index;
		device_queue_create_info.queueCount = 1;
		device_queue_create_info.pQueuePriorities = &queue_priority;

		std_vector_push(&device_queue_create_infos, &device_queue_create_info);
	}

	VkPhysicalDeviceDescriptorIndexingFeatures physical_device_descriptor_indexing_features = { 0 };
	physical_device_descriptor_indexing_features.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_DESCRIPTOR_INDEXING_FEATURES;
	physical_device_descriptor_indexing_features.pNext = 0;

	VkPhysicalDeviceFeatures2 physical_device_features_2 = { 0 };
	physical_device_features_2.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_FEATURES_2;
	physical_device_features_2.pNext = &physical_device_descriptor_indexing_features;

	vkGetPhysicalDeviceFeatures2(g_vulkan_device_physical, &physical_device_features_2);

	physical_device_features_2.features.samplerAnisotropy = 1;
	physical_device_features_2.features.shaderFloat64 = 1;

	VkDeviceCreateInfo device_create_info = { 0 };
	device_create_info.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
	device_create_info.pQueueCreateInfos = std_vector_buffer(&device_queue_create_infos);
	device_create_info.queueCreateInfoCount = (uint32_t)std_vector_count(&device_queue_create_infos);
	device_create_info.pEnabledFeatures = 0;
	device_create_info.pNext = &physical_device_features_2;
	device_create_info.ppEnabledExtensionNames = s_vulkan_device_extensions;
	device_create_info.enabledExtensionCount = (uint32_t)VEGA_ARRAY_COUNT(s_vulkan_device_extensions);

#ifdef VEGA_DEBUG
	device_create_info.ppEnabledLayerNames = g_vulkan_instance_validation_layers;
	device_create_info.enabledLayerCount = (uint32_t)VEGA_ARRAY_COUNT(g_vulkan_instance_validation_layers);
#endif

	vkCreateDevice(g_vulkan_device_physical, &device_create_info, 0, &g_vulkan_device);

	vkGetDeviceQueue(g_vulkan_device, g_vulkan_device_graphic_queue_index, 0, &g_vulkan_device_graphic_queue);
	vkGetDeviceQueue(g_vulkan_device, g_vulkan_device_graphic_queue_index, 0, &g_vulkan_device_compute_queue);
	vkGetDeviceQueue(g_vulkan_device, g_vulkan_device_present_queue_index, 0, &g_vulkan_device_present_queue);

	std_vector_free(&device_queue_create_infos);

	TRACY_ZONE_END
}
void vulkan_device_find_physical(void)
{
	TRACY_ZONE_BEGIN

	uint32_t physical_device_count = 0;
	vkEnumeratePhysicalDevices(g_vulkan_instance, &physical_device_count, 0);

	vector_t physical_devices = std_vector_alloc(sizeof(VkPhysicalDevice));
	std_vector_resize(&physical_devices, physical_device_count);

	vkEnumeratePhysicalDevices(g_vulkan_instance, &physical_device_count, std_vector_buffer(&physical_devices));

	uint64_t physical_device_index = 0;
	while (physical_device_index < physical_device_count)
	{
		VkPhysicalDevice physical_device = *(VkPhysicalDevice*)std_vector_get(&physical_devices, physical_device_index);

		vkGetPhysicalDeviceProperties(physical_device, &g_vulkan_device_physical_properties);
		vkGetPhysicalDeviceFeatures(physical_device, &g_vulkan_device_physical_features);

		if (g_vulkan_device_physical_properties.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU)
		{
			if (g_vulkan_device_physical_features.geometryShader &&
				g_vulkan_device_physical_features.samplerAnisotropy &&
				g_vulkan_device_physical_features.shaderFloat64)
			{
				g_vulkan_device_physical = physical_device;

				break;
			}
		}

		physical_device_index++;
	}

	std_vector_free(&physical_devices);

	TRACY_ZONE_END
}
void vulkan_device_find_physical_queue_families(void)
{
	TRACY_ZONE_BEGIN

	uint32_t queue_family_property_count = 0;
	vkGetPhysicalDeviceQueueFamilyProperties(g_vulkan_device_physical, &queue_family_property_count, 0);

	vector_t queue_family_properties = std_vector_alloc(sizeof(VkQueueFamilyProperties));
	std_vector_resize(&queue_family_properties, queue_family_property_count);

	vkGetPhysicalDeviceQueueFamilyProperties(g_vulkan_device_physical, &queue_family_property_count, std_vector_buffer(&queue_family_properties));

	uint64_t physical_device_queue_family_property_index = 0;
	while (physical_device_queue_family_property_index < queue_family_property_count)
	{
		VkQueueFamilyProperties properties = *(VkQueueFamilyProperties*)std_vector_get(&queue_family_properties, physical_device_queue_family_property_index);

		uint32_t graphic_support = 0;
		uint32_t compute_support = 0;
		uint32_t present_support = 0;

		graphic_support = properties.queueFlags & VK_QUEUE_GRAPHICS_BIT;
		compute_support = properties.queueFlags & VK_QUEUE_COMPUTE_BIT;

		vkGetPhysicalDeviceSurfaceSupportKHR(g_vulkan_device_physical, (uint32_t)physical_device_queue_family_property_index, g_vulkan_surface, &present_support);

		if (graphic_support && compute_support && (g_vulkan_device_graphic_queue_index == -1))
		{
			g_vulkan_device_graphic_queue_index = (uint32_t)physical_device_queue_family_property_index;
		}
		else if (present_support && (g_vulkan_device_present_queue_index == -1))
		{
			g_vulkan_device_present_queue_index = (uint32_t)physical_device_queue_family_property_index;
		}

		if ((g_vulkan_device_graphic_queue_index != -1) && (g_vulkan_device_present_queue_index != -1))
		{
			break;
		}

		physical_device_queue_family_property_index++;
	}

	std_vector_free(&queue_family_properties);

	printf("Queue Indices\n");
	printf("\tGraphicQueueIndex %d\n", g_vulkan_device_graphic_queue_index);
	printf("\tPresentQueueIndex %d\n", g_vulkan_device_present_queue_index);
	printf("\n");

	TRACY_ZONE_END
}
void vulkan_device_check_physical_extensions(void)
{
	TRACY_ZONE_BEGIN

	uint32_t available_device_extension_count = 0;
	vkEnumerateDeviceExtensionProperties(g_vulkan_device_physical, 0, &available_device_extension_count, 0);

	vector_t available_device_extensions = std_vector_alloc(sizeof(VkExtensionProperties));
	std_vector_resize(&available_device_extensions, available_device_extension_count);

	vkEnumerateDeviceExtensionProperties(g_vulkan_device_physical, 0, &available_device_extension_count, std_vector_buffer(&available_device_extensions));

	printf("Device Extensions\n");

	uint64_t device_extension_index = 0;
	uint64_t device_extension_count = VEGA_ARRAY_COUNT(s_vulkan_device_extensions);
	while (device_extension_index < device_extension_count)
	{
		uint8_t device_extensions_available = 0;

		uint64_t available_device_extension_index = 0;
		while (available_device_extension_index < available_device_extension_count)
		{
			VkExtensionProperties properties = *(VkExtensionProperties*)std_vector_get(&available_device_extensions, available_device_extension_index);

			if (strcmp(s_vulkan_device_extensions[device_extension_index], properties.extensionName) == 0)
			{
				printf("\tFound %s\n", s_vulkan_device_extensions[device_extension_index]);

				device_extensions_available = 1;

				break;
			}

			available_device_extension_index++;
		}

		if (device_extensions_available == 0)
		{
			printf("\tMissing %s\n", s_vulkan_device_extensions[device_extension_index]);

			break;
		}

		device_extension_index++;
	}

	printf("\n");

	std_vector_free(&available_device_extensions);

	TRACY_ZONE_END
}
void vulkan_device_free(void)
{
	TRACY_ZONE_BEGIN

	vkDestroyDevice(g_vulkan_device, 0);

	TRACY_ZONE_END
}
