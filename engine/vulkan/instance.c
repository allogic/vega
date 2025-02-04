#include <stdio.h>

#include <vega/engine/vulkan/instance.h>

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

#ifdef VEGA_DEBUG
char const* g_vulkan_instance_validation_layers[1] =
{
	"VK_LAYER_KHRONOS_validation",
};
#endif // VEGA_DEBUG

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

void vulkan_instance_alloc(void)
{
	TRACY_ZONE_BEGIN

	VkApplicationInfo app_info = { 0 };
	app_info.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
	app_info.pApplicationName = "";
	app_info.applicationVersion = VK_MAKE_API_VERSION(0, 1, 0, 0);
	app_info.pEngineName = "";
	app_info.engineVersion = VK_MAKE_API_VERSION(0, 1, 0, 0);
	app_info.apiVersion = VK_API_VERSION_1_0;

	VkInstanceCreateInfo instance_create_info = { 0 };
	instance_create_info.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
	instance_create_info.pApplicationInfo = &app_info;
	instance_create_info.enabledExtensionCount = (uint32_t)VEGA_ARRAY_COUNT(s_vulkan_instance_layer_extensions);
	instance_create_info.ppEnabledExtensionNames = s_vulkan_instance_layer_extensions;

#ifdef VEGA_DEBUG
	VkDebugUtilsMessengerCreateInfoEXT debug_create_info = { 0 };
	debug_create_info.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
	debug_create_info.messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
	debug_create_info.messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
	debug_create_info.pfnUserCallback = vulkan_instance_message_proc;

	instance_create_info.pNext = (VkDebugUtilsMessengerCreateInfoEXT*)&debug_create_info;
	instance_create_info.enabledLayerCount = VEGA_ARRAY_COUNT(g_vulkan_instance_validation_layers);
	instance_create_info.ppEnabledLayerNames = g_vulkan_instance_validation_layers;
#endif // VEGA_DEBUG

	vkCreateInstance(&instance_create_info, 0, &g_vulkan_instance);

#ifdef VEGA_DEBUG
	s_vulkan_instance_create_debug_utils_messenger_ext = (PFN_vkCreateDebugUtilsMessengerEXT)vkGetInstanceProcAddr(g_vulkan_instance, "vkCreateDebugUtilsMessengerEXT");
	s_destroy_instance_debug_utils_messenger_ext = (PFN_vkDestroyDebugUtilsMessengerEXT)vkGetInstanceProcAddr(g_vulkan_instance, "vkDestroyDebugUtilsMessengerEXT");

	s_vulkan_instance_create_debug_utils_messenger_ext(g_vulkan_instance, &debug_create_info, 0, &s_vulkan_instance_debug_messenger);
#endif // VEGA_DEBUG

	TRACY_ZONE_END
}
void vulkan_instance_free(void)
{
	TRACY_ZONE_BEGIN

#ifdef VEGA_DEBUG
	s_destroy_instance_debug_utils_messenger_ext(g_vulkan_instance, s_vulkan_instance_debug_messenger, 0);
#endif

	vkDestroyInstance(g_vulkan_instance, 0);

	TRACY_ZONE_END
}
#ifdef VEGA_DEBUG
static VkBool32 vulkan_instance_message_proc(VkDebugUtilsMessageSeverityFlagBitsEXT message_severity, VkDebugUtilsMessageTypeFlagsEXT message_type, VkDebugUtilsMessengerCallbackDataEXT const* callback_data, void* user_data)
{
	printf("%s\n", callback_data->pMessage);

	return 0;
}
#endif // VEGA_DEBUG