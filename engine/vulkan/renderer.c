#include <string.h>

#include <vega/engine/vulkan/buffer.h>
#include <vega/engine/vulkan/instance.h>
#include <vega/engine/vulkan/swap_chain.h>
#include <vega/engine/vulkan/renderer.h>

#ifndef TRACY_ZONE_BEGIN
	#define TRACY_ZONE_BEGIN TracyCZoneC(ctx, TRACY_COLOR_GREEN, 1U);
#endif // TRACY_ZONE_BEGIN

#ifndef TRACY_ZONE_END
	#define TRACY_ZONE_END TracyCZoneEnd(ctx);
#endif // TRACY_ZONE_END

static time_info_t s_vulkan_renderer_time_info = { 0 };
static screen_info_t s_vulkan_renderer_screen_info = { 0 };
static projection_info_t s_vulkan_renderer_projection_info = { 0 };

static VkCommandBuffer s_vulkan_renderer_graphic_command_buffer = 0;
static VkCommandBuffer s_vulkan_renderer_compute_command_buffer = 0;

static VkSemaphore s_vulkan_renderer_graphic_complete_semaphore = 0;
static VkSemaphore s_vulkan_renderer_compute_complete_semaphore = 0;
static VkSemaphore s_vulkan_renderer_image_available_semaphore = 0;

static VkFence s_vulkan_renderer_render_fence = 0;

static VkRenderPass s_vulkan_renderer_render_pass = 0;

static vector_t s_vulkan_renderer_frame_buffers = { 0 };

void vulkan_renderer_alloc(void)
{
	TRACY_ZONE_BEGIN

	s_vulkan_renderer_frame_buffers = std_vector_alloc(sizeof(VkFramebuffer));

	vulkan_renderer_command_buffer_alloc();
	vulkan_renderer_sync_objects_alloc();
	vulkan_renderer_render_pass_alloc();
	vulkan_renderer_frame_buffer_alloc();

	TRACY_ZONE_END
}
void vulkan_renderer_render(void)
{
	TRACY_ZONE_BEGIN

	TRACY_ZONE_END
}
void vulkan_renderer_free(void)
{
	TRACY_ZONE_BEGIN

	vulkan_renderer_frame_buffer_free();
	vulkan_renderer_render_pass_free();
	vulkan_renderer_sync_objects_free();
	vulkan_renderer_command_buffer_free();

	std_vector_free(&s_vulkan_renderer_frame_buffers);

	TRACY_ZONE_END
}
void vulkan_renderer_resize_before(void)
{
	TRACY_ZONE_BEGIN

	vulkan_renderer_frame_buffer_free();
	vulkan_renderer_render_pass_free();

	TRACY_ZONE_END
}
void vulkan_renderer_resize_after(void)
{
	TRACY_ZONE_BEGIN

	vulkan_renderer_render_pass_alloc();
	vulkan_renderer_frame_buffer_alloc();

	TRACY_ZONE_END
}
void vulkan_renderer_command_buffer_alloc(void)
{
	TRACY_ZONE_BEGIN

	VkCommandBufferAllocateInfo command_buffer_alloc_create_info;
	memset(&command_buffer_alloc_create_info, 0, sizeof(VkCommandBufferAllocateInfo));

	command_buffer_alloc_create_info.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
	command_buffer_alloc_create_info.commandPool = g_vulkan_instance_command_pool;
	command_buffer_alloc_create_info.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
	command_buffer_alloc_create_info.commandBufferCount = 1;

	vkAllocateCommandBuffers(g_vulkan_instance_device, &command_buffer_alloc_create_info, &s_vulkan_renderer_graphic_command_buffer);
	vkAllocateCommandBuffers(g_vulkan_instance_device, &command_buffer_alloc_create_info, &s_vulkan_renderer_compute_command_buffer);

	TRACY_ZONE_END
}
void vulkan_renderer_sync_objects_alloc(void)
{
	TRACY_ZONE_BEGIN

	VkSemaphoreCreateInfo semaphore_create_info;
	memset(&semaphore_create_info, 0, sizeof(VkSemaphoreCreateInfo));

	semaphore_create_info.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;

	vkCreateSemaphore(g_vulkan_instance_device, &semaphore_create_info, 0, &s_vulkan_renderer_graphic_complete_semaphore);
	vkCreateSemaphore(g_vulkan_instance_device, &semaphore_create_info, 0, &s_vulkan_renderer_compute_complete_semaphore);
	vkCreateSemaphore(g_vulkan_instance_device, &semaphore_create_info, 0, &s_vulkan_renderer_image_available_semaphore);

	VkFenceCreateInfo fence_create_info;
	memset(&fence_create_info, 0, sizeof(VkFenceCreateInfo));

	fence_create_info.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
	fence_create_info.flags = VK_FENCE_CREATE_SIGNALED_BIT;

	vkCreateFence(g_vulkan_instance_device, &fence_create_info, 0, &s_vulkan_renderer_render_fence);

	TRACY_ZONE_END
}
void vulkan_renderer_render_pass_alloc(void)
{
	TRACY_ZONE_BEGIN

	VkAttachmentDescription color_attachment_description;
	memset(&color_attachment_description, 0, sizeof(VkAttachmentDescription));

	color_attachment_description.format = g_vulkan_instance_prefered_surface_format.format;
	color_attachment_description.samples = VK_SAMPLE_COUNT_1_BIT;
	color_attachment_description.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
	color_attachment_description.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
	color_attachment_description.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
	color_attachment_description.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
	color_attachment_description.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
	color_attachment_description.finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;

	VkAttachmentDescription depth_attachment_description;
	memset(&depth_attachment_description, 0, sizeof(VkAttachmentDescription));

	depth_attachment_description.format = g_vulkan_swap_chain_depth_format;
	depth_attachment_description.samples = VK_SAMPLE_COUNT_1_BIT;
	depth_attachment_description.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
	depth_attachment_description.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
	depth_attachment_description.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
	depth_attachment_description.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
	depth_attachment_description.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
	depth_attachment_description.finalLayout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;

	VkAttachmentReference color_attachment_reference;
	memset(&color_attachment_reference, 0, sizeof(VkAttachmentReference));

	color_attachment_reference.attachment = 0;
	color_attachment_reference.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

	VkAttachmentReference depth_attachment_reference;
	memset(&depth_attachment_reference, 0, sizeof(VkAttachmentReference));

	depth_attachment_reference.attachment = 1;
	depth_attachment_reference.layout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;

	VkSubpassDescription subpass_description;
	memset(&subpass_description, 0, sizeof(VkSubpassDescription));

	subpass_description.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
	subpass_description.colorAttachmentCount = 1;
	subpass_description.pColorAttachments = &color_attachment_reference;
	subpass_description.pDepthStencilAttachment = &depth_attachment_reference;

	VkSubpassDependency subpass_dependency;
	memset(&subpass_dependency, 0, sizeof(VkSubpassDependency));

	subpass_dependency.srcSubpass = VK_SUBPASS_EXTERNAL;
	subpass_dependency.dstSubpass = 0;
	subpass_dependency.srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT | VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT;
	subpass_dependency.srcAccessMask = 0;
	subpass_dependency.dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT | VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT;
	subpass_dependency.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT | VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT;

	VkAttachmentDescription attachment_descriptions[] = { color_attachment_description, depth_attachment_description };

	VkRenderPassCreateInfo render_pass_create_info;
	memset(&render_pass_create_info, 0, sizeof(VkRenderPassCreateInfo));

	render_pass_create_info.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
	render_pass_create_info.pAttachments = attachment_descriptions;
	render_pass_create_info.attachmentCount = (uint32_t)ARRAY_COUNT(attachment_descriptions);
	render_pass_create_info.pSubpasses = &subpass_description;
	render_pass_create_info.subpassCount = 1;
	render_pass_create_info.pDependencies = &subpass_dependency;
	render_pass_create_info.dependencyCount = 1;

	vkCreateRenderPass(g_vulkan_instance_device, &render_pass_create_info, 0, &s_vulkan_renderer_render_pass);

	TRACY_ZONE_END
}
void vulkan_renderer_frame_buffer_alloc(void)
{
	TRACY_ZONE_BEGIN

	// TODO: multiple depth buffers..

	std_vector_resize(&s_vulkan_renderer_frame_buffers, g_vulkan_swap_chain_image_count);

	uint32_t swap_chain_image_index = 0;
	while (swap_chain_image_index < g_vulkan_swap_chain_image_count)
	{
		VkImageView image_attachments[] =
		{
			*(VkImageView*)std_vector_at(&g_vulkan_swap_chain_image_views, swap_chain_image_index),
			g_vulkan_swap_chain_depth_image.image_view,
		};

		VkFramebufferCreateInfo frame_buffer_create_info;
		memset(&frame_buffer_create_info, 0, sizeof(VkFramebufferCreateInfo));

		frame_buffer_create_info.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
		frame_buffer_create_info.renderPass = s_vulkan_renderer_render_pass;
		frame_buffer_create_info.pAttachments = image_attachments;
		frame_buffer_create_info.attachmentCount = (uint32_t)ARRAY_COUNT(image_attachments);
		frame_buffer_create_info.width = g_vulkan_instance_surface_capabilities.currentExtent.width;
		frame_buffer_create_info.height = g_vulkan_instance_surface_capabilities.currentExtent.height;
		frame_buffer_create_info.layers = 1;

		vkCreateFramebuffer(g_vulkan_instance_device, &frame_buffer_create_info, 0, std_vector_at(&s_vulkan_renderer_frame_buffers, swap_chain_image_index));

		swap_chain_image_index++;
	}

	TRACY_ZONE_END
}
void vulkan_renderer_command_buffer_free(void)
{
	TRACY_ZONE_BEGIN

	vkFreeCommandBuffers(g_vulkan_instance_device, g_vulkan_instance_command_pool, 1, &s_vulkan_renderer_graphic_command_buffer);
	vkFreeCommandBuffers(g_vulkan_instance_device, g_vulkan_instance_command_pool, 1, &s_vulkan_renderer_compute_command_buffer);

	TRACY_ZONE_END
}
void vulkan_renderer_sync_objects_free(void)
{
	TRACY_ZONE_BEGIN

	vkDestroySemaphore(g_vulkan_instance_device, s_vulkan_renderer_graphic_complete_semaphore, 0);
	vkDestroySemaphore(g_vulkan_instance_device, s_vulkan_renderer_compute_complete_semaphore, 0);
	vkDestroySemaphore(g_vulkan_instance_device, s_vulkan_renderer_image_available_semaphore, 0);

	vkDestroyFence(g_vulkan_instance_device, s_vulkan_renderer_render_fence, 0);

	TRACY_ZONE_END
}
void vulkan_renderer_render_pass_free(void)
{
	TRACY_ZONE_BEGIN

	vkDestroyRenderPass(g_vulkan_instance_device, s_vulkan_renderer_render_pass, 0);

	TRACY_ZONE_END
}
void vulkan_renderer_frame_buffer_free(void)
{
	TRACY_ZONE_BEGIN

	uint32_t swap_chain_image_index = 0;
	while (swap_chain_image_index < g_vulkan_swap_chain_image_count)
	{
		vkDestroyFramebuffer(g_vulkan_instance_device, *(VkFramebuffer*)std_vector_at(&s_vulkan_renderer_frame_buffers, swap_chain_image_index), 0);

		swap_chain_image_index++;
	}

	TRACY_ZONE_END
}
