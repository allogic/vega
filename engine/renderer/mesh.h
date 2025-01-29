#ifndef VEGA_ENGINE_RENDERER_MESH_H
#define VEGA_ENGINE_RENDERER_MESH_H

#include <stdint.h>

#include <vega/core/core.h>

#include <vega/engine/vulkan/buffer.h>

typedef struct _mesh_t
{
	buffer_t* vertex_buffer;
	buffer_t* index_buffer;
} mesh_t;

#endif // VEGA_ENGINE_RENDERER_MESH_H
