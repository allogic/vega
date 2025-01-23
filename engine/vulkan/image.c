#include <string.h>

#include <vega/engine/vulkan/image.h>

#ifndef TRACY_ZONE_BEGIN
	#define TRACY_ZONE_BEGIN TracyCZoneC(ctx, TRACY_COLOR_BLUE, 1U)
#endif // TRACY_ZONE_BEGIN

#ifndef TRACY_ZONE_END
	#define TRACY_ZONE_END TracyCZoneEnd(ctx)
#endif // TRACY_ZONE_END

image_t vulkan_image_2d_r_alloc(void* buffer, uint32_t width, uint32_t height, VkFormat format)
{
	TRACY_ZONE_BEGIN

	image_t image;
	memset(&image, 0, sizeof(image_t));

	TRACY_ZONE_END

	return image;
}
image_t vulkan_image_2d_rg_alloc(void* buffer, uint32_t width, uint32_t height, VkFormat format)
{
	TRACY_ZONE_BEGIN

	image_t image;
	memset(&image, 0, sizeof(image_t));

	TRACY_ZONE_END

	return image;
}
image_t vulkan_image_2d_rgb_alloc(void* buffer, uint32_t width, uint32_t height, VkFormat format)
{
	TRACY_ZONE_BEGIN

	image_t image;
	memset(&image, 0, sizeof(image_t));

	TRACY_ZONE_END

	return image;
}
image_t vulkan_image_2d_rgba_alloc(void* buffer, uint32_t width, uint32_t height, VkFormat format)
{
	TRACY_ZONE_BEGIN

	image_t image;
	memset(&image, 0, sizeof(image_t));

	TRACY_ZONE_END

	return image;
}
image_t vulkan_image_2d_depth_alloc(uint32_t width, uint32_t height, VkFormat format)
{
	TRACY_ZONE_BEGIN

	image_t image;
	memset(&image, 0, sizeof(image_t));

	TRACY_ZONE_END

	return image;
}
void vulkan_image_free(image_t* image)
{
	TRACY_ZONE_BEGIN

	// TODO

	TRACY_ZONE_END
}
VkImageView vulkan_image_view_alloc(VkImage image, VkImageViewType view_type, VkFormat format, VkImageAspectFlags aspect_flags)
{
	TRACY_ZONE_BEGIN

	// TODO

	TRACY_ZONE_END

	return 0;
}
void vulkan_image_view_free(VkImageView image_view)
{
	TRACY_ZONE_BEGIN

	// TODO

	TRACY_ZONE_END
}
