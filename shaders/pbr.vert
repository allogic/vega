#version 460 core

#extension GL_ARB_gpu_shader_fp64 : require

layout (location = 0) in dvec4 vertex_position;
layout (location = 2) in dvec4 vertex_normal;
layout (location = 4) in dvec4 vertex_tangent;
layout (location = 6) in dvec4 vertex_bitangent;
layout (location = 8) in dvec4 vertex_color_channel_0;
layout (location = 10) in dvec2 vertex_texture_channel_0;
layout (location = 12) in dvec4 vertex_bone_indices;
layout (location = 14) in dvec4 vertex_bone_weights;

layout (binding = 0) uniform time_info_t
{
	double time;
	double delta_time;
} time_info;

layout (binding = 1) uniform projection_info_t
{
	dmat4 view;
	dmat4 projection;
} projection_info;

layout (location = 0) out vec3 output_position;
layout (location = 1) out vec3 output_normal;
layout (location = 2) out vec3 output_tangent;
layout (location = 3) out vec3 output_bitangent;
layout (location = 4) out vec4 output_color_channel_0;
layout (location = 5) out vec2 output_texcoord_channel_0;

void main()
{
	dvec4 world_position = projection_info.projection * projection_info.view * vertex_position;

	output_position = vec3(world_position);
	output_normal = vec3(vertex_normal);
	output_tangent = vec3(vertex_tangent);
	output_bitangent = vec3(vertex_bitangent);
	output_color_channel_0 = vec4(vertex_color_channel_0);
	output_texcoord_channel_0 = vec2(vertex_texture_channel_0);

	gl_Position = vec4(world_position);
}
