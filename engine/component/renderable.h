#ifndef VEGA_INSTANCE_COMPONENT_RENDERABLE_H
#define VEGA_INSTANCE_COMPONENT_RENDERABLE_H

#include <stdint.h>

#include <vega/core/core.h>

#include <vega/engine/renderer/material.h>
#include <vega/engine/renderer/mesh.h>

typedef struct _renderable_t
{
	material_type_t material_type;
	void* material;
	mesh_t* mesh;
} renderable_t;

#endif // VEGA_INSTANCE_COMPONENT_RENDERABLE_H
