#include <string.h>

#include <vega/engine/scene.h>

#include <vega/engine/component/camera.h>
#include <vega/engine/component/renderable.h>
#include <vega/engine/component/transform.h>
#include <vega/engine/component/types.h>

#include <vega/engine/imgui/imgui.h>
#include <vega/engine/imgui/imgui_impl_win32.h>
#include <vega/engine/imgui/imgui_impl_vulkan.h>

#include <vega/engine/platform/window.h>

#include <vega/engine/vulkan/buffer.h>
#include <vega/engine/vulkan/command_pool.h>
#include <vega/engine/vulkan/device.h>
#include <vega/engine/vulkan/instance.h>
#include <vega/engine/vulkan/pipeline.h>
#include <vega/engine/vulkan/renderer.h>
#include <vega/engine/vulkan/surface.h>
#include <vega/engine/vulkan/swap_chain.h>
#include <vega/engine/vulkan/vertex.h>

#ifndef TRACY_ZONE_BEGIN
	#define TRACY_ZONE_BEGIN TracyCZoneC(ctx, TRACY_COLOR_GREEN, 1U);
#endif // TRACY_ZONE_BEGIN

#ifndef TRACY_ZONE_END
	#define TRACY_ZONE_END TracyCZoneEnd(ctx);
#endif // TRACY_ZONE_END

#ifndef VEGA_FRAMES_IN_FLIGHT
	#define VEGA_FRAMES_IN_FLIGHT (2ULL)
#endif // VEGA_FRAMES_IN_FLIGHT

#ifndef VEGA_PBR_VERTEX_BINDING_ID
	#define VEGA_PBR_VERTEX_BINDING_ID (0U)
#endif // VEGA_PBR_VERTEX_BINDING_ID

static void vulkan_renderer_update_camera_info_proc(ecs_t* ecs, uint64_t index, uint64_t entity);
static void vulkan_renderer_update_pbr_descriptor_sets_proc(ecs_t* ecs, uint64_t index, uint64_t entity);
static void vulkan_renderer_build_geometry_queues_proc(ecs_t* ecs, uint64_t index, uint64_t entity);

static buffer_t s_vulkan_renderer_time_info_buffer = {};
static buffer_t s_vulkan_renderer_screen_info_buffer = {};
static buffer_t s_vulkan_renderer_camera_info_buffer = {};
static buffer_t s_vulkan_renderer_transform_buffer = {};

static VkCommandBuffer s_vulkan_renderer_compute_command_buffers[VEGA_FRAMES_IN_FLIGHT] = {};
static VkCommandBuffer s_vulkan_renderer_geometry_command_buffers[VEGA_FRAMES_IN_FLIGHT] = {};

static VkSemaphore s_vulkan_renderer_compute_complete_semaphores[VEGA_FRAMES_IN_FLIGHT] = {};
static VkSemaphore s_vulkan_renderer_graphic_complete_semaphores[VEGA_FRAMES_IN_FLIGHT] = {};
static VkSemaphore s_vulkan_renderer_image_available_semaphores[VEGA_FRAMES_IN_FLIGHT] = {};

static VkFence s_vulkan_renderer_render_fences[VEGA_FRAMES_IN_FLIGHT] = {};

static VkRenderPass s_vulkan_renderer_render_pass = 0;

static vector_t s_vulkan_renderer_frame_buffers = {};

static uint32_t s_vulkan_renderer_frame_index = 0;
static uint32_t s_vulkan_renderer_image_index = 0;

static VkDescriptorSetLayoutBinding s_vulkan_renderer_pbr_descriptor_set_layout_bindings[] =
{
	{ 0, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, 1, VK_SHADER_STAGE_VERTEX_BIT, 0 },
	{ 1, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, 1, VK_SHADER_STAGE_VERTEX_BIT, 0 },
	{ 2, VK_DESCRIPTOR_TYPE_STORAGE_BUFFER, 1, VK_SHADER_STAGE_VERTEX_BIT, 0 },
	{ 3, VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, 1, VK_SHADER_STAGE_FRAGMENT_BIT, 0 },
};

static VkPushConstantRange s_vulkan_renderer_pbr_push_constant_ranges[] =
{
	{ VK_SHADER_STAGE_VERTEX_BIT, 0, sizeof(uint32_t) },
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

static char const* s_vulkan_renderer_pbr_vertex_shader_file_path = VEGA_ROOT_DIR "/shader/pbr.vert.spv";
static char const* s_vulkan_renderer_pbr_fragment_shader_file_path = VEGA_ROOT_DIR "/shader/pbr.frag.spv";

static VkDescriptorPool s_vulkan_renderer_pbr_descriptor_pool = 0;
static VkDescriptorSetLayout s_vulkan_renderer_pbr_descriptor_set_layout = 0;

static vector_t s_vulkan_renderer_pbr_descriptor_sets = {};

static VkPipelineLayout s_vulkan_renderer_pbr_pipeline_layout = 0;
static VkPipeline s_vulkan_renderer_pbr_pipeline = 0;

static map_t s_vulkan_renderer_geometry_passes = {};

// TODO: create entity filters for materials different then the pbr workflow..

static ecs_query_t s_vulkan_renderer_camera_query =
{
	.mask = VEGA_COMPONENT_BIT_TRANSFORM | VEGA_COMPONENT_BIT_CAMERA,
	.proc = vulkan_renderer_update_camera_info_proc,
};
static ecs_query_t s_vulkan_renderer_update_descriptor_sets_query =
{
	.mask = VEGA_COMPONENT_BIT_TRANSFORM | VEGA_COMPONENT_BIT_RENDERABLE,
	.proc = vulkan_renderer_update_pbr_descriptor_sets_proc,
};
static ecs_query_t s_vulkan_renderer_build_geometry_queues_query =
{
	.mask = VEGA_COMPONENT_BIT_TRANSFORM | VEGA_COMPONENT_BIT_RENDERABLE,
	.proc = vulkan_renderer_build_geometry_queues_proc,
};

void vulkan_renderer_alloc(void)
{
	TRACY_ZONE_BEGIN

	s_vulkan_renderer_geometry_passes = std_map_alloc();

	s_vulkan_renderer_frame_buffers = std_vector_alloc(sizeof(VkFramebuffer));
	s_vulkan_renderer_pbr_descriptor_sets = std_vector_alloc(sizeof(VkDescriptorSet));

	s_vulkan_renderer_time_info_buffer = vulkan_buffer_uniform_coherent_alloc(sizeof(time_info_t));
	s_vulkan_renderer_screen_info_buffer = vulkan_buffer_uniform_coherent_alloc(sizeof(screen_info_t));
	s_vulkan_renderer_camera_info_buffer = vulkan_buffer_uniform_coherent_alloc(sizeof(camera_info_t));
	s_vulkan_renderer_transform_buffer = vulkan_buffer_storage_coherent_alloc(sizeof(matrix4_t) * 10000);

	vulkan_renderer_command_buffer_alloc();
	vulkan_renderer_sync_objects_alloc();
	vulkan_renderer_render_pass_alloc();
	vulkan_renderer_frame_buffer_alloc();
	vulkan_renderer_imgui_context_alloc();

	// TODO: adjust pool count somehow to entity count..
	s_vulkan_renderer_pbr_descriptor_pool = vulkan_pipeline_descriptor_pool_alloc(10000, s_vulkan_renderer_pbr_descriptor_set_layout_bindings, VEGA_ARRAY_COUNT(s_vulkan_renderer_pbr_descriptor_set_layout_bindings));
	s_vulkan_renderer_pbr_descriptor_set_layout = vulkan_pipeline_descriptor_set_layout_alloc(s_vulkan_renderer_pbr_descriptor_set_layout_bindings, VEGA_ARRAY_COUNT(s_vulkan_renderer_pbr_descriptor_set_layout_bindings));
	s_vulkan_renderer_pbr_pipeline_layout = vulkan_pipeline_layout_alloc(s_vulkan_renderer_pbr_descriptor_set_layout, s_vulkan_renderer_pbr_push_constant_ranges, VEGA_ARRAY_COUNT(s_vulkan_renderer_pbr_push_constant_ranges));
	s_vulkan_renderer_pbr_pipeline = vulkan_pipeline_graphic_alloc(s_vulkan_renderer_pbr_pipeline_layout, s_vulkan_renderer_render_pass, VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST, s_vulkan_renderer_pbr_vertex_shader_file_path, s_vulkan_renderer_pbr_fragment_shader_file_path, s_vulkan_renderer_pbr_vertex_input_binding_descriptions, VEGA_ARRAY_COUNT(s_vulkan_renderer_pbr_vertex_input_binding_descriptions), s_vulkan_renderer_pbr_vertex_input_attribute_descriptions, VEGA_ARRAY_COUNT(s_vulkan_renderer_pbr_vertex_input_attribute_descriptions));

	vulkan_renderer_build_pbr_descriptor_sets(10000); // TODO: put this somewhere else..

	TRACY_ZONE_END
}
void vulkan_renderer_free(void)
{
	TRACY_ZONE_BEGIN

	vulkan_pipeline_free(s_vulkan_renderer_pbr_pipeline);
	vulkan_pipeline_layout_free(s_vulkan_renderer_pbr_pipeline_layout);
	vulkan_pipeline_descriptor_set_layout_free(s_vulkan_renderer_pbr_descriptor_set_layout);
	vulkan_pipeline_descriptor_pool_free(s_vulkan_renderer_pbr_descriptor_pool);

	vulkan_renderer_imgui_context_free();
	vulkan_renderer_frame_buffer_free();
	vulkan_renderer_render_pass_free();
	vulkan_renderer_sync_objects_free();
	vulkan_renderer_command_buffer_free();

	vulkan_buffer_free(&s_vulkan_renderer_transform_buffer);
	vulkan_buffer_free(&s_vulkan_renderer_camera_info_buffer);
	vulkan_buffer_free(&s_vulkan_renderer_screen_info_buffer);
	vulkan_buffer_free(&s_vulkan_renderer_time_info_buffer);

	std_vector_free(&s_vulkan_renderer_pbr_descriptor_sets);
	std_vector_free(&s_vulkan_renderer_frame_buffers);

	std_map_free(&s_vulkan_renderer_geometry_passes);

	TRACY_ZONE_END
}
void vulkan_renderer_begin(void)
{
	TRACY_ZONE_BEGIN

	vkResetFences(g_vulkan_device, 1, &s_vulkan_renderer_render_fences[s_vulkan_renderer_frame_index]);

	vkAcquireNextImageKHR(g_vulkan_device, g_vulkan_swap_chain, UINT64_MAX, s_vulkan_renderer_image_available_semaphores[s_vulkan_renderer_frame_index], 0, &s_vulkan_renderer_image_index);

	vulkan_renderer_update_uniform_buffers();

	TRACY_ZONE_END
}
void vulkan_renderer_end(void)
{
	TRACY_ZONE_BEGIN

	{
		VkSemaphore signal_semaphores[] = { s_vulkan_renderer_compute_complete_semaphores[s_vulkan_renderer_frame_index] };
		VkCommandBuffer command_buffers[] = { s_vulkan_renderer_compute_command_buffers[s_vulkan_renderer_frame_index] };

		VkSubmitInfo submit_info = {};
		submit_info.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
		submit_info.pWaitSemaphores = 0;
		submit_info.waitSemaphoreCount = 0;
		submit_info.pSignalSemaphores = signal_semaphores;
		submit_info.signalSemaphoreCount = (uint32_t)VEGA_ARRAY_COUNT(signal_semaphores);
		submit_info.pWaitDstStageMask = 0;
		submit_info.pCommandBuffers = command_buffers;
		submit_info.commandBufferCount = (uint32_t)VEGA_ARRAY_COUNT(command_buffers);

		vkQueueSubmit(g_vulkan_device_compute_queue, 1, &submit_info, 0);
	}

	{
		VkSemaphore wait_semaphores[] = { s_vulkan_renderer_compute_complete_semaphores[s_vulkan_renderer_frame_index], s_vulkan_renderer_image_available_semaphores[s_vulkan_renderer_frame_index] };
		VkSemaphore signal_semaphores[] = { s_vulkan_renderer_graphic_complete_semaphores[s_vulkan_renderer_frame_index] };
		VkPipelineStageFlags wait_stages[] = { VK_PIPELINE_STAGE_VERTEX_INPUT_BIT, VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT };
		VkCommandBuffer command_buffers[] = { s_vulkan_renderer_geometry_command_buffers[s_vulkan_renderer_frame_index] };

		VkSubmitInfo submit_info = {};
		submit_info.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
		submit_info.pWaitSemaphores = wait_semaphores;
		submit_info.waitSemaphoreCount = (uint32_t)VEGA_ARRAY_COUNT(wait_semaphores);
		submit_info.pSignalSemaphores = signal_semaphores;
		submit_info.signalSemaphoreCount = (uint32_t)VEGA_ARRAY_COUNT(signal_semaphores);
		submit_info.pWaitDstStageMask = wait_stages;
		submit_info.pCommandBuffers = command_buffers;
		submit_info.commandBufferCount = (uint32_t)VEGA_ARRAY_COUNT(command_buffers);

		vkQueueSubmit(g_vulkan_device_graphic_queue, 1, &submit_info, s_vulkan_renderer_render_fences[s_vulkan_renderer_frame_index]);
	}

	vkWaitForFences(g_vulkan_device, 1, &s_vulkan_renderer_render_fences[s_vulkan_renderer_frame_index], 1, UINT64_MAX);

	{
		VkSemaphore wait_semaphores[] = { s_vulkan_renderer_graphic_complete_semaphores[s_vulkan_renderer_frame_index] };
		VkSwapchainKHR swap_chains[] = { g_vulkan_swap_chain };

		VkPresentInfoKHR present_info = {};
		present_info.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
		present_info.pWaitSemaphores = wait_semaphores;
		present_info.waitSemaphoreCount = (uint32_t)VEGA_ARRAY_COUNT(wait_semaphores);
		present_info.pSwapchains = swap_chains;
		present_info.swapchainCount = (uint32_t)VEGA_ARRAY_COUNT(swap_chains);
		present_info.pImageIndices = &s_vulkan_renderer_image_index;

		vkQueuePresentKHR(g_vulkan_device_present_queue, &present_info);
	}

	s_vulkan_renderer_frame_index = (s_vulkan_renderer_frame_index + 1) % VEGA_FRAMES_IN_FLIGHT;

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

	VkDescriptorSetAllocateInfo descriptor_set_allocate_info = {};
	descriptor_set_allocate_info.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
	descriptor_set_allocate_info.descriptorSetCount = (uint32_t)descriptor_count;
	descriptor_set_allocate_info.descriptorPool = s_vulkan_renderer_pbr_descriptor_pool;
	descriptor_set_allocate_info.pSetLayouts = (VkDescriptorSetLayout*)std_vector_buffer(&descriptor_set_layouts);

	vkAllocateDescriptorSets(g_vulkan_device, &descriptor_set_allocate_info, (VkDescriptorSet*)std_vector_buffer(&s_vulkan_renderer_pbr_descriptor_sets));

	std_vector_free(&descriptor_set_layouts);

	TRACY_ZONE_END
}
void vulkan_renderer_update_pbr_descriptor_sets(void)
{
	TRACY_ZONE_BEGIN

	scene_t* scene = scene_current();

	if (scene)
	{
		std_ecs_query(&scene->ecs, &s_vulkan_renderer_update_descriptor_sets_query);
		std_ecs_for(&scene->ecs, &s_vulkan_renderer_update_descriptor_sets_query);
	}

	TRACY_ZONE_END
}
void vulkan_renderer_build_geometry_queues(void)
{
	TRACY_ZONE_BEGIN

	scene_t* scene = scene_current();

	if (scene)
	{
		std_ecs_query(&scene->ecs, &s_vulkan_renderer_build_geometry_queues_query);
		std_ecs_for(&scene->ecs, &s_vulkan_renderer_build_geometry_queues_query);
	}

	TRACY_ZONE_END
}
void vulkan_renderer_command_buffer_alloc(void)
{
	TRACY_ZONE_BEGIN

	VkCommandBufferAllocateInfo command_buffer_alloc_create_info = {};
	command_buffer_alloc_create_info.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
	command_buffer_alloc_create_info.commandPool = g_vulkan_command_pool;
	command_buffer_alloc_create_info.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
	command_buffer_alloc_create_info.commandBufferCount = VEGA_FRAMES_IN_FLIGHT;

	vkAllocateCommandBuffers(g_vulkan_device, &command_buffer_alloc_create_info, s_vulkan_renderer_compute_command_buffers);
	vkAllocateCommandBuffers(g_vulkan_device, &command_buffer_alloc_create_info, s_vulkan_renderer_geometry_command_buffers);

	TRACY_ZONE_END
}
void vulkan_renderer_sync_objects_alloc(void)
{
	TRACY_ZONE_BEGIN

	VkSemaphoreCreateInfo semaphore_create_info = {};
	semaphore_create_info.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;

	uint64_t semaphore_index = 0;
	while (semaphore_index < VEGA_FRAMES_IN_FLIGHT)
	{
		vkCreateSemaphore(g_vulkan_device, &semaphore_create_info, 0, &s_vulkan_renderer_compute_complete_semaphores[semaphore_index]);
		vkCreateSemaphore(g_vulkan_device, &semaphore_create_info, 0, &s_vulkan_renderer_graphic_complete_semaphores[semaphore_index]);
		vkCreateSemaphore(g_vulkan_device, &semaphore_create_info, 0, &s_vulkan_renderer_image_available_semaphores[semaphore_index]);

		semaphore_index++;
	}

	VkFenceCreateInfo fence_create_info = {};
	fence_create_info.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
	fence_create_info.flags = VK_FENCE_CREATE_SIGNALED_BIT;

	uint64_t fence_index = 0;
	while (fence_index < VEGA_FRAMES_IN_FLIGHT)
	{
		vkCreateFence(g_vulkan_device, &fence_create_info, 0, &s_vulkan_renderer_render_fences[fence_index]);

		fence_index++;
	}

	TRACY_ZONE_END
}
void vulkan_renderer_render_pass_alloc(void)
{
	TRACY_ZONE_BEGIN

	VkAttachmentDescription color_attachment_description = {};
	color_attachment_description.format = g_vulkan_surface_prefered_format.format;
	color_attachment_description.samples = VK_SAMPLE_COUNT_1_BIT;
	color_attachment_description.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
	color_attachment_description.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
	color_attachment_description.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
	color_attachment_description.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
	color_attachment_description.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
	color_attachment_description.finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;

	VkAttachmentDescription depth_attachment_description = {};
	depth_attachment_description.format = g_vulkan_swap_chain_depth_format;
	depth_attachment_description.samples = VK_SAMPLE_COUNT_1_BIT;
	depth_attachment_description.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
	depth_attachment_description.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
	depth_attachment_description.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
	depth_attachment_description.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
	depth_attachment_description.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
	depth_attachment_description.finalLayout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;

	VkAttachmentReference color_attachment_reference = {};
	color_attachment_reference.attachment = 0;
	color_attachment_reference.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

	VkAttachmentReference depth_attachment_reference = {};
	depth_attachment_reference.attachment = 1;
	depth_attachment_reference.layout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;

	VkSubpassDescription subpass_description = {};
	subpass_description.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
	subpass_description.colorAttachmentCount = 1;
	subpass_description.pColorAttachments = &color_attachment_reference;
	subpass_description.pDepthStencilAttachment = &depth_attachment_reference;

	VkSubpassDependency subpass_dependency = {};
	subpass_dependency.srcSubpass = VK_SUBPASS_EXTERNAL;
	subpass_dependency.dstSubpass = 0;
	subpass_dependency.srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT | VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT;
	subpass_dependency.srcAccessMask = 0;
	subpass_dependency.dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT | VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT;
	subpass_dependency.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT | VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT;

	VkAttachmentDescription attachment_descriptions[] = { color_attachment_description, depth_attachment_description };

	VkRenderPassCreateInfo render_pass_create_info = {};
	render_pass_create_info.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
	render_pass_create_info.pAttachments = attachment_descriptions;
	render_pass_create_info.attachmentCount = (uint32_t)VEGA_ARRAY_COUNT(attachment_descriptions);
	render_pass_create_info.pSubpasses = &subpass_description;
	render_pass_create_info.subpassCount = 1;
	render_pass_create_info.pDependencies = &subpass_dependency;
	render_pass_create_info.dependencyCount = 1;

	vkCreateRenderPass(g_vulkan_device, &render_pass_create_info, 0, &s_vulkan_renderer_render_pass);

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

		VkFramebufferCreateInfo frame_buffer_create_info = {};
		frame_buffer_create_info.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
		frame_buffer_create_info.renderPass = s_vulkan_renderer_render_pass;
		frame_buffer_create_info.pAttachments = image_attachments;
		frame_buffer_create_info.attachmentCount = (uint32_t)VEGA_ARRAY_COUNT(image_attachments);
		frame_buffer_create_info.width = g_vulkan_surface_width;
		frame_buffer_create_info.height = g_vulkan_surface_height;
		frame_buffer_create_info.layers = 1;

		vkCreateFramebuffer(g_vulkan_device, &frame_buffer_create_info, 0, (VkFramebuffer*)std_vector_get(&s_vulkan_renderer_frame_buffers, swap_chain_image_index));

		swap_chain_image_index++;
	}

	TRACY_ZONE_END
}
void vulkan_renderer_imgui_context_alloc(void)
{
	TRACY_ZONE_BEGIN

	VkDescriptorPoolSize descriptor_pool_sizes[] =
	{
		{ VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, IMGUI_IMPL_VULKAN_MINIMUM_IMAGE_SAMPLER_POOL_SIZE },
	};

	VkDescriptorPoolCreateInfo descriptor_pool_create_info = {};
	descriptor_pool_create_info.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
	descriptor_pool_create_info.flags = VK_DESCRIPTOR_POOL_CREATE_FREE_DESCRIPTOR_SET_BIT;
	descriptor_pool_create_info.maxSets = 0;

	// TODO: refactor this out..
	for (VkDescriptorPoolSize& descriptor_pool_size : descriptor_pool_sizes)
	{
		descriptor_pool_create_info.maxSets += descriptor_pool_size.descriptorCount;
	}

	descriptor_pool_create_info.pPoolSizes = descriptor_pool_sizes;
	descriptor_pool_create_info.poolSizeCount = (uint32_t)VEGA_ARRAY_COUNT(descriptor_pool_sizes);

	VkDescriptorPool descriptor_pool = 0;

	vkCreateDescriptorPool(g_vulkan_device, &descriptor_pool_create_info, 0, &descriptor_pool);

	IMGUI_CHECKVERSION();

	ImGui::CreateContext();

	ImGuiIO& io = ImGui::GetIO();
	io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;
	io.ConfigWindowsMoveFromTitleBarOnly = 1;

	ImGui::StyleColorsDark();

	ImGui_ImplWin32_Init(g_platform_window_handle);

	ImGui_ImplVulkan_InitInfo imgui_vulkan_init_info = {};
	imgui_vulkan_init_info.Instance = g_vulkan_instance;
	imgui_vulkan_init_info.PhysicalDevice = g_vulkan_device_physical;
	imgui_vulkan_init_info.Device = g_vulkan_device;
	imgui_vulkan_init_info.QueueFamily = g_vulkan_device_graphic_queue_index;
	imgui_vulkan_init_info.Queue = g_vulkan_device_graphic_queue;
	imgui_vulkan_init_info.PipelineCache = 0;
	imgui_vulkan_init_info.DescriptorPool = descriptor_pool; // TODO
	imgui_vulkan_init_info.RenderPass = s_vulkan_renderer_render_pass;
	imgui_vulkan_init_info.Subpass = 0;
	imgui_vulkan_init_info.MinImageCount = g_vulkan_swap_chain_min_image_count;
	imgui_vulkan_init_info.ImageCount = g_vulkan_swap_chain_image_count;
	imgui_vulkan_init_info.MSAASamples = VK_SAMPLE_COUNT_1_BIT;
	imgui_vulkan_init_info.Allocator = 0;
	imgui_vulkan_init_info.CheckVkResultFn = 0;

	ImGui_ImplVulkan_Init(&imgui_vulkan_init_info);

	TRACY_ZONE_END
}
void vulkan_renderer_update_uniform_buffers(void)
{
	TRACY_ZONE_BEGIN

	time_info_t* time_info = (time_info_t*)s_vulkan_renderer_time_info_buffer.mapped_buffer;

	time_info->time = g_platform_window_time;
	time_info->delta_time = g_platform_window_delta_time;

	screen_info_t* screen_info = (screen_info_t*)s_vulkan_renderer_screen_info_buffer.mapped_buffer;

	screen_info->size.x = (double)g_vulkan_surface_width;
	screen_info->size.x = (double)g_vulkan_surface_height;

	scene_t* scene = scene_current();

	if (scene)
	{
		std_ecs_query(&scene->ecs, &s_vulkan_renderer_camera_query);
		std_ecs_for(&scene->ecs, &s_vulkan_renderer_camera_query);
	}

	TRACY_ZONE_END
}
void vulkan_renderer_compute_begin(void)
{
	TRACY_ZONE_BEGIN

	VkCommandBufferBeginInfo command_buffer_begin_info = {};
	command_buffer_begin_info.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
	command_buffer_begin_info.flags = 0;
	command_buffer_begin_info.pInheritanceInfo = 0;

	vkBeginCommandBuffer(s_vulkan_renderer_compute_command_buffers[s_vulkan_renderer_frame_index], &command_buffer_begin_info);

	TRACY_ZONE_END
}
void vulkan_renderer_compute_end(void)
{
	TRACY_ZONE_BEGIN

	vkEndCommandBuffer(s_vulkan_renderer_compute_command_buffers[s_vulkan_renderer_frame_index]);

	TRACY_ZONE_END
}
void vulkan_renderer_graphic_begin(void)
{
	TRACY_ZONE_BEGIN

	VkCommandBufferBeginInfo command_buffer_begin_info = {};
	command_buffer_begin_info.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
	command_buffer_begin_info.flags = 0;
	command_buffer_begin_info.pInheritanceInfo = 0;

	vkBeginCommandBuffer(s_vulkan_renderer_geometry_command_buffers[s_vulkan_renderer_frame_index], &command_buffer_begin_info);

	TRACY_ZONE_END
}
void vulkan_renderer_graphic_end(void)
{
	TRACY_ZONE_BEGIN

	vkEndCommandBuffer(s_vulkan_renderer_geometry_command_buffers[s_vulkan_renderer_frame_index]);

	TRACY_ZONE_END
}
void vulkan_renderer_geometry_pass_begin(void)
{
	TRACY_ZONE_BEGIN

	VkClearValue color_clear_value = {};
	color_clear_value.color.float32[0] = 0.0F;
	color_clear_value.color.float32[1] = 0.0F;
	color_clear_value.color.float32[2] = 0.0F;
	color_clear_value.color.float32[3] = 1.0F;

	VkClearValue depth_clear_value = {};
	depth_clear_value.depthStencil.depth = 1.0F;
	depth_clear_value.depthStencil.stencil = 0;

	VkClearValue clear_values[] = { color_clear_value, depth_clear_value };

	VkRenderPassBeginInfo render_pass_create_info = {};
	render_pass_create_info.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
	render_pass_create_info.renderPass = s_vulkan_renderer_render_pass;
	render_pass_create_info.framebuffer = *(VkFramebuffer*)std_vector_get(&s_vulkan_renderer_frame_buffers, s_vulkan_renderer_image_index);
	render_pass_create_info.renderArea.offset.x = 0;
	render_pass_create_info.renderArea.offset.y = 0;
	render_pass_create_info.renderArea.extent.width = g_vulkan_surface_width;
	render_pass_create_info.renderArea.extent.height = g_vulkan_surface_height;
	render_pass_create_info.pClearValues = clear_values;
	render_pass_create_info.clearValueCount = (uint32_t)VEGA_ARRAY_COUNT(clear_values);

	vkCmdBeginRenderPass(s_vulkan_renderer_geometry_command_buffers[s_vulkan_renderer_frame_index], &render_pass_create_info, VK_SUBPASS_CONTENTS_INLINE);

	VkViewport viewport = {};
	viewport.x = 0.0F;
	viewport.y = 0.0F;
	viewport.width = (float)g_vulkan_surface_width;
	viewport.height = (float)g_vulkan_surface_height;
	viewport.minDepth = 0.0F;
	viewport.maxDepth = 1.0F;

	vkCmdSetViewport(s_vulkan_renderer_geometry_command_buffers[s_vulkan_renderer_frame_index], 0, 1, &viewport);

	VkRect2D scissor = {};
	scissor.offset.x = 0;
	scissor.offset.y = 0;
	scissor.extent.width = g_vulkan_surface_width;
	scissor.extent.height = g_vulkan_surface_height;

	vkCmdSetScissor(s_vulkan_renderer_geometry_command_buffers[s_vulkan_renderer_frame_index], 0, 1, &scissor);

	vkCmdBindPipeline(s_vulkan_renderer_geometry_command_buffers[s_vulkan_renderer_frame_index], VK_PIPELINE_BIND_POINT_GRAPHICS, s_vulkan_renderer_pbr_pipeline);

	TRACY_ZONE_END
}
void vulkan_renderer_geometry_pass(void)
{
	TRACY_ZONE_BEGIN

	map_iter_t geometry_pass_iter = std_map_begin(&s_vulkan_renderer_geometry_passes);
	while (std_map_end(&geometry_pass_iter) == 0)
	{
		geometry_pass_t* geometry_pass = (geometry_pass_t*)std_map_value(&geometry_pass_iter, 0);

		buffer_t* vertex_buffer = &geometry_pass->mesh_asset->pbr_vertex_buffer;
		buffer_t* index_buffer = &geometry_pass->mesh_asset->pbr_index_buffer;

		VkDescriptorSet* descriptor_set = (VkDescriptorSet*)std_vector_get(&s_vulkan_renderer_pbr_descriptor_sets, 0); // TODO

		VkBuffer vertex_buffers[] = { vertex_buffer->buffer };
		uint64_t vertex_offsets[] = { 0 };

		// TODO: bind all descriptor sets at once..
		vkCmdBindDescriptorSets(s_vulkan_renderer_geometry_command_buffers[s_vulkan_renderer_frame_index], VK_PIPELINE_BIND_POINT_GRAPHICS, s_vulkan_renderer_pbr_pipeline_layout, 0, 1, descriptor_set, 0, 0);
		vkCmdBindVertexBuffers(s_vulkan_renderer_geometry_command_buffers[s_vulkan_renderer_frame_index], VEGA_PBR_VERTEX_BINDING_ID, 1, vertex_buffers, vertex_offsets);
		vkCmdBindIndexBuffer(s_vulkan_renderer_geometry_command_buffers[s_vulkan_renderer_frame_index], index_buffer->buffer, 0, VK_INDEX_TYPE_UINT32);

		uint64_t transform_index = 0;
		uint64_t transform_count = std_fvector64_count(&geometry_pass->transforms);
		while (transform_index < transform_count)
		{
			transform_t* transform = (transform_t*)std_fvector64_get(&geometry_pass->transforms, transform_index);

			*(((matrix4_t*)s_vulkan_renderer_transform_buffer.mapped_buffer) + transform_index) = transform_matrix(transform);

			vkCmdPushConstants(s_vulkan_renderer_geometry_command_buffers[s_vulkan_renderer_frame_index], s_vulkan_renderer_pbr_pipeline_layout, VK_SHADER_STAGE_VERTEX_BIT, 0, sizeof(uint32_t), &transform_index);

			uint32_t index_count = (uint32_t)(index_buffer->size / sizeof(uint32_t));
			uint32_t instance_count = 1;

			vkCmdDrawIndexed(s_vulkan_renderer_geometry_command_buffers[s_vulkan_renderer_frame_index], index_count, instance_count, 0, 0, 0);

			transform_index++;
		}
	}

	TRACY_ZONE_END
}
void vulkan_renderer_imgui_pass(void)
{
	TRACY_ZONE_BEGIN

	ImGui_ImplVulkan_NewFrame();

	ImGui_ImplWin32_NewFrame();

	ImGui::NewFrame();
	ImGui::DockSpaceOverViewport(0, 0, ImGuiDockNodeFlags_PassthruCentralNode);

	ImGui::Begin("Hello, Vulkan!");
	ImGui::Text("This is an ImGui window with Vulkan!");
	ImGui::End();

	ImGui::Render();

	ImDrawData* imgui_draw_data = ImGui::GetDrawData();

	ImGui_ImplVulkan_RenderDrawData(imgui_draw_data, s_vulkan_renderer_geometry_command_buffers[s_vulkan_renderer_frame_index]);

	TRACY_ZONE_END
}
void vulkan_renderer_geometry_pass_end(void)
{
	TRACY_ZONE_BEGIN

	vkCmdEndRenderPass(s_vulkan_renderer_geometry_command_buffers[s_vulkan_renderer_frame_index]);

	TRACY_ZONE_END
}
void vulkan_renderer_command_buffer_free(void)
{
	TRACY_ZONE_BEGIN

	vkFreeCommandBuffers(g_vulkan_device, g_vulkan_command_pool, VEGA_FRAMES_IN_FLIGHT, s_vulkan_renderer_compute_command_buffers);
	vkFreeCommandBuffers(g_vulkan_device, g_vulkan_command_pool, VEGA_FRAMES_IN_FLIGHT, s_vulkan_renderer_geometry_command_buffers);

	TRACY_ZONE_END
}
void vulkan_renderer_sync_objects_free(void)
{
	TRACY_ZONE_BEGIN

	vkQueueWaitIdle(g_vulkan_device_compute_queue);
	vkQueueWaitIdle(g_vulkan_device_graphic_queue);
	vkQueueWaitIdle(g_vulkan_device_present_queue);

	uint64_t semaphore_index = 0;
	while (semaphore_index < VEGA_FRAMES_IN_FLIGHT)
	{
		vkDestroySemaphore(g_vulkan_device, s_vulkan_renderer_compute_complete_semaphores[semaphore_index], 0);
		vkDestroySemaphore(g_vulkan_device, s_vulkan_renderer_graphic_complete_semaphores[semaphore_index], 0);
		vkDestroySemaphore(g_vulkan_device, s_vulkan_renderer_image_available_semaphores[semaphore_index], 0);

		semaphore_index++;
	}

	uint64_t fence_index = 0;
	while (fence_index < VEGA_FRAMES_IN_FLIGHT)
	{
		vkDestroyFence(g_vulkan_device, s_vulkan_renderer_render_fences[fence_index], 0);

		fence_index++;
	}

	TRACY_ZONE_END
}
void vulkan_renderer_render_pass_free(void)
{
	TRACY_ZONE_BEGIN

	vkDestroyRenderPass(g_vulkan_device, s_vulkan_renderer_render_pass, 0);

	TRACY_ZONE_END
}
void vulkan_renderer_frame_buffer_free(void)
{
	TRACY_ZONE_BEGIN

	uint32_t swap_chain_image_index = 0;
	while (swap_chain_image_index < g_vulkan_swap_chain_image_count)
	{
		vkDestroyFramebuffer(g_vulkan_device, *(VkFramebuffer*)std_vector_get(&s_vulkan_renderer_frame_buffers, swap_chain_image_index), 0);

		swap_chain_image_index++;
	}

	TRACY_ZONE_END
}
void vulkan_renderer_imgui_context_free(void)
{
	TRACY_ZONE_BEGIN

	ImGui_ImplVulkan_DestroyFontsTexture();
	ImGui_ImplVulkan_Shutdown();

	ImGui_ImplWin32_Shutdown();

	ImGui::DestroyContext();

	TRACY_ZONE_END
}
static void vulkan_renderer_update_camera_info_proc(ecs_t* ecs, uint64_t index, uint64_t entity)
{
	TRACY_ZONE_BEGIN

	transform_t* transform = (transform_t*)std_ecs_get(ecs, entity, VEGA_COMPONENT_TYPE_TRANSFORM);
	camera_t* camera = (camera_t*)std_ecs_get(ecs, entity, VEGA_COMPONENT_TYPE_CAMERA);

	camera_info_t* camera_info = (camera_info_t*)s_vulkan_renderer_camera_info_buffer.mapped_buffer;

	vector3_t eye = transform->world_position;
	vector3_t center = math_vector3_add(transform->world_position, transform->local_front);
	vector3_t up = transform->local_up;

	camera_info->view = math_matrix4_look_at(eye, center, up);

	switch (camera->mode)
	{
		case CAMERA_MODE_ORTHO:
		{
			double left = camera->left;
			double right = camera->right;
			double bottom = camera->bottom;
			double top = camera->top;
			double near_z = camera->near_z;
			double far_z = camera->far_z;

			camera_info->projection = math_matrix4_ortho(left, right, bottom, top, near_z, far_z);

			break;
		}
		case CAMERA_MODE_PERSP:
		{
			double fov = camera->fov;
			double aspect_ratio = (double)g_vulkan_surface_width / (double)g_vulkan_surface_height;
			double near_z = camera->near_z;
			double far_z = camera->far_z;

			camera_info->projection = math_matrix4_persp(fov, aspect_ratio, near_z, far_z);

			break;
		}
	}

	TRACY_ZONE_END
}
static void vulkan_renderer_update_pbr_descriptor_sets_proc(ecs_t* ecs, uint64_t index, uint64_t entity)
{
	TRACY_ZONE_BEGIN

	transform_t* transform = (transform_t*)std_ecs_get(ecs, entity, VEGA_COMPONENT_TYPE_TRANSFORM);
	renderable_t* renderable = (renderable_t*)std_ecs_get(ecs, entity, VEGA_COMPONENT_TYPE_RENDERABLE);

	// TODO: support more workflows..

	material_asset_t* material_asset = (material_asset_t*)renderable->material_asset;

	// TODO: abstract asset stuff out of the renderer..
	texture_asset_t* texture_asset = (texture_asset_t*)std_map_get(&g_asset_loader_textures, std_string_buffer(&material_asset->base_color_ref), std_string_size(&material_asset->base_color_ref), 0);

	VkDescriptorSet descriptor_set = *(VkDescriptorSet*)std_vector_get(&s_vulkan_renderer_pbr_descriptor_sets, index);

	VkWriteDescriptorSet write_descriptor_sets[4] = {};

	uint32_t available_write_descriptor_sets = 0;

	{
		uint32_t descriptor_binding_index = 0;

		VkDescriptorBufferInfo time_info_descriptor_buffer_info = {};
		time_info_descriptor_buffer_info.offset = 0;
		time_info_descriptor_buffer_info.buffer = s_vulkan_renderer_time_info_buffer.buffer;
		time_info_descriptor_buffer_info.range = s_vulkan_renderer_time_info_buffer.size;

		write_descriptor_sets[descriptor_binding_index].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
		write_descriptor_sets[descriptor_binding_index].pNext = 0;
		write_descriptor_sets[descriptor_binding_index].dstSet = descriptor_set;
		write_descriptor_sets[descriptor_binding_index].dstBinding = descriptor_binding_index;
		write_descriptor_sets[descriptor_binding_index].dstArrayElement = 0;
		write_descriptor_sets[descriptor_binding_index].descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
		write_descriptor_sets[descriptor_binding_index].descriptorCount = 1;
		write_descriptor_sets[descriptor_binding_index].pImageInfo = 0;
		write_descriptor_sets[descriptor_binding_index].pBufferInfo = &time_info_descriptor_buffer_info;
		write_descriptor_sets[descriptor_binding_index].pTexelBufferView = 0;

		available_write_descriptor_sets++;
	}

	{
		uint32_t descriptor_binding_index = 1;

		VkDescriptorBufferInfo camera_info_descriptor_buffer_info = {};
		camera_info_descriptor_buffer_info.offset = 0;
		camera_info_descriptor_buffer_info.buffer = s_vulkan_renderer_camera_info_buffer.buffer;
		camera_info_descriptor_buffer_info.range = s_vulkan_renderer_camera_info_buffer.size;

		write_descriptor_sets[descriptor_binding_index].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
		write_descriptor_sets[descriptor_binding_index].pNext = 0;
		write_descriptor_sets[descriptor_binding_index].dstSet = descriptor_set;
		write_descriptor_sets[descriptor_binding_index].dstBinding = descriptor_binding_index;
		write_descriptor_sets[descriptor_binding_index].dstArrayElement = 0;
		write_descriptor_sets[descriptor_binding_index].descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
		write_descriptor_sets[descriptor_binding_index].descriptorCount = 1;
		write_descriptor_sets[descriptor_binding_index].pImageInfo = 0;
		write_descriptor_sets[descriptor_binding_index].pBufferInfo = &camera_info_descriptor_buffer_info;
		write_descriptor_sets[descriptor_binding_index].pTexelBufferView = 0;

		available_write_descriptor_sets++;
	}

	{
		uint32_t descriptor_binding_index = 2;

		VkDescriptorBufferInfo transform_descriptor_buffer_info = {};
		transform_descriptor_buffer_info.offset = 0;
		transform_descriptor_buffer_info.buffer = s_vulkan_renderer_transform_buffer.buffer;
		transform_descriptor_buffer_info.range = s_vulkan_renderer_transform_buffer.size;

		write_descriptor_sets[descriptor_binding_index].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
		write_descriptor_sets[descriptor_binding_index].pNext = 0;
		write_descriptor_sets[descriptor_binding_index].dstSet = descriptor_set;
		write_descriptor_sets[descriptor_binding_index].dstBinding = descriptor_binding_index;
		write_descriptor_sets[descriptor_binding_index].dstArrayElement = 0;
		write_descriptor_sets[descriptor_binding_index].descriptorType = VK_DESCRIPTOR_TYPE_STORAGE_BUFFER;
		write_descriptor_sets[descriptor_binding_index].descriptorCount = 1;
		write_descriptor_sets[descriptor_binding_index].pImageInfo = 0;
		write_descriptor_sets[descriptor_binding_index].pBufferInfo = &transform_descriptor_buffer_info;
		write_descriptor_sets[descriptor_binding_index].pTexelBufferView = 0;

		available_write_descriptor_sets++;
	}

	if (texture_asset)
	{
		uint32_t descriptor_binding_index = 3;

		VkDescriptorImageInfo base_color_descriptor_image_info = {};
		base_color_descriptor_image_info.imageLayout = VK_IMAGE_LAYOUT_GENERAL;
		base_color_descriptor_image_info.imageView = texture_asset->image.image_view;
		base_color_descriptor_image_info.sampler = texture_asset->image.sampler;

		write_descriptor_sets[descriptor_binding_index].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
		write_descriptor_sets[descriptor_binding_index].pNext = 0;
		write_descriptor_sets[descriptor_binding_index].dstSet = descriptor_set;
		write_descriptor_sets[descriptor_binding_index].dstBinding = descriptor_binding_index;
		write_descriptor_sets[descriptor_binding_index].dstArrayElement = 0;
		write_descriptor_sets[descriptor_binding_index].descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
		write_descriptor_sets[descriptor_binding_index].descriptorCount = 1;
		write_descriptor_sets[descriptor_binding_index].pImageInfo = &base_color_descriptor_image_info;
		write_descriptor_sets[descriptor_binding_index].pBufferInfo = 0;
		write_descriptor_sets[descriptor_binding_index].pTexelBufferView = 0;

		available_write_descriptor_sets++;
	}

	vkUpdateDescriptorSets(g_vulkan_device, available_write_descriptor_sets, write_descriptor_sets, 0, 0);

	TRACY_ZONE_END
}
static void vulkan_renderer_build_geometry_queues_proc(ecs_t* ecs, uint64_t index, uint64_t entity)
{
	TRACY_ZONE_BEGIN

	transform_t* transform = (transform_t*)std_ecs_get(ecs, entity, VEGA_COMPONENT_TYPE_TRANSFORM);
	renderable_t* renderable = (renderable_t*)std_ecs_get(ecs, entity, VEGA_COMPONENT_TYPE_RENDERABLE);

	string_t mesh_name = renderable->mesh_asset->name;

	if (std_map_contains(&s_vulkan_renderer_geometry_passes, std_string_buffer(&mesh_name), std_string_size(&mesh_name)))
	{
		geometry_pass_t* geometry_pass = (geometry_pass_t*)std_map_get(&s_vulkan_renderer_geometry_passes, std_string_buffer(&mesh_name), std_string_size(&mesh_name), 0);

		std_fvector64_push(&geometry_pass->transforms, (uint64_t)transform);
	}
	else
	{
		geometry_pass_t* geometry_pass = (geometry_pass_t*)std_map_insert(&s_vulkan_renderer_geometry_passes, std_string_buffer(&mesh_name), std_string_size(&mesh_name), 0, sizeof(geometry_pass_t));

		geometry_pass->material_asset = renderable->material_asset;
		geometry_pass->mesh_asset = renderable->mesh_asset;
		geometry_pass->transforms = std_fvector64_alloc(); // TODO: free this..
	}

	TRACY_ZONE_END
}
