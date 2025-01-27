#version 460 core

layout (location = 0) in vec3 input_position;
layout (location = 1) in vec3 input_normal;
layout (location = 2) in vec3 input_tangent;
layout (location = 3) in vec3 input_bitangent;
layout (location = 4) in vec4 input_color_channel_0;
layout (location = 5) in vec2 input_texcoord_channel_0;

layout (binding = 0) uniform sampler2D base_color_sampler;

layout (location = 0) out vec4 output_color;

void main()
{
	output_color = texture(base_color_sampler, input_texcoord_channel_0);
}
