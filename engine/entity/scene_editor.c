#include <vega/engine/component/camera.h>
#include <vega/engine/component/scene_controller.h>
#include <vega/engine/component/transform.h>
#include <vega/engine/component/types.h>

#include <vega/engine/entity/scene_editor.h>

uint64_t entity_scene_editor_create(ecs_t* ecs)
{
	uint64_t entity = std_ecs_create(ecs);

	transform_t transform = transform_identity();

	transform_set_position_xyz(&transform, 0.0, 0.0, -1000.0);

	camera_t camera = { 0 };
	camera.mode = CAMERA_MODE_PERSP;
	camera.fov = math_deg_to_rad(45.0);
	camera.near_z = 1.0E-6;
	camera.far_z = 1.0E6;

	scene_controller_t scene_controller = { 0 };
	scene_controller.velocity = math_vector3_zero();
	scene_controller.mouse_position_start = math_vector2_zero();
	scene_controller.mouse_position_delta = math_vector2_zero();
	scene_controller.keyboard_move_speed_fast = 100.0;
	scene_controller.keyboard_move_speed_normal = 0.1;
	scene_controller.mouse_move_speed_fast = 5.0;
	scene_controller.mouse_move_speed_normal = 0.005;
	scene_controller.mouse_rotation_speed_fast = 5.0;
	scene_controller.mouse_rotation_speed_normal = 2.0;
	scene_controller.move_drag = 5.0;
	scene_controller.rotation_drag = 20.0;

	std_ecs_attach(ecs, entity, VEGA_COMPONENT_TYPE_TRANSFORM, &transform);
	std_ecs_attach(ecs, entity, VEGA_COMPONENT_TYPE_CAMERA, &camera);
	std_ecs_attach(ecs, entity, VEGA_COMPONENT_TYPE_SCENE_CONTROLLER, &scene_controller);

	return entity;
}
