#version 460 core

layout (location = 0) in vec4 vertex_position;
layout (location = 1) in vec4 vertex_normal;
layout (location = 2) in vec4 vertex_tangent;
layout (location = 3) in vec4 vertex_bitangent;
layout (location = 4) in vec4 vertex_color_channel_0;
layout (location = 5) in vec2 vertex_texture_channel_0;
layout (location = 6) in vec4 vertex_bone_indices;
layout (location = 7) in vec4 vertex_bone_weights;

layout (binding = 0) uniform time_info_t
{
	double time;
	double delta_time;
} time_info;

layout (binding = 1) uniform projection_info_t
{
	mat4 view;
	mat4 projection;
} projection_info;

layout (location = 0) out vec3 output_position;
layout (location = 1) out vec3 output_normal;
layout (location = 2) out vec3 output_tangent;
layout (location = 3) out vec3 output_bitangent;
layout (location = 4) out vec4 output_color_channel_0;
layout (location = 5) out vec2 output_texcoord_channel_0;

void main()
{
	vec4 world_position = projection_info.projection * projection_info.view * vertex_position;

	output_position = vec3(world_position);
	output_normal = vec3(vertex_normal);
	output_tangent = vec3(vertex_tangent);
	output_bitangent = vec3(vertex_bitangent);
	output_color_channel_0 = vertex_color_channel_0;
	output_texcoord_channel_0 = vertex_texture_channel_0;

	gl_Position = world_position;
}
