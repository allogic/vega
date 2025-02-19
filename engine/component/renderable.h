#ifndef VEGA_INSTANCE_COMPONENT_RENDERABLE_H
#define VEGA_INSTANCE_COMPONENT_RENDERABLE_H

#include <stdint.h>

#include <vega/core/core.h>

#include <vega/engine/asset/material.h>
#include <vega/engine/asset/mesh.h>

VEGA_EXTERN_C_BEGIN

typedef struct _renderable_t
{
	mesh_asset_t* mesh_asset;
	material_asset_t* material_asset;
} renderable_t;

///////////////////////////////////////////////////////////////
// Public API
///////////////////////////////////////////////////////////////

void renderable_init(renderable_t* renderable);
// TODO

VEGA_EXTERN_C_END

#endif // VEGA_INSTANCE_COMPONENT_RENDERABLE_H
