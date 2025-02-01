#ifndef VEGA_INSTANCE_COMPONENT_RENDERABLE_H
#define VEGA_INSTANCE_COMPONENT_RENDERABLE_H

#include <stdint.h>

#include <vega/core/core.h>

#include <vega/engine/asset/material.h>
#include <vega/engine/asset/mesh.h>

typedef struct _renderable_t
{
	mesh_asset_t* mesh_asset;
	material_asset_t* material_asset;
} renderable_t;

#endif // VEGA_INSTANCE_COMPONENT_RENDERABLE_H
