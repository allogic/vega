#ifndef VEGA_ENGINE_SCENE_H
#define VEGA_ENGINE_SCENE_H

#include <stdint.h>

#include <vega/core/core.h>

#include <vega/engine/asset/loader.h>

typedef struct _scene_t
{
	ecs_t ecs;
	uint64_t scene_editor;
} scene_t;

extern vector_t g_scene_stack;

///////////////////////////////////////////////////////////////
// Public API
///////////////////////////////////////////////////////////////

void scene_stack_alloc(void);
void scene_stack_free(void);

void scene_push(void);
void scene_pop(void);

scene_t* scene_current(void);

uint64_t scene_create_entity_from_model_asset(scene_t* scene, model_asset_t* model_asset);

///////////////////////////////////////////////////////////////
// Internal API
///////////////////////////////////////////////////////////////

void scene_create_entity_from_asset_recursive(scene_t* scene, model_asset_t* model_asset, uint64_t entity);

#endif // VEGA_ENGINE_SCENE_H
