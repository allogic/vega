#ifndef VEGA_ENGINE_ASSET_MESH_H
#define VEGA_ENGINE_ASSET_MESH_H

#include <stdint.h>

#include <vega/core/core.h>

typedef struct _asset_mesh_t
{
	vector_t pbr_vertices;
	fvector32_t pbr_indices;
} asset_mesh_t;

#endif // VEGA_ENGINE_ASSET_MESH_H
