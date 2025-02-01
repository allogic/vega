#include <string.h>

#include <vega/engine/vulkan/instance.h>
#include <vega/engine/vulkan/image.h>
#include <vega/engine/vulkan/swap_chain.h>

#ifndef TRACY_ZONE_BEGIN
	#define TRACY_ZONE_BEGIN TracyCZoneC(ctx, TRACY_COLOR_GREEN, 1U);
#endif // TRACY_ZONE_BEGIN

#ifndef TRACY_ZONE_END
	#define TRACY_ZONE_END TracyCZoneEnd(ctx);
#endif // TRACY_ZONE_END

VkFormat g_vulkan_swap_chain_depth_format = { 0 };

uint32_t g_vulkan_swap_chain_image_count = 0;

vector_t g_vulkan_swap_chain_image_views = { 0 };

image_t g_vulkan_swap_chain_depth_image = { 0 };

VkSwapchainKHR g_vulkan_swap_chain = 0;

static vector_t s_vulkan_swap_chain_images = { 0 };

void vulkan_swap_chain_alloc(void)
{
	TRACY_ZONE_BEGIN

	s_vulkan_swap_chain_images = std_vector_alloc(sizeof(VkImage));
	g_vulkan_swap_chain_image_views = std_vector_alloc(sizeof(VkImageView));

	VkSwapchainCreateInfoKHR swap_chain_create_info = { 0 };
	swap_chain_create_info.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
	swap_chain_create_info.surface = g_vulkan_instance_surface;
	swap_chain_create_info.minImageCount = g_vulkan_instance_surface_capabilities.minImageCount + 1;
	swap_chain_create_info.imageFormat = g_vulkan_instance_prefered_surface_format.format;
	swap_chain_create_info.imageColorSpace = g_vulkan_instance_prefered_surface_format.colorSpace;
	swap_chain_create_info.imageExtent.width = g_vulkan_instance_surface_capabilities.currentExtent.width;
	swap_chain_create_info.imageExtent.height = g_vulkan_instance_surface_capabilities.currentExtent.height;
	swap_chain_create_info.imageArrayLayers = 1;
	swap_chain_create_info.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;
	swap_chain_create_info.preTransform = g_vulkan_instance_surface_capabilities.currentTransform;
	swap_chain_create_info.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
	swap_chain_create_info.presentMode = g_vulkan_instance_prefered_present_mode;
	swap_chain_create_info.clipped = 1;
	swap_chain_create_info.oldSwapchain = 0;

	int32_t queue_families[2] = { g_vulkan_instance_graphic_and_compute_queue_index, g_vulkan_instance_present_queue_index };

	if (g_vulkan_instance_graphic_and_compute_queue_index == g_vulkan_instance_present_queue_index)
	{
		swap_chain_create_info.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
		swap_chain_create_info.queueFamilyIndexCount = 0;
		swap_chain_create_info.pQueueFamilyIndices = 0;
	}
	else
	{
		swap_chain_create_info.imageSharingMode = VK_SHARING_MODE_CONCURRENT;
		swap_chain_create_info.pQueueFamilyIndices = queue_families;
		swap_chain_create_info.queueFamilyIndexCount = ARRAY_COUNT(queue_families);
	}

	vkCreateSwapchainKHR(g_vulkan_instance_device, &swap_chain_create_info, 0, &g_vulkan_swap_chain);

	vulkan_swap_chain_images_alloc();
	vulkan_swap_chain_image_views_alloc();

	TRACY_ZONE_END
}
void vulkan_swap_chain_free(void)
{
	TRACY_ZONE_BEGIN

	vulkan_swap_chain_image_views_free();
	vulkan_swap_chain_images_free();

	vkDestroySwapchainKHR(g_vulkan_instance_device, g_vulkan_swap_chain, 0);

	std_vector_free(&g_vulkan_swap_chain_image_views);
	std_vector_free(&s_vulkan_swap_chain_images);

	TRACY_ZONE_END
}
void vulkan_swap_chain_resize_before(void)
{
	TRACY_ZONE_BEGIN

	vulkan_swap_chain_image_views_free();
	vulkan_swap_chain_images_free();

	vkDestroySwapchainKHR(g_vulkan_instance_device, g_vulkan_swap_chain, 0);

	TRACY_ZONE_END
}
void vulkan_swap_chain_resize_after(void)
{
	TRACY_ZONE_BEGIN

	VkSwapchainCreateInfoKHR swap_chain_create_info = { 0 };
	swap_chain_create_info.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
	swap_chain_create_info.surface = g_vulkan_instance_surface;
	swap_chain_create_info.minImageCount = g_vulkan_instance_surface_capabilities.minImageCount + 1;
	swap_chain_create_info.imageFormat = g_vulkan_instance_prefered_surface_format.format;
	swap_chain_create_info.imageColorSpace = g_vulkan_instance_prefered_surface_format.colorSpace;
	swap_chain_create_info.imageExtent.width = g_vulkan_instance_surface_capabilities.currentExtent.width;
	swap_chain_create_info.imageExtent.height = g_vulkan_instance_surface_capabilities.currentExtent.height;
	swap_chain_create_info.imageArrayLayers = 1;
	swap_chain_create_info.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;
	swap_chain_create_info.preTransform = g_vulkan_instance_surface_capabilities.currentTransform;
	swap_chain_create_info.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
	swap_chain_create_info.presentMode = g_vulkan_instance_prefered_present_mode;
	swap_chain_create_info.clipped = 1;
	swap_chain_create_info.oldSwapchain = 0;

	int32_t queue_families[2] = { g_vulkan_instance_graphic_and_compute_queue_index, g_vulkan_instance_present_queue_index };

	if (g_vulkan_instance_graphic_and_compute_queue_index == g_vulkan_instance_present_queue_index)
	{
		swap_chain_create_info.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
		swap_chain_create_info.queueFamilyIndexCount = 0;
		swap_chain_create_info.pQueueFamilyIndices = 0;
	}
	else
	{
		swap_chain_create_info.imageSharingMode = VK_SHARING_MODE_CONCURRENT;
		swap_chain_create_info.pQueueFamilyIndices = queue_families;
		swap_chain_create_info.queueFamilyIndexCount = ARRAY_COUNT(queue_families);
	}

	vkCreateSwapchainKHR(g_vulkan_instance_device, &swap_chain_create_info, 0, &g_vulkan_swap_chain);

	vulkan_swap_chain_images_alloc();
	vulkan_swap_chain_image_views_alloc();

	TRACY_ZONE_END
}
void vulkan_swap_chain_images_alloc(void)
{
	TRACY_ZONE_BEGIN

	vkGetSwapchainImagesKHR(g_vulkan_instance_device, g_vulkan_swap_chain, &g_vulkan_swap_chain_image_count, 0);

	std_vector_resize(&s_vulkan_swap_chain_images, g_vulkan_swap_chain_image_count);

	vkGetSwapchainImagesKHR(g_vulkan_instance_device, g_vulkan_swap_chain, &g_vulkan_swap_chain_image_count, std_vector_buffer(&s_vulkan_swap_chain_images));

	g_vulkan_swap_chain_depth_format = vulkan_image_find_depth_format();
	g_vulkan_swap_chain_depth_image = vulkan_image_2d_depth_alloc(g_vulkan_instance_surface_capabilities.currentExtent.width, g_vulkan_instance_surface_capabilities.currentExtent.height, g_vulkan_swap_chain_depth_format);

	TRACY_ZONE_END
}
void vulkan_swap_chain_image_views_alloc(void)
{
	TRACY_ZONE_BEGIN

	std_vector_resize(&g_vulkan_swap_chain_image_views, g_vulkan_swap_chain_image_count);

	uint32_t swap_chain_image_index = 0;
	while (swap_chain_image_index < g_vulkan_swap_chain_image_count)
	{
		VkImage image = *(VkImage*)std_vector_get(&s_vulkan_swap_chain_images, swap_chain_image_index);
		VkImageView* image_view = (VkImageView*)std_vector_get(&g_vulkan_swap_chain_image_views, swap_chain_image_index);

		*image_view = vulkan_image_view_alloc(image, VK_IMAGE_VIEW_TYPE_2D, g_vulkan_instance_prefered_surface_format.format, VK_IMAGE_ASPECT_COLOR_BIT);

		swap_chain_image_index++;
	}

	TRACY_ZONE_END
}
void vulkan_swap_chain_images_free(void)
{
	TRACY_ZONE_BEGIN

	vulkan_image_free(&g_vulkan_swap_chain_depth_image);

	std_vector_clear(&s_vulkan_swap_chain_images);

	TRACY_ZONE_END
}
void vulkan_swap_chain_image_views_free(void)
{
	TRACY_ZONE_BEGIN

	uint32_t swap_chain_image_index = 0;
	while (swap_chain_image_index < g_vulkan_swap_chain_image_count)
	{
		VkImageView image_view = *(VkImageView*)std_vector_get(&g_vulkan_swap_chain_image_views, swap_chain_image_index);

		vulkan_image_view_free(image_view);

		swap_chain_image_index++;
	}

	std_vector_clear(&g_vulkan_swap_chain_image_views);

	TRACY_ZONE_END
}