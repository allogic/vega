#include <vega/engine/component/camera.h>
#include <vega/engine/component/scene_controller.h>
#include <vega/engine/component/transform.h>
#include <vega/engine/component/types.h>

#include <vega/engine/entity/scene_editor.h>

uint64_t entity_scene_editor_create(ecs_t* ecs)
{
	uint64_t entity = std_ecs_create(ecs);

	transform_t* transform = (transform_t*)std_ecs_attach(ecs, entity, VEGA_COMPONENT_TYPE_TRANSFORM, 0);
	transform_init(transform, 0);
	transform_set_position_xyz(transform, 0.0, 0.0, 0.0);
	transform_set_euler_angles_pyr(transform, 0.0, 0.0, 0.0);

	camera_t* camera = (camera_t*)std_ecs_attach(ecs, entity, VEGA_COMPONENT_TYPE_CAMERA, 0);
	camera_init(camera);

	scene_controller_t* scene_controller = (scene_controller_t*)std_ecs_attach(ecs, entity, VEGA_COMPONENT_TYPE_SCENE_CONTROLLER, 0);
	scene_controller_init(scene_controller);

	return entity;
}
