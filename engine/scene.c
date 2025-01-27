#include <string.h>

#include <vega/engine/engine.h>
#include <vega/engine/scene.h>

#include <vega/engine/component/camera.h>
#include <vega/engine/component/transform.h>
#include <vega/engine/component/types.h>

#include <vega/engine/entity/scene_editor.h>

#ifndef TRACY_ZONE_BEGIN
	#define TRACY_ZONE_BEGIN TracyCZoneC(ctx, TRACY_COLOR_GREEN, 1U);
#endif // TRACY_ZONE_BEGIN

#ifndef TRACY_ZONE_END
	#define TRACY_ZONE_END TracyCZoneEnd(ctx);
#endif // TRACY_ZONE_END

vector_t g_scene_stack = { 0 };

void scene_stack_alloc(void)
{
	TRACY_ZONE_BEGIN

	g_scene_stack = std_vector_alloc(sizeof(scene_t));

	TRACY_ZONE_END
}
void scene_stack_free(void)
{
	TRACY_ZONE_BEGIN

	std_vector_free(&g_scene_stack);

	TRACY_ZONE_END
}
void scene_push(void)
{
	TRACY_ZONE_BEGIN

	scene_t scene;
	memset(&scene, 0, sizeof(scene_t));

	scene.ecs = std_ecs_alloc();

	std_ecs_register(&scene.ecs, COMPONENT_TYPE_TRANSFORM, sizeof(transform_t));
	std_ecs_register(&scene.ecs, COMPONENT_TYPE_CAMERA, sizeof(camera_t));

	scene.scene_editor = entity_scene_editor_create(&scene.ecs);

	std_vector_push(&g_scene_stack, &scene);

	TRACY_ZONE_END
}
void scene_pop(void)
{
	TRACY_ZONE_BEGIN

	scene_t scene;

	std_vector_pop(&g_scene_stack, &scene);

	std_ecs_free(&scene.ecs);

	TRACY_ZONE_END
}
scene_t* scene_current(void)
{
	TRACY_ZONE_BEGIN

	scene_t* current = std_vector_back(&g_scene_stack);

	TRACY_ZONE_END

	return current;
}
