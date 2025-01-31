#include <stdio.h>
#include <string.h>

#define WIN32_LEAN_AND_MEAN
#define NO_GUID_DEFS
#include <windows.h>

#define VK_USE_PLATFORM_WIN32_KHR
#include <vulkan/vulkan.h>

#include <vega/engine/platform/window.h>

#include <vega/engine/vulkan/instance.h>

#include <vega/engine/platform/window.h>

#ifndef TRACY_ZONE_BEGIN
	#define TRACY_ZONE_BEGIN TracyCZoneC(ctx, TRACY_COLOR_GREEN, 1U);
#endif // TRACY_ZONE_BEGIN

#ifndef TRACY_ZONE_END
	#define TRACY_ZONE_END TracyCZoneEnd(ctx);
#endif // TRACY_ZONE_END

#ifdef VEGA_DEBUG
static VkBool32 vulkan_instance_message_proc(VkDebugUtilsMessageSeverityFlagBitsEXT message_severity, VkDebugUtilsMessageTypeFlagsEXT message_type, VkDebugUtilsMessengerCallbackDataEXT const* callback_data, void* user_data);
#endif // VEGA_DEBUG

VkInstance g_vulkan_instance = 0;

VkSurfaceKHR g_vulkan_instance_surface = 0;
VkSurfaceCapabilitiesKHR g_vulkan_instance_surface_capabilities = { 0 };

VkPhysicalDevice g_vulkan_instance_physical_device = 0;
VkPhysicalDeviceProperties g_vulkan_instance_physical_device_properties = { 0 };
VkPhysicalDeviceFeatures g_vulkan_instance_physical_device_features = { 0 };

int32_t g_vulkan_instance_graphic_and_compute_queue_index = -1;
int32_t g_vulkan_instance_present_queue_index = -1;

VkDevice g_vulkan_instance_device = 0;

VkQueue g_vulkan_instance_graphic_queue = 0;
VkQueue g_vulkan_instance_compute_queue = 0;
VkQueue g_vulkan_instance_present_queue = 0;

VkSurfaceFormatKHR g_vulkan_instance_prefered_surface_format = { 0 };
VkPresentModeKHR g_vulkan_instance_prefered_present_mode = { 0 };

VkCommandPool g_vulkan_instance_command_pool = 0;

#ifdef VEGA_DEBUG
static PFN_vkCreateDebugUtilsMessengerEXT s_vulkan_instance_create_debug_utils_messenger_ext = 0;
static PFN_vkDestroyDebugUtilsMessengerEXT s_destroy_instance_debug_utils_messenger_ext = 0;

static VkDebugUtilsMessengerEXT s_vulkan_instance_debug_messenger = 0;
#endif VEGA_DEBUG

static char const* s_vulkan_instance_layer_extensions[] =
{
	"VK_KHR_surface",
	"VK_KHR_win32_surface",
#ifdef VEGA_DEBUG
	"VK_EXT_debug_utils",
#endif // VEGA_DEBUG
};

static char const* s_vulkan_instance_device_extensions[] =
{
	"VK_KHR_swapchain",
	"VK_EXT_descriptor_indexing",
};

#ifdef VEGA_DEBUG
static char const* s_vulkan_instance_validation_layers[] =
{
	"VK_LAYER_KHRONOS_validation",
};
#endif // VEGA_DEBUG

void vulkan_instance_alloc(void)
{
	TRACY_ZONE_BEGIN

	VkApplicationInfo app_info;
	memset(&app_info, 0, sizeof(VkApplicationInfo));

	app_info.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
	app_info.pApplicationName = "";
	app_info.applicationVersion = VK_MAKE_API_VERSION(0, 1, 0, 0);
	app_info.pEngineName = "";
	app_info.engineVersion = VK_MAKE_API_VERSION(0, 1, 0, 0);
	app_info.apiVersion = VK_API_VERSION_1_0;

	VkInstanceCreateInfo instance_create_info;
	memset(&instance_create_info, 0, sizeof(VkInstanceCreateInfo));

	instance_create_info.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
	instance_create_info.pApplicationInfo = &app_info;
	instance_create_info.enabledExtensionCount = ARRAY_COUNT(s_vulkan_instance_layer_extensions);
	instance_create_info.ppEnabledExtensionNames = s_vulkan_instance_layer_extensions;

#ifdef VEGA_DEBUG
	VkDebugUtilsMessengerCreateInfoEXT debug_create_info;
	memset(&debug_create_info, 0, sizeof(VkDebugUtilsMessengerCreateInfoEXT));

	debug_create_info.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
	debug_create_info.messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
	debug_create_info.messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
	debug_create_info.pfnUserCallback = vulkan_instance_message_proc;

	instance_create_info.pNext = (VkDebugUtilsMessengerCreateInfoEXT*)&debug_create_info;
	instance_create_info.enabledLayerCount = ARRAY_COUNT(s_vulkan_instance_validation_layers);
	instance_create_info.ppEnabledLayerNames = s_vulkan_instance_validation_layers;
#endif // VEGA_DEBUG

	vkCreateInstance(&instance_create_info, 0, &g_vulkan_instance);

#ifdef VEGA_DEBUG
	s_vulkan_instance_create_debug_utils_messenger_ext = (PFN_vkCreateDebugUtilsMessengerEXT)vkGetInstanceProcAddr(g_vulkan_instance, "vkCreateDebugUtilsMessengerEXT");
	s_destroy_instance_debug_utils_messenger_ext = (PFN_vkDestroyDebugUtilsMessengerEXT)vkGetInstanceProcAddr(g_vulkan_instance, "vkDestroyDebugUtilsMessengerEXT");

	s_vulkan_instance_create_debug_utils_messenger_ext(g_vulkan_instance, &debug_create_info, 0, &s_vulkan_instance_debug_messenger);
#endif // VEGA_DEBUG

	vulkan_instance_surface_alloc();

	vulkan_instance_find_physical_device();
	vulkan_instance_find_physical_device_queue_families();

	vulkan_instance_check_physical_device_extensions();

	vulkan_instance_device_alloc();

	vulkan_instance_check_surface_capabilities();

	vulkan_instance_command_pool_alloc();

	TRACY_ZONE_END
}
void vulkan_instance_free(void)
{
	TRACY_ZONE_BEGIN

	vulkan_instance_command_pool_free();
	vulkan_instance_device_free();
	vulkan_instance_surface_free();

#ifdef VEGA_DEBUG
	s_destroy_instance_debug_utils_messenger_ext(g_vulkan_instance, s_vulkan_instance_debug_messenger, 0);
#endif

	vkDestroyInstance(g_vulkan_instance, 0);

	TRACY_ZONE_END
}
void vulkan_instance_surface_alloc()
{
	TRACY_ZONE_BEGIN

	VkWin32SurfaceCreateInfoKHR surface_create_info;
	memset(&surface_create_info, 0, sizeof(VkWin32SurfaceCreateInfoKHR));

	surface_create_info.sType = VK_STRUCTURE_TYPE_WIN32_SURFACE_CREATE_INFO_KHR;
	surface_create_info.hwnd = g_platform_window_handle;
	surface_create_info.hinstance = g_platform_window_module_handle;

	vkCreateWin32SurfaceKHR(g_vulkan_instance, &surface_create_info, 0, &g_vulkan_instance_surface);

	TRACY_ZONE_END
}
void vulkan_instance_device_alloc(void)
{
	TRACY_ZONE_BEGIN

	float queue_priority = 1.0F;

	vector_t device_queue_create_infos = std_vector_alloc(sizeof(VkDeviceQueueCreateInfo));

	{
		VkDeviceQueueCreateInfo device_queue_create_info;
		memset(&device_queue_create_info, 0, sizeof(VkDeviceQueueCreateInfo));

		device_queue_create_info.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
		device_queue_create_info.queueFamilyIndex = g_vulkan_instance_graphic_and_compute_queue_index;
		device_queue_create_info.queueCount = 1;
		device_queue_create_info.pQueuePriorities = &queue_priority;

		std_vector_push(&device_queue_create_infos, &device_queue_create_info);
	}

	{
		VkDeviceQueueCreateInfo device_queue_create_info;
		memset(&device_queue_create_info, 0, sizeof(VkDeviceQueueCreateInfo));

		device_queue_create_info.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
		device_queue_create_info.queueFamilyIndex = g_vulkan_instance_present_queue_index;
		device_queue_create_info.queueCount = 1;
		device_queue_create_info.pQueuePriorities = &queue_priority;

		std_vector_push(&device_queue_create_infos, &device_queue_create_info);
	}

	VkPhysicalDeviceDescriptorIndexingFeatures physical_device_descriptor_indexing_features;
	memset(&physical_device_descriptor_indexing_features, 0, sizeof(VkPhysicalDeviceDescriptorIndexingFeatures));

	physical_device_descriptor_indexing_features.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_DESCRIPTOR_INDEXING_FEATURES;
	physical_device_descriptor_indexing_features.pNext = 0;

	VkPhysicalDeviceFeatures2 physical_device_features_2;
	memset(&physical_device_features_2, 0, sizeof(VkPhysicalDeviceFeatures2));

	physical_device_features_2.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_FEATURES_2;
	physical_device_features_2.pNext = &physical_device_descriptor_indexing_features;

	vkGetPhysicalDeviceFeatures2(g_vulkan_instance_physical_device, &physical_device_features_2);
	vkGetPhysicalDeviceSurfaceCapabilitiesKHR(g_vulkan_instance_physical_device, g_vulkan_instance_surface, &g_vulkan_instance_surface_capabilities);

	physical_device_features_2.features.samplerAnisotropy = 1;
	physical_device_features_2.features.shaderFloat64 = 1;

	VkDeviceCreateInfo device_create_info;
	memset(&device_create_info, 0, sizeof(VkDeviceCreateInfo));

	device_create_info.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
	device_create_info.pQueueCreateInfos = std_vector_buffer(&device_queue_create_infos);
	device_create_info.queueCreateInfoCount = (uint32_t)std_vector_count(&device_queue_create_infos);
	device_create_info.pEnabledFeatures = 0;
	device_create_info.pNext = &physical_device_features_2;
	device_create_info.ppEnabledExtensionNames = s_vulkan_instance_device_extensions;
	device_create_info.enabledExtensionCount = ARRAY_COUNT(s_vulkan_instance_device_extensions);

#ifdef VEGA_DEBUG
	device_create_info.ppEnabledLayerNames = s_vulkan_instance_validation_layers;
	device_create_info.enabledLayerCount = ARRAY_COUNT(s_vulkan_instance_validation_layers);
#endif

	vkCreateDevice(g_vulkan_instance_physical_device, &device_create_info, 0, &g_vulkan_instance_device);

	vkGetDeviceQueue(g_vulkan_instance_device, g_vulkan_instance_graphic_and_compute_queue_index, 0, &g_vulkan_instance_graphic_queue);
	vkGetDeviceQueue(g_vulkan_instance_device, g_vulkan_instance_graphic_and_compute_queue_index, 0, &g_vulkan_instance_compute_queue);
	vkGetDeviceQueue(g_vulkan_instance_device, g_vulkan_instance_present_queue_index, 0, &g_vulkan_instance_present_queue);

	std_vector_free(&device_queue_create_infos);

	TRACY_ZONE_END
}
void vulkan_instance_command_pool_alloc(void)
{
	TRACY_ZONE_BEGIN

	VkCommandPoolCreateInfo command_pool_create_info;
	memset(&command_pool_create_info, 0, sizeof(VkCommandPoolCreateInfo));

	command_pool_create_info.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
	command_pool_create_info.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;
	command_pool_create_info.queueFamilyIndex = g_vulkan_instance_graphic_and_compute_queue_index;

	vkCreateCommandPool(g_vulkan_instance_device, &command_pool_create_info, 0, &g_vulkan_instance_command_pool);

	TRACY_ZONE_END
}
void vulkan_instance_find_physical_device(void)
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

		vkGetPhysicalDeviceProperties(physical_device, &g_vulkan_instance_physical_device_properties);
		vkGetPhysicalDeviceFeatures(physical_device, &g_vulkan_instance_physical_device_features);

		if (g_vulkan_instance_physical_device_properties.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU)
		{
			if (g_vulkan_instance_physical_device_features.geometryShader &&
				g_vulkan_instance_physical_device_features.samplerAnisotropy &&
				g_vulkan_instance_physical_device_features.shaderFloat64)
			{
				g_vulkan_instance_physical_device = physical_device;

				break;
			}
		}

		physical_device_index++;
	}

	std_vector_free(&physical_devices);

	TRACY_ZONE_END
}
void vulkan_instance_find_physical_device_queue_families(void)
{
	TRACY_ZONE_BEGIN

	uint32_t queue_family_property_count = 0;
	vkGetPhysicalDeviceQueueFamilyProperties(g_vulkan_instance_physical_device, &queue_family_property_count, 0);

	vector_t queue_family_properties = std_vector_alloc(sizeof(VkQueueFamilyProperties));
	std_vector_resize(&queue_family_properties, queue_family_property_count);

	vkGetPhysicalDeviceQueueFamilyProperties(g_vulkan_instance_physical_device, &queue_family_property_count, std_vector_buffer(&queue_family_properties));

	uint64_t physical_device_queue_family_property_index = 0;
	while (physical_device_queue_family_property_index < queue_family_property_count)
	{
		VkQueueFamilyProperties properties = *(VkQueueFamilyProperties*)std_vector_get(&queue_family_properties, physical_device_queue_family_property_index);

		uint32_t graphic_support = 0;
		uint32_t compute_support = 0;
		uint32_t present_support = 0;

		graphic_support = properties.queueFlags & VK_QUEUE_GRAPHICS_BIT;
		compute_support = properties.queueFlags & VK_QUEUE_COMPUTE_BIT;

		vkGetPhysicalDeviceSurfaceSupportKHR(g_vulkan_instance_physical_device, (uint32_t)physical_device_queue_family_property_index, g_vulkan_instance_surface, &present_support);

		if (graphic_support && compute_support && (g_vulkan_instance_graphic_and_compute_queue_index == -1))
		{
			g_vulkan_instance_graphic_and_compute_queue_index = (uint32_t)physical_device_queue_family_property_index;
		}
		else if (present_support && (g_vulkan_instance_present_queue_index == -1))
		{
			g_vulkan_instance_present_queue_index = (uint32_t)physical_device_queue_family_property_index;
		}

		if ((g_vulkan_instance_graphic_and_compute_queue_index != -1) && (g_vulkan_instance_present_queue_index != -1))
		{
			break;
		}

		physical_device_queue_family_property_index++;
	}

	std_vector_free(&queue_family_properties);

	printf("Queue Indices\n");
	printf("\tGraphicAndComputeQueueIndex %d\n", g_vulkan_instance_graphic_and_compute_queue_index);
	printf("\tPresentQueueIndex %d\n", g_vulkan_instance_present_queue_index);
	printf("\n");

	TRACY_ZONE_END
}
void vulkan_instance_check_physical_device_extensions(void)
{
	TRACY_ZONE_BEGIN

	uint32_t available_device_extension_count = 0;
	vkEnumerateDeviceExtensionProperties(g_vulkan_instance_physical_device, 0, &available_device_extension_count, 0);

	vector_t available_device_extensions = std_vector_alloc(sizeof(VkExtensionProperties));
	std_vector_resize(&available_device_extensions, available_device_extension_count);

	vkEnumerateDeviceExtensionProperties(g_vulkan_instance_physical_device, 0, &available_device_extension_count, std_vector_buffer(&available_device_extensions));

	printf("Device Extensions\n");

	uint64_t device_extension_index = 0;
	uint64_t device_extension_count = ARRAY_COUNT(s_vulkan_instance_device_extensions);
	while (device_extension_index < device_extension_count)
	{
		uint8_t device_extensions_available = 0;

		uint64_t available_device_extension_index = 0;
		while (available_device_extension_index < available_device_extension_count)
		{
			VkExtensionProperties properties = *(VkExtensionProperties*)std_vector_get(&available_device_extensions, available_device_extension_index);

			if (strcmp(s_vulkan_instance_device_extensions[device_extension_index], properties.extensionName) == 0)
			{
				printf("\tFound %s\n", s_vulkan_instance_device_extensions[device_extension_index]);

				device_extensions_available = 1;

				break;
			}

			available_device_extension_index++;
		}

		if (device_extensions_available == 0)
		{
			printf("\tMissing %s\n", s_vulkan_instance_device_extensions[device_extension_index]);

			break;
		}

		device_extension_index++;
	}

	printf("\n");

	std_vector_free(&available_device_extensions);

	TRACY_ZONE_END
}
void vulkan_instance_check_surface_capabilities(void)
{
	TRACY_ZONE_BEGIN

	uint32_t surface_format_count = 0;
	uint32_t present_mode_count = 0;

	vkGetPhysicalDeviceSurfaceFormatsKHR(g_vulkan_instance_physical_device, g_vulkan_instance_surface, &surface_format_count, 0);
	vkGetPhysicalDeviceSurfacePresentModesKHR(g_vulkan_instance_physical_device, g_vulkan_instance_surface, &present_mode_count, 0);

	vector_t surface_formats = std_vector_alloc(sizeof(VkSurfaceFormatKHR));
	vector_t present_modes = std_vector_alloc(sizeof(VkPresentModeKHR));

	std_vector_resize(&surface_formats, surface_format_count);
	std_vector_resize(&present_modes, present_mode_count);

	vkGetPhysicalDeviceSurfaceFormatsKHR(g_vulkan_instance_physical_device, g_vulkan_instance_surface, &surface_format_count, std_vector_buffer(&surface_formats));
	vkGetPhysicalDeviceSurfacePresentModesKHR(g_vulkan_instance_physical_device, g_vulkan_instance_surface, &present_mode_count, std_vector_buffer(&present_modes));

	uint64_t surface_format_index = 0;
	while (surface_format_index < surface_format_count)
	{
		VkSurfaceFormatKHR surface_format = *(VkSurfaceFormatKHR*)std_vector_get(&surface_formats, surface_format_index);

		if ((surface_format.format == VK_FORMAT_B8G8R8A8_UNORM) && (surface_format.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR))
		{
			g_vulkan_instance_prefered_surface_format = surface_format;

			break;
		}

		surface_format_index++;
	}

	uint64_t present_mode_index = 0;
	while (present_mode_index < present_mode_count)
	{
		VkPresentModeKHR present_mode = *(VkPresentModeKHR*)std_vector_get(&present_modes, present_mode_index);

		if (present_mode == VK_PRESENT_MODE_MAILBOX_KHR)
		{
			g_vulkan_instance_prefered_present_mode = present_mode;

			break;
		}

		present_mode_index++;
	}

	std_vector_free(&surface_formats);
	std_vector_free(&present_modes);

	TRACY_ZONE_END
}
void vulkan_instance_surface_free(void)
{
	TRACY_ZONE_BEGIN

	vkDestroySurfaceKHR(g_vulkan_instance, g_vulkan_instance_surface, 0);

	TRACY_ZONE_END
}
void vulkan_instance_device_free(void)
{
	TRACY_ZONE_BEGIN

	vkDestroyDevice(g_vulkan_instance_device, 0);

	TRACY_ZONE_END
}
void vulkan_instance_command_pool_free(void)
{
	TRACY_ZONE_BEGIN

	vkDestroyCommandPool(g_vulkan_instance_device, g_vulkan_instance_command_pool, 0);

	TRACY_ZONE_END
}
#ifdef VEGA_DEBUG
static VkBool32 vulkan_instance_message_proc(VkDebugUtilsMessageSeverityFlagBitsEXT message_severity, VkDebugUtilsMessageTypeFlagsEXT message_type, VkDebugUtilsMessengerCallbackDataEXT const* callback_data, void* user_data)
{
	printf("%s\n", callback_data->pMessage);

	return 0;
}
#endif // VEGA_DEBUG