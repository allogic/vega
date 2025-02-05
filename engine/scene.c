#include <string.h>

#include <vega/engine/engine.h>
#include <vega/engine/scene.h>

#include <vega/engine/asset/loader.h>

#include <vega/engine/component/camera.h>
#include <vega/engine/component/renderable.h>
#include <vega/engine/component/scene_controller.h>
#include <vega/engine/component/transform.h>
#include <vega/engine/component/types.h>

#include <vega/engine/entity/scene_editor.h>

#include <vega/engine/vulkan/renderer.h>

#ifndef TRACY_ZONE_BEGIN
	#define TRACY_ZONE_BEGIN TracyCZoneC(ctx, TRACY_COLOR_GREEN, 1U);
#endif // TRACY_ZONE_BEGIN

#ifndef TRACY_ZONE_END
	#define TRACY_ZONE_END TracyCZoneEnd(ctx);
#endif // TRACY_ZONE_END

static void scene_update_controller_proc(ecs_t* ecs, uint64_t index, uint64_t entity);

vector_t g_scene_stack = { 0 };

static ecs_query_t s_scene_stack_controller_query =
{
	.mask = VEGA_COMPONENT_BIT_TRANSFORM | VEGA_COMPONENT_BIT_SCENE_CONTROLLER,
	.proc = scene_update_controller_proc,
};

void scene_stack_alloc(void)
{
	TRACY_ZONE_BEGIN

	g_scene_stack = std_vector_alloc(sizeof(scene_t));

	TRACY_ZONE_END
}
void scene_stack_free(void)
{
	TRACY_ZONE_BEGIN

	while (std_vector_count(&g_scene_stack) > 0)
	{
		scene_pop();
	}

	std_vector_free(&g_scene_stack);

	TRACY_ZONE_END
}
void scene_push(void)
{
	TRACY_ZONE_BEGIN

	scene_t scene = { 0 };
	scene.ecs = std_ecs_alloc();

	std_ecs_register(&scene.ecs, VEGA_COMPONENT_TYPE_TRANSFORM, sizeof(transform_t));
	std_ecs_register(&scene.ecs, VEGA_COMPONENT_TYPE_CAMERA, sizeof(camera_t));
	std_ecs_register(&scene.ecs, VEGA_COMPONENT_TYPE_RENDERABLE, sizeof(renderable_t));
	std_ecs_register(&scene.ecs, VEGA_COMPONENT_TYPE_SCENE_CONTROLLER, sizeof(scene_controller_t));

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
void scene_update(scene_t* scene)
{
	TRACY_ZONE_BEGIN

	std_ecs_query(&scene->ecs, &s_scene_stack_controller_query);
	std_ecs_for(&scene->ecs, &s_scene_stack_controller_query);

	TRACY_ZONE_END
}
scene_t* scene_current(void)
{
	TRACY_ZONE_BEGIN

	scene_t* current = 0;

	if (std_vector_count(&g_scene_stack) > 0)
	{
		current = std_vector_back(&g_scene_stack);
	}

	TRACY_ZONE_END

	return current;
}
uint64_t scene_create_entity_from_model_asset(scene_t* scene, char const* asset_name)
{
	TRACY_ZONE_BEGIN

	uint64_t root_entity = std_ecs_create(&scene->ecs);

	transform_t* root_transform = (transform_t*)std_ecs_attach(&scene->ecs, root_entity, VEGA_COMPONENT_TYPE_TRANSFORM, 0);
	transform_init(root_transform, 0);

	model_asset_t* model_asset = (model_asset_t*)std_map_get(&g_asset_loader_models, asset_name, strlen(asset_name), 0);

	uint64_t mesh_ref_index = 0;
	uint64_t mesh_ref_count = std_vector_count(&model_asset->mesh_refs);
	while (mesh_ref_index < mesh_ref_count)
	{
		string_t* mesh_ref = std_vector_get(&model_asset->mesh_refs, mesh_ref_index);

		mesh_asset_t* mesh_asset = (mesh_asset_t*)std_map_get(&g_asset_loader_meshes, std_string_buffer(mesh_ref), std_string_size(mesh_ref), 0);
		material_asset_t* material_asset = (material_asset_t*)std_map_get(&g_asset_loader_materials, std_string_buffer(&mesh_asset->material_ref), std_string_size(&mesh_asset->material_ref), 0);

		uint64_t mesh_entity = std_ecs_create(&scene->ecs); // TODO: make parent-child relationship..

		transform_t* mesh_transform = (transform_t*)std_ecs_attach(&scene->ecs, mesh_entity, VEGA_COMPONENT_TYPE_TRANSFORM, 0);
		transform_init(mesh_transform, root_transform);

		renderable_t* mesh_renderable = (renderable_t*)std_ecs_attach(&scene->ecs, mesh_entity, VEGA_COMPONENT_TYPE_RENDERABLE, 0);
		renderable_init(mesh_renderable);
		mesh_renderable->mesh_asset = mesh_asset; // TODO: setter..
		mesh_renderable->material_asset = material_asset; // TODO: setter..

		mesh_ref_index++;
	}

	vulkan_renderer_update_pbr_descriptor_sets(); // TODO

	TRACY_ZONE_END

	return root_entity;
}
void scene_create_entity_from_asset_recursive(scene_t* scene, model_asset_t* model_asset, uint64_t entity)
{
	TRACY_ZONE_BEGIN

	// TODO

	TRACY_ZONE_END
}
static void scene_update_controller_proc(ecs_t* ecs, uint64_t index, uint64_t entity)
{
	TRACY_ZONE_BEGIN

	transform_t* transform = (transform_t*)std_ecs_get(ecs, entity, VEGA_COMPONENT_TYPE_TRANSFORM);
	scene_controller_t* scene_controller = (scene_controller_t*)std_ecs_get(ecs, entity, VEGA_COMPONENT_TYPE_SCENE_CONTROLLER);

	scene_controller_handle_position(scene_controller, transform);
	scene_controller_handle_rotation(scene_controller, transform);

	TRACY_ZONE_END
}
