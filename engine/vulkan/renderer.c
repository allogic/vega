#include <string.h>

#include <vega/engine/scene.h>

#include <vega/engine/component/camera.h>
#include <vega/engine/component/renderable.h>
#include <vega/engine/component/transform.h>
#include <vega/engine/component/types.h>

#include <vega/engine/platform/window.h>

#include <vega/engine/renderer/material.h>
#include <vega/engine/renderer/mesh.h>
#include <vega/engine/renderer/vertex.h>

#include <vega/engine/vulkan/buffer.h>
#include <vega/engine/vulkan/instance.h>
#include <vega/engine/vulkan/pipeline.h>
#include <vega/engine/vulkan/renderer.h>
#include <vega/engine/vulkan/swap_chain.h>

#ifndef TRACY_ZONE_BEGIN
	#define TRACY_ZONE_BEGIN TracyCZoneC(ctx, TRACY_COLOR_GREEN, 1U);
#endif // TRACY_ZONE_BEGIN

#ifndef TRACY_ZONE_END
	#define TRACY_ZONE_END TracyCZoneEnd(ctx);
#endif // TRACY_ZONE_END

#ifndef VEGA_PBR_VERTEX_BINDING_ID
	#define VEGA_PBR_VERTEX_BINDING_ID (0U)
#endif // VEGA_PBR_VERTEX_BINDING_ID

static void vulkan_renderer_update_projection_info_proc(ecs_t* ecs, uint64_t entity, vector_t* view);
static void vulkan_renderer_update_pbr_descriptor_sets_proc(ecs_t* ecs, uint64_t entity, vector_t* view);
static void vulkan_renderer_pbr_render_proc(ecs_t* ecs, uint64_t entity, vector_t* view);

static buffer_t s_vulkan_renderer_time_info_buffer = { 0 };
static buffer_t s_vulkan_renderer_screen_info_buffer = { 0 };
static buffer_t s_vulkan_renderer_projection_info_buffer = { 0 };

static VkCommandBuffer s_vulkan_renderer_graphic_command_buffer = 0;
static VkCommandBuffer s_vulkan_renderer_compute_command_buffer = 0;

static VkSemaphore s_vulkan_renderer_graphic_complete_semaphore = 0;
static VkSemaphore s_vulkan_renderer_compute_complete_semaphore = 0;
static VkSemaphore s_vulkan_renderer_image_available_semaphore = 0;

static VkFence s_vulkan_renderer_render_fence = 0;

static VkRenderPass s_vulkan_renderer_render_pass = 0;

static vector_t s_vulkan_renderer_frame_buffers = { 0 };

static VkDescriptorSetLayoutBinding s_vulkan_renderer_pbr_descriptor_set_layout_bindings[] =
{
	{ 0, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, 1, VK_SHADER_STAGE_VERTEX_BIT, 0 },
	{ 1, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, 1, VK_SHADER_STAGE_VERTEX_BIT, 0 },
	{ 3, VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, 1, VK_SHADER_STAGE_FRAGMENT_BIT, 0 },
};

static VkVertexInputBindingDescription s_vulkan_renderer_pbr_vertex_input_binding_descriptions[] =
{
	{ VEGA_PBR_VERTEX_BINDING_ID, sizeof(pbr_vertex_t), VK_VERTEX_INPUT_RATE_VERTEX },
};

static VkVertexInputAttributeDescription s_vulkan_renderer_pbr_vertex_input_attribute_descriptions[] =
{
	{ 0, VEGA_PBR_VERTEX_BINDING_ID, VK_FORMAT_R64G64B64A64_SFLOAT, 0 },
	{ 2, VEGA_PBR_VERTEX_BINDING_ID, VK_FORMAT_R64G64B64A64_SFLOAT, VEGA_OFFSET_OF(pbr_vertex_t, normal) },
	{ 4, VEGA_PBR_VERTEX_BINDING_ID, VK_FORMAT_R64G64B64A64_SFLOAT, VEGA_OFFSET_OF(pbr_vertex_t, tangent) },
	{ 6, VEGA_PBR_VERTEX_BINDING_ID, VK_FORMAT_R64G64B64A64_SFLOAT, VEGA_OFFSET_OF(pbr_vertex_t, bitangent) },
	{ 8, VEGA_PBR_VERTEX_BINDING_ID, VK_FORMAT_R64G64B64A64_SFLOAT, VEGA_OFFSET_OF(pbr_vertex_t, color_channel_0) },
	{ 10, VEGA_PBR_VERTEX_BINDING_ID, VK_FORMAT_R64G64_SFLOAT, VEGA_OFFSET_OF(pbr_vertex_t, texcoord_channel_0) },
	{ 12, VEGA_PBR_VERTEX_BINDING_ID, VK_FORMAT_R64G64B64A64_SFLOAT, VEGA_OFFSET_OF(pbr_vertex_t, bone_indices) },
	{ 14, VEGA_PBR_VERTEX_BINDING_ID, VK_FORMAT_R64G64B64A64_SFLOAT, VEGA_OFFSET_OF(pbr_vertex_t, bone_weights) },
};

static char const* s_vulkan_renderer_pbr_vertex_shader_file_path = VEGA_ROOT_DIR "/shaders/pbr.vert.spv";
static char const* s_vulkan_renderer_pbr_fragment_shader_file_path = VEGA_ROOT_DIR "/shaders/pbr.frag.spv";

static VkDescriptorPool s_vulkan_renderer_pbr_descriptor_pool = 0;
static VkDescriptorSetLayout s_vulkan_renderer_pbr_descriptor_set_layout = 0;

static vector_t s_vulkan_renderer_pbr_descriptor_sets = { 0 };

static uint64_t s_vulkan_renderer_pbr_descriptor_set_index = 0;

static VkPipelineLayout s_vulkan_renderer_pbr_pipeline_layout = 0;
static VkPipeline s_vulkan_renderer_pbr_pipeline = 0;

// TODO: create entity filters for materials different then the pbr workflow..

static ecs_query_t s_vulkan_renderer_camera_query = { VEGA_COMPONENT_BIT_TRANSFORM | VEGA_COMPONENT_BIT_CAMERA };
static ecs_query_t s_vulkan_renderer_render_query = { VEGA_COMPONENT_BIT_RENDERABLE };

void vulkan_renderer_alloc(void)
{
	TRACY_ZONE_BEGIN

	s_vulkan_renderer_frame_buffers = std_vector_alloc(sizeof(VkFramebuffer));
	s_vulkan_renderer_pbr_descriptor_sets = std_vector_alloc(sizeof(VkDescriptorSet));

	s_vulkan_renderer_time_info_buffer = vulkan_buffer_uniform_coherent_alloc(sizeof(time_info_t));
	s_vulkan_renderer_screen_info_buffer = vulkan_buffer_uniform_coherent_alloc(sizeof(screen_info_t));
	s_vulkan_renderer_projection_info_buffer = vulkan_buffer_uniform_coherent_alloc(sizeof(projection_info_t));

	vulkan_renderer_command_buffer_alloc();
	vulkan_renderer_sync_objects_alloc();
	vulkan_renderer_render_pass_alloc();
	vulkan_renderer_frame_buffer_alloc();

	// TODO: adjust pool count somehow to entity count..
	s_vulkan_renderer_pbr_descriptor_pool = vulkan_pipeline_descriptor_pool_alloc(1, s_vulkan_renderer_pbr_descriptor_set_layout_bindings, ARRAY_COUNT(s_vulkan_renderer_pbr_descriptor_set_layout_bindings));
	s_vulkan_renderer_pbr_descriptor_set_layout = vulkan_pipeline_descriptor_set_layout_alloc(s_vulkan_renderer_pbr_descriptor_set_layout_bindings, ARRAY_COUNT(s_vulkan_renderer_pbr_descriptor_set_layout_bindings));
	s_vulkan_renderer_pbr_pipeline_layout = vulkan_pipeline_layout_alloc(s_vulkan_renderer_pbr_descriptor_set_layout, 0, 0);
	s_vulkan_renderer_pbr_pipeline = vulkan_pipeline_graphic_alloc(s_vulkan_renderer_pbr_pipeline_layout, s_vulkan_renderer_render_pass, VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST, s_vulkan_renderer_pbr_vertex_shader_file_path, s_vulkan_renderer_pbr_fragment_shader_file_path, s_vulkan_renderer_pbr_vertex_input_binding_descriptions, ARRAY_COUNT(s_vulkan_renderer_pbr_vertex_input_binding_descriptions), s_vulkan_renderer_pbr_vertex_input_attribute_descriptions, ARRAY_COUNT(s_vulkan_renderer_pbr_vertex_input_attribute_descriptions));

	std_ecs_query_alloc(&s_vulkan_renderer_camera_query);
	std_ecs_query_alloc(&s_vulkan_renderer_render_query);

	TRACY_ZONE_END
}
void vulkan_renderer_render(void)
{
	TRACY_ZONE_BEGIN

	vkResetFences(g_vulkan_instance_device, 1, &s_vulkan_renderer_render_fence);

	uint32_t image_index;
	vkAcquireNextImageKHR(g_vulkan_instance_device, g_vulkan_swap_chain, UINT64_MAX, s_vulkan_renderer_image_available_semaphore, 0, &image_index);

	vulkan_renderer_update_uniform_buffers();

	vulkan_renderer_record_compute_command_buffer();
	vulkan_renderer_record_graphic_command_buffer(image_index);

	{
		VkSemaphore signal_semaphores[] = { s_vulkan_renderer_compute_complete_semaphore };

		VkSubmitInfo submit_info;
		memset(&submit_info, 0, sizeof(VkSubmitInfo));

		submit_info.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
		submit_info.pWaitSemaphores = 0;
		submit_info.waitSemaphoreCount = 0;
		submit_info.pSignalSemaphores = signal_semaphores;
		submit_info.signalSemaphoreCount = (uint32_t)ARRAY_COUNT(signal_semaphores);
		submit_info.pWaitDstStageMask = 0;
		submit_info.commandBufferCount = 1;
		submit_info.pCommandBuffers = &s_vulkan_renderer_compute_command_buffer;

		vkQueueSubmit(g_vulkan_instance_compute_queue, 1, &submit_info, 0);
	}

	{
		VkSemaphore wait_semaphores[] = { s_vulkan_renderer_compute_complete_semaphore, s_vulkan_renderer_image_available_semaphore };
		VkSemaphore signal_semaphores[] = { s_vulkan_renderer_graphic_complete_semaphore };
		VkPipelineStageFlags wait_stages[] = { VK_PIPELINE_STAGE_VERTEX_INPUT_BIT, VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT };

		VkSubmitInfo submit_info;
		memset(&submit_info, 0, sizeof(VkSubmitInfo));

		submit_info.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
		submit_info.pWaitSemaphores = wait_semaphores;
		submit_info.waitSemaphoreCount = (uint32_t)ARRAY_COUNT(wait_semaphores);
		submit_info.pSignalSemaphores = signal_semaphores;
		submit_info.signalSemaphoreCount = (uint32_t)ARRAY_COUNT(signal_semaphores);
		submit_info.pWaitDstStageMask = wait_stages;
		submit_info.commandBufferCount = 1;
		submit_info.pCommandBuffers = &s_vulkan_renderer_graphic_command_buffer;

		vkQueueSubmit(g_vulkan_instance_graphic_queue, 1, &submit_info, s_vulkan_renderer_render_fence);
	}

	vkWaitForFences(g_vulkan_instance_device, 1, &s_vulkan_renderer_render_fence, 1, UINT64_MAX);

	{
		VkSemaphore wait_semaphores[] = { s_vulkan_renderer_graphic_complete_semaphore };
		VkSwapchainKHR swap_chains[] = { g_vulkan_swap_chain };

		VkPresentInfoKHR present_info;
		memset(&present_info, 0, sizeof(VkPresentInfoKHR));

		present_info.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
		present_info.pWaitSemaphores = wait_semaphores;
		present_info.waitSemaphoreCount = (uint32_t)ARRAY_COUNT(wait_semaphores);
		present_info.pSwapchains = swap_chains;
		present_info.swapchainCount = (uint32_t)ARRAY_COUNT(swap_chains);
		present_info.pImageIndices = &image_index;

		vkQueuePresentKHR(g_vulkan_instance_present_queue, &present_info);
	}

	TRACY_ZONE_END
}
void vulkan_renderer_free(void)
{
	TRACY_ZONE_BEGIN

	std_ecs_query_free(&s_vulkan_renderer_render_query);
	std_ecs_query_free(&s_vulkan_renderer_camera_query);

	vulkan_pipeline_free(s_vulkan_renderer_pbr_pipeline);
	vulkan_pipeline_layout_free(s_vulkan_renderer_pbr_pipeline_layout);
	vulkan_pipeline_descriptor_set_layout_free(s_vulkan_renderer_pbr_descriptor_set_layout);
	vulkan_pipeline_descriptor_pool_free(s_vulkan_renderer_pbr_descriptor_pool);

	vulkan_renderer_frame_buffer_free();
	vulkan_renderer_render_pass_free();
	vulkan_renderer_sync_objects_free();
	vulkan_renderer_command_buffer_free();

	vulkan_buffer_free(&s_vulkan_renderer_projection_info_buffer);
	vulkan_buffer_free(&s_vulkan_renderer_screen_info_buffer);
	vulkan_buffer_free(&s_vulkan_renderer_time_info_buffer);

	std_vector_free(&s_vulkan_renderer_pbr_descriptor_sets);
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
			*(VkImageView*)std_vector_get(&g_vulkan_swap_chain_image_views, swap_chain_image_index),
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

		vkCreateFramebuffer(g_vulkan_instance_device, &frame_buffer_create_info, 0, std_vector_get(&s_vulkan_renderer_frame_buffers, swap_chain_image_index));

		swap_chain_image_index++;
	}

	TRACY_ZONE_END
}
void vulkan_renderer_update_uniform_buffers(void)
{
	TRACY_ZONE_BEGIN

	time_info_t* time_info = (time_info_t*)s_vulkan_renderer_time_info_buffer.mapped_buffer;

	time_info->time = g_platform_window_time;
	time_info->delta_time = g_platform_window_delta_time;

	screen_info_t* screen_info = (screen_info_t*)s_vulkan_renderer_screen_info_buffer.mapped_buffer;

	screen_info->size.x = (double)g_platform_window_width;
	screen_info->size.x = (double)g_platform_window_height;

	scene_t* scene = scene_current();

	if (scene)
	{
		std_ecs_query(&scene->ecs, &s_vulkan_renderer_camera_query);
		std_ecs_for(&scene->ecs, &s_vulkan_renderer_camera_query, vulkan_renderer_update_projection_info_proc);
	}

	TRACY_ZONE_END
}
void vulkan_renderer_build_pbr_descriptor_sets(uint64_t descriptor_count)
{
	TRACY_ZONE_BEGIN

	std_vector_resize(&s_vulkan_renderer_pbr_descriptor_sets, descriptor_count);

	vector_t descriptor_set_layouts = std_vector_alloc(sizeof(VkDescriptorSetLayout));

	std_vector_resize(&descriptor_set_layouts, descriptor_count);

	uint64_t descriptor_index = 0;
	while (descriptor_index < descriptor_count)
	{
		*(VkDescriptorSetLayout*)std_vector_get(&descriptor_set_layouts, descriptor_index) = s_vulkan_renderer_pbr_descriptor_set_layout;

		descriptor_index++;
	}

	VkDescriptorSetAllocateInfo descriptor_set_allocate_info;
	memset(&descriptor_set_allocate_info, 0, sizeof(VkDescriptorSetAllocateInfo));

	descriptor_set_allocate_info.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
	descriptor_set_allocate_info.descriptorSetCount = (uint32_t)descriptor_count;
	descriptor_set_allocate_info.descriptorPool = s_vulkan_renderer_pbr_descriptor_pool;
	descriptor_set_allocate_info.pSetLayouts = std_vector_buffer(&descriptor_set_layouts);

	vkAllocateDescriptorSets(g_vulkan_instance_device, &descriptor_set_allocate_info, std_vector_buffer(&s_vulkan_renderer_pbr_descriptor_sets));

	std_vector_free(&descriptor_set_layouts);

	TRACY_ZONE_END
}
void vulkan_renderer_update_pbr_descriptor_sets(void)
{
	TRACY_ZONE_BEGIN

	s_vulkan_renderer_pbr_descriptor_set_index = 0;

	scene_t* scene = scene_current();

	if (scene)
	{
		std_ecs_query(&scene->ecs, &s_vulkan_renderer_render_query);
		std_ecs_for(&scene->ecs, &s_vulkan_renderer_render_query, vulkan_renderer_update_pbr_descriptor_sets_proc);
	}

	TRACY_ZONE_END
}
void vulkan_renderer_record_compute_command_buffer(void)
{
	TRACY_ZONE_BEGIN

	vkQueueWaitIdle(g_vulkan_instance_compute_queue); // TODO

	VkCommandBufferBeginInfo command_buffer_begin_info;
	memset(&command_buffer_begin_info, 0, sizeof(VkCommandBufferBeginInfo));

	command_buffer_begin_info.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
	command_buffer_begin_info.flags = 0;
	command_buffer_begin_info.pInheritanceInfo = 0;

	vkBeginCommandBuffer(s_vulkan_renderer_compute_command_buffer, &command_buffer_begin_info);

	scene_t* scene = scene_current();

	if (scene)
	{
		/* // TODO: proper compute queue syncronization with graphics queue..
		Scene->DispatchTransformHierarchy();

		for (auto const& [name, model] : Scene->GetModels())
		{
			Skeleton* skeleton = model->GetSkeleton();

			skeleton->DispatchBoneHierarchy();
		}
		*/
	}

	vkEndCommandBuffer(s_vulkan_renderer_compute_command_buffer);

	TRACY_ZONE_END
}
void vulkan_renderer_record_graphic_command_buffer(uint32_t image_index)
{
	TRACY_ZONE_BEGIN

	vkQueueWaitIdle(g_vulkan_instance_graphic_queue); // TODO

	VkCommandBufferBeginInfo command_buffer_begin_info;
	memset(&command_buffer_begin_info, 0, sizeof(VkCommandBufferBeginInfo));

	command_buffer_begin_info.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
	command_buffer_begin_info.flags = 0;
	command_buffer_begin_info.pInheritanceInfo = 0;

	vkBeginCommandBuffer(s_vulkan_renderer_graphic_command_buffer, &command_buffer_begin_info);

	VkClearValue color_clear_value;
	memset(&color_clear_value, 0, sizeof(VkClearValue));

	color_clear_value.color.float32[0] = 0.0F;
	color_clear_value.color.float32[1] = 0.0F;
	color_clear_value.color.float32[2] = 0.0F;
	color_clear_value.color.float32[3] = 1.0F;

	VkClearValue depth_clear_value;
	memset(&depth_clear_value, 0, sizeof(VkClearValue));

	depth_clear_value.depthStencil.depth = 1.0F;
	depth_clear_value.depthStencil.stencil = 0;

	VkClearValue clear_values[] = { color_clear_value, depth_clear_value };

	VkRenderPassBeginInfo render_pass_create_info;
	memset(&render_pass_create_info, 0, sizeof(VkRenderPassBeginInfo));

	render_pass_create_info.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
	render_pass_create_info.renderPass = s_vulkan_renderer_render_pass;
	render_pass_create_info.framebuffer = *(VkFramebuffer*)std_vector_get(&s_vulkan_renderer_frame_buffers, image_index);
	render_pass_create_info.renderArea.offset.x = 0;
	render_pass_create_info.renderArea.offset.y = 0;
	render_pass_create_info.renderArea.extent.width = g_vulkan_instance_surface_capabilities.currentExtent.width;
	render_pass_create_info.renderArea.extent.height = g_vulkan_instance_surface_capabilities.currentExtent.height;
	render_pass_create_info.pClearValues = clear_values;
	render_pass_create_info.clearValueCount = (uint32_t)ARRAY_COUNT(clear_values);

	VkViewport viewport;
	memset(&viewport, 0, sizeof(VkViewport));

	viewport.x = 0.0F;
	viewport.y = 0.0F;
	viewport.width = (float)g_vulkan_instance_surface_capabilities.currentExtent.width;
	viewport.height = (float)g_vulkan_instance_surface_capabilities.currentExtent.height;
	viewport.minDepth = 0.0F;
	viewport.maxDepth = 1.0F;

	VkRect2D scissor;
	memset(&scissor, 0, sizeof(VkRect2D));

	scissor.offset.x = 0;
	scissor.offset.y = 0;
	scissor.extent.width = g_vulkan_instance_surface_capabilities.currentExtent.width;
	scissor.extent.height = g_vulkan_instance_surface_capabilities.currentExtent.height;

	vkCmdBeginRenderPass(s_vulkan_renderer_graphic_command_buffer, &render_pass_create_info, VK_SUBPASS_CONTENTS_INLINE);

	vkCmdSetViewport(s_vulkan_renderer_graphic_command_buffer, 0, 1, &viewport);
	vkCmdSetScissor(s_vulkan_renderer_graphic_command_buffer, 0, 1, &scissor);

	vkCmdBindPipeline(s_vulkan_renderer_graphic_command_buffer, VK_PIPELINE_BIND_POINT_GRAPHICS, s_vulkan_renderer_pbr_pipeline);

	s_vulkan_renderer_pbr_descriptor_set_index = 0;

	scene_t* scene = scene_current();

	if (scene)
	{
		std_ecs_query(&scene->ecs, &s_vulkan_renderer_render_query);
		std_ecs_for(&scene->ecs, &s_vulkan_renderer_render_query, vulkan_renderer_pbr_render_proc);
	}

	vkCmdEndRenderPass(s_vulkan_renderer_graphic_command_buffer);

	vkEndCommandBuffer(s_vulkan_renderer_graphic_command_buffer);

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

	vkQueueWaitIdle(g_vulkan_instance_graphic_queue); // TODO
	vkQueueWaitIdle(g_vulkan_instance_compute_queue); // TODO
	vkQueueWaitIdle(g_vulkan_instance_present_queue); // TODO

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
		vkDestroyFramebuffer(g_vulkan_instance_device, *(VkFramebuffer*)std_vector_get(&s_vulkan_renderer_frame_buffers, swap_chain_image_index), 0);

		swap_chain_image_index++;
	}

	TRACY_ZONE_END
}
static void vulkan_renderer_update_projection_info_proc(ecs_t* ecs, uint64_t entity, vector_t* view)
{
	TRACY_ZONE_BEGIN

	transform_t* transform = (transform_t*)std_ecs_value(ecs, entity, VEGA_COMPONENT_TYPE_TRANSFORM, view);
	camera_t* camera = (camera_t*)std_ecs_value(ecs, entity, VEGA_COMPONENT_TYPE_CAMERA, view);

	switch (camera->mode)
	{
		case CAMERA_MODE_ORTHO:
		{
			// TODO

			break;
		}
		case CAMERA_MODE_PERSP:
		{
			projection_info_t* projection_info = (projection_info_t*)s_vulkan_renderer_projection_info_buffer.mapped_buffer;

			vector3_t eye = transform->world_position;
			vector3_t center = math_vector3_add(transform->world_position, transform->local_front);
			vector3_t up = g_world_up;

			projection_info->view = math_matrix4_look_at(eye, center, up);

			double fov = camera->fov;
			double aspect_ratio = (double)g_vulkan_instance_surface_capabilities.currentExtent.width / (double)g_vulkan_instance_surface_capabilities.currentExtent.height;
			double near_z = camera->near_z;
			double far_z = camera->far_z;

			projection_info->projection = math_matrix4_persp(fov, aspect_ratio, near_z, far_z);

			break;
		}
	}

	TRACY_ZONE_END
}
static void vulkan_renderer_update_pbr_descriptor_sets_proc(ecs_t* ecs, uint64_t entity, vector_t* view)
{
	TRACY_ZONE_BEGIN

	renderable_t* renderable = (renderable_t*)std_ecs_value(ecs, entity, VEGA_COMPONENT_TYPE_RENDERABLE, view);

	// TODO: support more workflows..

	switch (renderable->material_type)
	{
		case MATERIAL_TYPE_PBR:
		{
			pbr_material_t* pbr_material = (pbr_material_t*)renderable->material;

			image_t* base_color = pbr_material->base_color;

			VkDescriptorBufferInfo time_info_descriptor_buffer_info = { 0 };
			time_info_descriptor_buffer_info.offset = 0;
			time_info_descriptor_buffer_info.buffer = s_vulkan_renderer_time_info_buffer.buffer;
			time_info_descriptor_buffer_info.range = s_vulkan_renderer_time_info_buffer.size;

			VkDescriptorBufferInfo projection_info_descriptor_buffer_info = { 0 };
			projection_info_descriptor_buffer_info.offset = 0;
			projection_info_descriptor_buffer_info.buffer = s_vulkan_renderer_projection_info_buffer.buffer;
			projection_info_descriptor_buffer_info.range = s_vulkan_renderer_projection_info_buffer.size;

			VkDescriptorImageInfo base_color_descriptor_image_info = { 0 };
			base_color_descriptor_image_info.imageLayout = VK_IMAGE_LAYOUT_GENERAL;
			base_color_descriptor_image_info.imageView = base_color->image_view;
			base_color_descriptor_image_info.sampler = base_color->sampler;

			VkDescriptorSet descriptor_set = *(VkDescriptorSet*)std_vector_get(&s_vulkan_renderer_pbr_descriptor_sets, s_vulkan_renderer_pbr_descriptor_set_index);

			VkWriteDescriptorSet write_descriptor_sets[3] = { 0 };

			write_descriptor_sets[0].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
			write_descriptor_sets[0].pNext = 0;
			write_descriptor_sets[0].dstSet = descriptor_set;
			write_descriptor_sets[0].dstBinding = 0;
			write_descriptor_sets[0].dstArrayElement = 0;
			write_descriptor_sets[0].descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
			write_descriptor_sets[0].descriptorCount = 1;
			write_descriptor_sets[0].pImageInfo = 0;
			write_descriptor_sets[0].pBufferInfo = &time_info_descriptor_buffer_info;
			write_descriptor_sets[0].pTexelBufferView = 0;

			write_descriptor_sets[1].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
			write_descriptor_sets[1].pNext = 0;
			write_descriptor_sets[1].dstSet = descriptor_set;
			write_descriptor_sets[1].dstBinding = 1;
			write_descriptor_sets[1].dstArrayElement = 0;
			write_descriptor_sets[1].descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
			write_descriptor_sets[1].descriptorCount = 1;
			write_descriptor_sets[1].pImageInfo = 0;
			write_descriptor_sets[1].pBufferInfo = &projection_info_descriptor_buffer_info;
			write_descriptor_sets[1].pTexelBufferView = 0;

			write_descriptor_sets[2].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
			write_descriptor_sets[2].pNext = 0;
			write_descriptor_sets[2].dstSet = descriptor_set;
			write_descriptor_sets[2].dstBinding = 2;
			write_descriptor_sets[2].dstArrayElement = 0;
			write_descriptor_sets[2].descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
			write_descriptor_sets[2].descriptorCount = 1;
			write_descriptor_sets[2].pImageInfo = &base_color_descriptor_image_info;
			write_descriptor_sets[2].pBufferInfo = 0;
			write_descriptor_sets[2].pTexelBufferView = 0;

			vkUpdateDescriptorSets(g_vulkan_instance_device, (uint32_t)ARRAY_COUNT(write_descriptor_sets), write_descriptor_sets, 0, 0);

			break;
		}
	}

	s_vulkan_renderer_pbr_descriptor_set_index++;

	TRACY_ZONE_END
}
static void vulkan_renderer_pbr_render_proc(ecs_t* ecs, uint64_t entity, vector_t* view)
{
	TRACY_ZONE_BEGIN

	renderable_t* renderable = (renderable_t*)std_ecs_value(ecs, entity, VEGA_COMPONENT_TYPE_RENDERABLE, view);

	buffer_t* vertex_buffer = renderable->mesh->vertex_buffer;
	buffer_t* index_buffer = renderable->mesh->index_buffer;

	VkDescriptorSet descriptor_set = *(VkDescriptorSet*)std_vector_get(&s_vulkan_renderer_pbr_descriptor_sets, s_vulkan_renderer_pbr_descriptor_set_index);

	VkBuffer default_vertex_buffers[] = { vertex_buffer->buffer };
	uint64_t default_offsets[] = { 0 };

	vkCmdBindDescriptorSets(s_vulkan_renderer_graphic_command_buffer, VK_PIPELINE_BIND_POINT_GRAPHICS, s_vulkan_renderer_pbr_pipeline_layout, 0, 1, &descriptor_set, 0, 0);
	vkCmdBindVertexBuffers(s_vulkan_renderer_graphic_command_buffer, VEGA_PBR_VERTEX_BINDING_ID, 1, default_vertex_buffers, default_offsets);
	vkCmdBindIndexBuffer(s_vulkan_renderer_graphic_command_buffer, index_buffer->buffer, 0, VK_INDEX_TYPE_UINT32);

	vkCmdDrawIndexed(s_vulkan_renderer_graphic_command_buffer, (uint32_t)(index_buffer->size / sizeof(uint32_t)), 1, 0, 0, 0);

	s_vulkan_renderer_pbr_descriptor_set_index++;

	TRACY_ZONE_END
}
