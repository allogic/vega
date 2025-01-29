#include <vega/engine/component/camera.h>
#include <vega/engine/component/transform.h>
#include <vega/engine/component/types.h>

#include <vega/engine/entity/scene_editor.h>

uint64_t entity_scene_editor_create(ecs_t* ecs)
{
	uint64_t entity = std_ecs_create(ecs);

	transform_t transform = transform_identity();

	camera_t camera =
	{
		.mode = CAMERA_MODE_PERSP,
		.fov = math_deg_to_rad(45.0),
		.near_z = 1.0E-6,
		.far_z = 1.0E6,
	};

	std_ecs_attach(ecs, entity, VEGA_COMPONENT_TYPE_TRANSFORM, &transform);
	std_ecs_attach(ecs, entity, VEGA_COMPONENT_TYPE_CAMERA, &camera);

	return entity;
}
