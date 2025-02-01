#ifndef VEGA_ENGINE_ASSET_MESH_H
#define VEGA_ENGINE_ASSET_MESH_H

#include <stdint.h>

#include <vega/core/core.h>

#include <vega/engine/vulkan/buffer.h>

typedef struct _mesh_asset_t
{
	string_t name;
	uint32_t mesh_index;
	uint8_t loaded;
	uint8_t is_loading;
	string_t material_ref;
	vector_t pbr_vertices;
	fvector32_t pbr_indices;
	buffer_t pbr_vertex_buffer;
	buffer_t pbr_index_buffer;
} mesh_asset_t;

#endif // VEGA_ENGINE_ASSET_MESH_H
