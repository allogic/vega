#ifndef VEGA_ENGINE_VULKAN_PIPELINE_H
#define VEGA_ENGINE_VULKAN_PIPELINE_H

#include <stdint.h>

#include <vega/core/core.h>

#include <vulkan/vulkan.h>

VEGA_EXTERN_C_BEGIN

///////////////////////////////////////////////////////////////
// Public API
///////////////////////////////////////////////////////////////

VkDescriptorPool vulkan_pipeline_descriptor_pool_alloc(uint32_t pool_count, VkDescriptorSetLayoutBinding* descriptor_set_layout_bindings, uint64_t descriptor_set_layout_binding_count);
VkDescriptorSetLayout vulkan_pipeline_descriptor_set_layout_alloc(VkDescriptorSetLayoutBinding* descriptor_set_layout_bindings, uint64_t descriptor_set_layout_binding_count);
VkPipelineLayout vulkan_pipeline_layout_alloc(VkDescriptorSetLayout descriptor_set_layout, VkPushConstantRange* push_constant_ranges, uint64_t push_constant_range_count);
VkPipeline vulkan_pipeline_graphic_alloc(VkPipelineLayout pipeline_layout, VkRenderPass render_pass, VkPrimitiveTopology topology, char const* vertex_shader_file_path, char const* fragment_shader_file_path, VkVertexInputBindingDescription* vertex_input_binding_descriptions, uint64_t vertex_input_binding_description_count, VkVertexInputAttributeDescription* vertex_input_attribute_descriptions, uint64_t vertex_input_attribute_description_count);
VkPipeline vulkan_pipeline_pipeline_alloc(VkPipelineLayout pipeline_layout, char const* compute_shader_file_path);

void vulkan_pipeline_descriptor_pool_free(VkDescriptorPool descriptor_pool);
void vulkan_pipeline_descriptor_set_layout_free(VkDescriptorSetLayout descriptor_set_layout);
void vulkan_pipeline_layout_free(VkPipelineLayout pipeline_layout);
void vulkan_pipeline_free(VkPipeline pipeline);

VEGA_EXTERN_C_END

#endif // VEGA_ENGINE_VULKAN_PIPELINE_H
