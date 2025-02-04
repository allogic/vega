#ifndef VEGA_ENGINE_ENTITY_SCENE_EDITOR_H
#define VEGA_ENGINE_ENTITY_SCENE_EDITOR_H

#include <stdint.h>

#include <vega/core/core.h>

VEGA_EXTERN_C_BEGIN

///////////////////////////////////////////////////////////////
// Public API
///////////////////////////////////////////////////////////////

uint64_t entity_scene_editor_create(ecs_t* ecs);

VEGA_EXTERN_C_END

#endif // VEGA_ENGINE_ENTITY_SCENE_EDITOR_H
