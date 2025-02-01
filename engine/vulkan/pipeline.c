#include <string.h>

#include <vega/engine/vulkan/instance.h>
#include <vega/engine/vulkan/pipeline.h>

#ifndef TRACY_ZONE_BEGIN
	#define TRACY_ZONE_BEGIN TracyCZoneC(ctx, TRACY_COLOR_GREEN, 1U);
#endif // TRACY_ZONE_BEGIN

#ifndef TRACY_ZONE_END
	#define TRACY_ZONE_END TracyCZoneEnd(ctx);
#endif // TRACY_ZONE_END

VkDescriptorPool vulkan_pipeline_descriptor_pool_alloc(uint32_t pool_count, VkDescriptorSetLayoutBinding* descriptor_set_layout_bindings, uint64_t descriptor_set_layout_binding_count)
{
	TRACY_ZONE_BEGIN

	VkDescriptorPool descriptor_pool = 0;

	vector_t descriptor_pool_sizes = std_vector_alloc(sizeof(VkDescriptorPoolSize));
	fvector32_t descriptor_type_counts = std_fvector32_alloc();

	std_fvector32_resize(&descriptor_type_counts, VK_DESCRIPTOR_TYPE_INPUT_ATTACHMENT);
	std_fvector32_fill(&descriptor_type_counts, 0);

	uint64_t descriptor_pool_binding_index = 0;
	while (descriptor_pool_binding_index < descriptor_set_layout_binding_count)
	{
		uint64_t descriptor_type_index = descriptor_set_layout_bindings[descriptor_pool_binding_index].descriptorType;

		uint32_t descriptor_count = std_fvector32_get(&descriptor_type_counts, descriptor_type_index);
		descriptor_count++;
		std_fvector32_set(&descriptor_type_counts, descriptor_type_index, descriptor_count);

		descriptor_pool_binding_index++;
	}

	uint64_t descriptor_type_index = 0;
	uint64_t descriptor_type_count = std_fvector32_count(&descriptor_type_counts);
	while (descriptor_type_index < descriptor_type_count)
	{
		uint32_t descriptor_count = std_fvector32_get(&descriptor_type_counts, descriptor_type_index);

		if (descriptor_count > 0)
		{
			VkDescriptorPoolSize descriptor_pool_size = { 0 };
			descriptor_pool_size.type = (VkDescriptorType)descriptor_type_index;
			descriptor_pool_size.descriptorCount = descriptor_count;

			std_vector_push(&descriptor_pool_sizes, &descriptor_pool_size);
		}

		descriptor_type_index++;
	}

	VkDescriptorPoolCreateInfo descriptor_pool_create_info = { 0 };
	descriptor_pool_create_info.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
	descriptor_pool_create_info.pPoolSizes = std_vector_buffer(&descriptor_pool_sizes);
	descriptor_pool_create_info.poolSizeCount = (uint32_t)std_vector_count(&descriptor_pool_sizes);
	descriptor_pool_create_info.maxSets = pool_count;

	vkCreateDescriptorPool(g_vulkan_instance_device, &descriptor_pool_create_info, 0, &descriptor_pool);

	std_vector_free(&descriptor_pool_sizes);
	std_fvector32_free(&descriptor_type_counts);

	TRACY_ZONE_END

	return descriptor_pool;
}
VkDescriptorSetLayout vulkan_pipeline_descriptor_set_layout_alloc(VkDescriptorSetLayoutBinding* descriptor_set_layout_bindings, uint64_t descriptor_set_layout_binding_count)
{
	TRACY_ZONE_BEGIN

	VkDescriptorSetLayout descriptor_set_layout = 0;

	fvector32_t descriptor_binding_flags = std_fvector32_alloc();

	std_fvector32_resize(&descriptor_binding_flags, descriptor_set_layout_binding_count);
	std_fvector32_fill(&descriptor_binding_flags, 0);

	uint64_t descriptor_binding_flags_index = 0;
	while (descriptor_binding_flags_index < descriptor_set_layout_binding_count)
	{
		std_fvector32_set(&descriptor_binding_flags, descriptor_binding_flags_index, VK_DESCRIPTOR_BINDING_PARTIALLY_BOUND_BIT);

		descriptor_binding_flags_index++;
	}

	VkDescriptorSetLayoutBindingFlagsCreateInfo descriptor_set_layout_binding_flags_create_info = { 0 };
	descriptor_set_layout_binding_flags_create_info.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_BINDING_FLAGS_CREATE_INFO;
	descriptor_set_layout_binding_flags_create_info.pBindingFlags = std_fvector32_buffer(&descriptor_binding_flags);
	descriptor_set_layout_binding_flags_create_info.bindingCount = (uint32_t)std_fvector32_count(&descriptor_binding_flags);

	VkDescriptorSetLayoutCreateInfo descriptor_set_layout_create_info = { 0 };
	descriptor_set_layout_create_info.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
	descriptor_set_layout_create_info.pBindings = descriptor_set_layout_bindings;
	descriptor_set_layout_create_info.bindingCount = (uint32_t)descriptor_set_layout_binding_count;
	descriptor_set_layout_create_info.pNext = &descriptor_set_layout_binding_flags_create_info;

	vkCreateDescriptorSetLayout(g_vulkan_instance_device, &descriptor_set_layout_create_info, 0, &descriptor_set_layout);

	std_fvector32_free(&descriptor_binding_flags);

	TRACY_ZONE_END

	return descriptor_set_layout;
}
VkPipelineLayout vulkan_pipeline_layout_alloc(VkDescriptorSetLayout descriptor_set_layout, VkPushConstantRange* push_constant_ranges, uint64_t push_constant_range_count)
{
	TRACY_ZONE_BEGIN

	VkPipelineLayout pipeline_layout = 0;

	VkPipelineLayoutCreateInfo pipeline_layout_create_info = { 0 };
	pipeline_layout_create_info.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
	pipeline_layout_create_info.setLayoutCount = 1;
	pipeline_layout_create_info.pSetLayouts = &descriptor_set_layout;
	pipeline_layout_create_info.pPushConstantRanges = push_constant_ranges;
	pipeline_layout_create_info.pushConstantRangeCount = (uint32_t)push_constant_range_count;

	vkCreatePipelineLayout(g_vulkan_instance_device, &pipeline_layout_create_info, 0, &pipeline_layout);

	TRACY_ZONE_END

	return pipeline_layout;
}
VkPipeline vulkan_pipeline_graphic_alloc(VkPipelineLayout pipeline_layout, VkRenderPass render_pass, VkPrimitiveTopology topology, char const* vertex_shader_file_path, char const* fragment_shader_file_path, VkVertexInputBindingDescription* vertex_input_binding_descriptions, uint64_t vertex_input_binding_description_count, VkVertexInputAttributeDescription* vertex_input_attribute_descriptions, uint64_t vertex_input_attribute_description_count)
{
	TRACY_ZONE_BEGIN

	VkPipeline pipeline = 0;

	uint8_t* vertex_shader_bytes = 0;
	uint8_t* fragment_shader_bytes = 0;

	uint64_t vertex_shader_size = 0;
	uint64_t fragment_shader_size = 0;

	fs_read_binary(&vertex_shader_bytes, &vertex_shader_size, vertex_shader_file_path);
	fs_read_binary(&fragment_shader_bytes, &fragment_shader_size, fragment_shader_file_path);

	VkShaderModule vertex_module = 0;
	VkShaderModule fragment_module = 0;

	{
		VkShaderModuleCreateInfo shader_module_create_info = { 0 };
		shader_module_create_info.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
		shader_module_create_info.codeSize = vertex_shader_size;
		shader_module_create_info.pCode = (uint32_t const*)vertex_shader_bytes;

		vkCreateShaderModule(g_vulkan_instance_device, &shader_module_create_info, 0, &vertex_module);
	}

	{
		VkShaderModuleCreateInfo shader_module_create_info = { 0 };
		shader_module_create_info.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
		shader_module_create_info.codeSize = fragment_shader_size;
		shader_module_create_info.pCode = (uint32_t const*)fragment_shader_bytes;

		vkCreateShaderModule(g_vulkan_instance_device, &shader_module_create_info, 0, &fragment_module);
	}

	VkPipelineShaderStageCreateInfo vertex_shader_stage_create_info = { 0 };
	vertex_shader_stage_create_info.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
	vertex_shader_stage_create_info.stage = VK_SHADER_STAGE_VERTEX_BIT;
	vertex_shader_stage_create_info.module = vertex_module;
	vertex_shader_stage_create_info.pName = "main";

	VkPipelineShaderStageCreateInfo fragment_shader_stage_create_info = { 0 };
	fragment_shader_stage_create_info.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
	fragment_shader_stage_create_info.stage = VK_SHADER_STAGE_FRAGMENT_BIT;
	fragment_shader_stage_create_info.module = fragment_module;
	fragment_shader_stage_create_info.pName = "main";

	VkPipelineShaderStageCreateInfo shader_stages[] = { vertex_shader_stage_create_info, fragment_shader_stage_create_info };

	VkPipelineVertexInputStateCreateInfo vertex_input_create_info = { 0 };
	vertex_input_create_info.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
	vertex_input_create_info.pVertexBindingDescriptions = vertex_input_binding_descriptions;
	vertex_input_create_info.vertexBindingDescriptionCount = (uint32_t)vertex_input_binding_description_count;
	vertex_input_create_info.pVertexAttributeDescriptions = vertex_input_attribute_descriptions;
	vertex_input_create_info.vertexAttributeDescriptionCount = (uint32_t)vertex_input_attribute_description_count;

	VkPipelineInputAssemblyStateCreateInfo input_assembly_create_info = { 0 };
	input_assembly_create_info.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
	input_assembly_create_info.topology = topology;
	input_assembly_create_info.primitiveRestartEnable = 0;

	VkViewport viewport = { 0 };
	viewport.x = 0.0F;
	viewport.y = 0.0F;
	viewport.width = (float)g_vulkan_instance_surface_capabilities.currentExtent.width;
	viewport.height = (float)g_vulkan_instance_surface_capabilities.currentExtent.height;
	viewport.minDepth = 0.0F;
	viewport.maxDepth = 1.0F;

	VkRect2D scissor = { 0 };
	scissor.offset.x = 0;
	scissor.offset.y = 0;
	scissor.extent.width = g_vulkan_instance_surface_capabilities.currentExtent.width;
	scissor.extent.height = g_vulkan_instance_surface_capabilities.currentExtent.height;

	VkPipelineViewportStateCreateInfo viewport_state_create_info = { 0 };
	viewport_state_create_info.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
	viewport_state_create_info.viewportCount = 1;
	viewport_state_create_info.pViewports = &viewport;
	viewport_state_create_info.scissorCount = 1;
	viewport_state_create_info.pScissors = &scissor;

	VkPipelineRasterizationStateCreateInfo rasterizer_create_info = { 0 };
	rasterizer_create_info.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
	rasterizer_create_info.depthClampEnable = 0;
	rasterizer_create_info.rasterizerDiscardEnable = 0;
	rasterizer_create_info.polygonMode = VK_POLYGON_MODE_FILL;
	rasterizer_create_info.lineWidth = 1.0F;
	rasterizer_create_info.cullMode = VK_CULL_MODE_BACK_BIT;
	rasterizer_create_info.frontFace = VK_FRONT_FACE_CLOCKWISE;
	rasterizer_create_info.depthBiasEnable = 0;
	rasterizer_create_info.depthBiasConstantFactor = 0.0F;
	rasterizer_create_info.depthBiasClamp = 0.0F;
	rasterizer_create_info.depthBiasSlopeFactor = 0.0F;

	VkPipelineMultisampleStateCreateInfo multisampling_create_info = { 0 };
	multisampling_create_info.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
	multisampling_create_info.sampleShadingEnable = 0;
	multisampling_create_info.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT;
	multisampling_create_info.minSampleShading = 1.0F;
	multisampling_create_info.pSampleMask = 0;
	multisampling_create_info.alphaToCoverageEnable = 0;
	multisampling_create_info.alphaToOneEnable = 0;

	VkPipelineColorBlendAttachmentState color_blend_attachment = { 0 };
	color_blend_attachment.colorWriteMask = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT;
	color_blend_attachment.blendEnable = 1;
	color_blend_attachment.srcColorBlendFactor = VK_BLEND_FACTOR_SRC_ALPHA;
	color_blend_attachment.dstColorBlendFactor = VK_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA;
	color_blend_attachment.colorBlendOp = VK_BLEND_OP_ADD;
	color_blend_attachment.srcAlphaBlendFactor = VK_BLEND_FACTOR_SRC_ALPHA;
	color_blend_attachment.dstAlphaBlendFactor = VK_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA;
	color_blend_attachment.alphaBlendOp = VK_BLEND_OP_ADD;

	VkPipelineDepthStencilStateCreateInfo depth_stencil_state_create_info = { 0 };
	depth_stencil_state_create_info.sType = VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO;
	depth_stencil_state_create_info.depthTestEnable = 0; // TODO: why is this not working..
	depth_stencil_state_create_info.depthWriteEnable = 1;
	depth_stencil_state_create_info.depthCompareOp = VK_COMPARE_OP_LESS;
	depth_stencil_state_create_info.depthBoundsTestEnable = 0;
	depth_stencil_state_create_info.stencilTestEnable = 0;

	VkPipelineColorBlendStateCreateInfo color_blend_create_info = { 0 };
	color_blend_create_info.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
	color_blend_create_info.logicOpEnable = 0;
	color_blend_create_info.logicOp = VK_LOGIC_OP_COPY;
	color_blend_create_info.attachmentCount = 1;
	color_blend_create_info.pAttachments = &color_blend_attachment;
	color_blend_create_info.blendConstants[0] = 0.0F;
	color_blend_create_info.blendConstants[1] = 0.0F;
	color_blend_create_info.blendConstants[2] = 0.0F;
	color_blend_create_info.blendConstants[3] = 0.0F;

	VkDynamicState dynamic_states[] = { VK_DYNAMIC_STATE_VIEWPORT, VK_DYNAMIC_STATE_SCISSOR };

	VkPipelineDynamicStateCreateInfo dynamic_state_create_info = { 0 };
	dynamic_state_create_info.sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO;
	dynamic_state_create_info.pDynamicStates = dynamic_states;
	dynamic_state_create_info.dynamicStateCount = (uint32_t)ARRAY_COUNT(dynamic_states);

	VkGraphicsPipelineCreateInfo pipeline_create_info = { 0 };
	pipeline_create_info.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
	pipeline_create_info.pStages = shader_stages;
	pipeline_create_info.stageCount = (uint32_t)ARRAY_COUNT(shader_stages);
	pipeline_create_info.pVertexInputState = &vertex_input_create_info;
	pipeline_create_info.pInputAssemblyState = &input_assembly_create_info;
	pipeline_create_info.pViewportState = &viewport_state_create_info;
	pipeline_create_info.pRasterizationState = &rasterizer_create_info;
	pipeline_create_info.pMultisampleState = &multisampling_create_info;
	pipeline_create_info.pDepthStencilState = &depth_stencil_state_create_info;
	pipeline_create_info.pColorBlendState = &color_blend_create_info;
	pipeline_create_info.pDynamicState = &dynamic_state_create_info;
	pipeline_create_info.layout = pipeline_layout;
	pipeline_create_info.renderPass = render_pass;
	pipeline_create_info.subpass = 0;
	pipeline_create_info.basePipelineHandle = 0;

	vkCreateGraphicsPipelines(g_vulkan_instance_device, 0, 1, &pipeline_create_info, 0, &pipeline);

	vkDestroyShaderModule(g_vulkan_instance_device, vertex_module, 0);
	vkDestroyShaderModule(g_vulkan_instance_device, fragment_module, 0);

	heap_free(vertex_shader_bytes);
	heap_free(fragment_shader_bytes);

	TRACY_ZONE_END

	return pipeline;
}
VkPipeline vulkan_pipeline_pipeline_alloc(VkPipelineLayout pipeline_layout, char const* compute_shader_file_path)
{
	TRACY_ZONE_BEGIN

	VkPipeline pipeline = 0;

	uint8_t* compute_shader_bytes = 0;

	uint64_t compute_shader_size = 0;

	fs_read_binary(&compute_shader_bytes, &compute_shader_size, compute_shader_file_path);

	VkShaderModule compute_module = 0;

	VkShaderModuleCreateInfo shader_module_create_info = { 0 };
	shader_module_create_info.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
	shader_module_create_info.codeSize = compute_shader_size;
	shader_module_create_info.pCode = (uint32_t const*)compute_shader_bytes;

	vkCreateShaderModule(g_vulkan_instance_device, &shader_module_create_info, 0, &compute_module);

	VkPipelineShaderStageCreateInfo compute_shader_stage_create_info = { 0 };
	compute_shader_stage_create_info.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
	compute_shader_stage_create_info.stage = VK_SHADER_STAGE_COMPUTE_BIT;
	compute_shader_stage_create_info.module = compute_module;
	compute_shader_stage_create_info.pName = "main";

	VkComputePipelineCreateInfo pipeline_create_info = { 0 };
	pipeline_create_info.sType = VK_STRUCTURE_TYPE_COMPUTE_PIPELINE_CREATE_INFO;
	pipeline_create_info.layout = pipeline_layout;
	pipeline_create_info.stage = compute_shader_stage_create_info;

	vkCreateComputePipelines(g_vulkan_instance_device, 0, 1, &pipeline_create_info, 0, &pipeline);

	vkDestroyShaderModule(g_vulkan_instance_device, compute_module, 0);

	heap_free(compute_shader_bytes);

	TRACY_ZONE_END

	return pipeline;
}
void vulkan_pipeline_descriptor_pool_free(VkDescriptorPool descriptor_pool)
{
	TRACY_ZONE_BEGIN

	vkDestroyDescriptorPool(g_vulkan_instance_device, descriptor_pool, 0);

	TRACY_ZONE_END
}
void vulkan_pipeline_descriptor_set_layout_free(VkDescriptorSetLayout descriptor_set_layout)
{
	TRACY_ZONE_BEGIN

	vkDestroyDescriptorSetLayout(g_vulkan_instance_device, descriptor_set_layout, 0);

	TRACY_ZONE_END
}
void vulkan_pipeline_layout_free(VkPipelineLayout pipeline_layout)
{
	TRACY_ZONE_BEGIN

	vkDestroyPipelineLayout(g_vulkan_instance_device, pipeline_layout, 0);

	TRACY_ZONE_END
}
void vulkan_pipeline_free(VkPipeline pipeline)
{
	TRACY_ZONE_BEGIN

	vkDestroyPipeline(g_vulkan_instance_device, pipeline, 0);

	TRACY_ZONE_END
}
