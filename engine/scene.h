#ifndef VEGA_ENGINE_SCENE_H
#define VEGA_ENGINE_SCENE_H

#include <stdint.h>

#include <vega/core/core.h>

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

#endif // VEGA_ENGINE_SCENE_H
