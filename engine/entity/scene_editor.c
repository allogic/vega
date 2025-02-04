#include <vega/engine/component/camera.h>
#include <vega/engine/component/scene_controller.h>
#include <vega/engine/component/transform.h>
#include <vega/engine/component/types.h>

#include <vega/engine/entity/scene_editor.h>

uint64_t entity_scene_editor_create(ecs_t* ecs)
{
	uint64_t entity = std_ecs_create(ecs);

	transform_t* transform = (transform_t*)std_ecs_attach(ecs, entity, VEGA_COMPONENT_TYPE_TRANSFORM, 0);
	transform_identity(transform, 0);
	transform_set_position_xyz(transform, 0.0, 0.0, -1000.0);

	camera_t* camera = (camera_t*)std_ecs_attach(ecs, entity, VEGA_COMPONENT_TYPE_CAMERA, 0);
	camera->mode = CAMERA_MODE_PERSP;
	camera->fov = math_deg_to_rad(45.0);
	camera->near_z = 1.0E-3;
	camera->far_z = 1.0E4;

	scene_controller_t* scene_controller = (scene_controller_t*)std_ecs_attach(ecs, entity, VEGA_COMPONENT_TYPE_SCENE_CONTROLLER, 0);
	scene_controller->velocity = math_vector3_zero();
	scene_controller->mouse_position_begin = math_vector3_zero();
	scene_controller->mouse_position_end = math_vector3_zero();
	scene_controller->mouse_position_delta = math_vector3_zero();
	scene_controller->keyboard_move_speed_fast = 20.0;
	scene_controller->keyboard_move_speed_normal = 5.0;
	scene_controller->mouse_move_speed_fast = 5.0;
	scene_controller->mouse_move_speed_normal = 0.005;
	scene_controller->mouse_rotation_speed_fast = 0.0001;
	scene_controller->mouse_rotation_speed_normal = 0.0001;
	scene_controller->move_drag = 5.0;
	scene_controller->rotation_drag = 20.0;

	return entity;
}
