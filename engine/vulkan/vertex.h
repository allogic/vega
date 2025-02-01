#ifndef VEGA_ENGINE_VULKAN_VERTEX_H
#define VEGA_ENGINE_VULKAN_VERTEX_H

#include <stdint.h>

#include <vega/core/core.h>

typedef struct _pbr_vertex_t
{
	vector4_t position;
	vector4_t normal;
	vector4_t tangent;
	vector4_t bitangent;
	vector4_t color_channel_0;
	vector2_t texcoord_channel_0;
	vector2_t reserved_0;
	vector4_t bone_indices; // TODO
	vector4_t bone_weights; // TODO
} pbr_vertex_t;

#endif // VEGA_ENGINE_VULKAN_VERTEX_H
