#define WIN32_LEAN_AND_MEAN
#define NO_GUID_DEFS
#include <windows.h>

#define VK_USE_PLATFORM_WIN32_KHR
#include <vulkan/vulkan.h>

#include <vega/engine/platform/window.h>

#include <vega/engine/vulkan/device.h>
#include <vega/engine/vulkan/instance.h>
#include <vega/engine/vulkan/surface.h>

#ifndef TRACY_ZONE_BEGIN
	#define TRACY_ZONE_BEGIN TracyCZoneC(ctx, TRACY_COLOR_GREEN, 1U);
#endif // TRACY_ZONE_BEGIN

#ifndef TRACY_ZONE_END
	#define TRACY_ZONE_END TracyCZoneEnd(ctx);
#endif // TRACY_ZONE_END

VkSurfaceKHR g_vulkan_surface = 0;
VkSurfaceCapabilitiesKHR g_vulkan_surface_capabilities = { 0 };
VkSurfaceFormatKHR g_vulkan_surface_prefered_format = { 0 };

VkPresentModeKHR g_vulkan_surface_prefered_present_mode = { 0 };

uint32_t g_vulkan_surface_width = 0;
uint32_t g_vulkan_surface_height = 0;

void vulkan_surface_alloc(void)
{
	TRACY_ZONE_BEGIN

	VkWin32SurfaceCreateInfoKHR surface_create_info = { 0 };
	surface_create_info.sType = VK_STRUCTURE_TYPE_WIN32_SURFACE_CREATE_INFO_KHR;
	surface_create_info.hwnd = g_platform_window_handle;
	surface_create_info.hinstance = g_platform_window_module_handle;

	vkCreateWin32SurfaceKHR(g_vulkan_instance, &surface_create_info, 0, &g_vulkan_surface);

	TRACY_ZONE_END
}
void vulkan_surface_check_capabilities(void)
{
	TRACY_ZONE_BEGIN

	uint32_t surface_format_count = 0;
	uint32_t present_mode_count = 0;

	vkGetPhysicalDeviceSurfaceFormatsKHR(g_vulkan_device_physical, g_vulkan_surface, &surface_format_count, 0);
	vkGetPhysicalDeviceSurfacePresentModesKHR(g_vulkan_device_physical, g_vulkan_surface, &present_mode_count, 0);

	vector_t surface_formats = std_vector_alloc(sizeof(VkSurfaceFormatKHR));
	vector_t present_modes = std_vector_alloc(sizeof(VkPresentModeKHR));

	std_vector_resize(&surface_formats, surface_format_count);
	std_vector_resize(&present_modes, present_mode_count);

	vkGetPhysicalDeviceSurfaceFormatsKHR(g_vulkan_device_physical, g_vulkan_surface, &surface_format_count, std_vector_buffer(&surface_formats));
	vkGetPhysicalDeviceSurfacePresentModesKHR(g_vulkan_device_physical, g_vulkan_surface, &present_mode_count, std_vector_buffer(&present_modes));

	uint64_t surface_format_index = 0;
	while (surface_format_index < surface_format_count)
	{
		VkSurfaceFormatKHR surface_format = *(VkSurfaceFormatKHR*)std_vector_get(&surface_formats, surface_format_index);

		if ((surface_format.format == VK_FORMAT_B8G8R8A8_UNORM) && (surface_format.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR))
		{
			g_vulkan_surface_prefered_format = surface_format;

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
			g_vulkan_surface_prefered_present_mode = present_mode;

			break;
		}

		present_mode_index++;
	}

	std_vector_free(&surface_formats);
	std_vector_free(&present_modes);

	TRACY_ZONE_END
}
void vulkan_surface_resize(void)
{
	TRACY_ZONE_BEGIN

	vkGetPhysicalDeviceSurfaceCapabilitiesKHR(g_vulkan_device_physical, g_vulkan_surface, &g_vulkan_surface_capabilities);

	g_vulkan_surface_width = g_vulkan_surface_capabilities.currentExtent.width;
	g_vulkan_surface_height = g_vulkan_surface_capabilities.currentExtent.height;

	TRACY_ZONE_END
}
void vulkan_surface_free(void)
{
	TRACY_ZONE_BEGIN

	vkDestroySurfaceKHR(g_vulkan_instance, g_vulkan_surface, 0);

	TRACY_ZONE_END
}
