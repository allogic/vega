#include <assert.h>
#include <stdio.h>

#include <vega/engine/engine.h>

#include <vega/game/game.h>

#ifndef TRACY_ZONE_BEGIN
	#define TRACY_ZONE_BEGIN TracyCZoneC(ctx, TRACY_COLOR_BLUE, 1U);
#endif // TRACY_ZONE_BEGIN

#ifndef TRACY_ZONE_END
	#define TRACY_ZONE_END TracyCZoneEnd(ctx);
#endif // TRACY_ZONE_END

static void game_update_proc(ecs_t* ecs, uint64_t index, uint64_t entity);

static ecs_query_t s_game_update_query = { VEGA_COMPONENT_BIT_TRANSFORM | VEGA_COMPONENT_BIT_RENDERABLE };

int main(int argc, char** argv, char** envp)
{
	TRACY_ZONE_BEGIN

	platform_window_alloc("VEGA", 1920, 1080);

	asset_loader_alloc();

	asset_loader_load_model("cyberdemon", "cyberdemon", "fbx", "C:\\Users\\mialb\\Downloads\\vega\\assets\\cyberdemon", "C:\\Users\\mialb\\Downloads\\vega\\assets\\cyberdemon\\cyberdemon.fbx");
	//asset_loader_load_model("cockpit", "cockpit", "fbx", "C:\\Users\\mialb\\Downloads\\vega\\assets\\cockpit", "C:\\Users\\mialb\\Downloads\\vega\\assets\\cockpit\\cockpit.fbx");

	scene_stack_alloc();

	scene_push();

	scene_t* scene = scene_current();

	if (scene)
	{
		uint64_t sector_index = 0;
		uint64_t sector_count = 32;
		while (sector_index < sector_count)
		{
			double sector_radius = 3000.0;
			double sector_step = MATH_TAU / sector_count;
			double sector_angle = sector_step * sector_index;
			double sector_x = cos(sector_angle) * sector_radius;
			double sector_z = sin(sector_angle) * sector_radius;

			uint64_t cyberdemon_entity = scene_create_entity_from_model_asset(scene, "cyberdemon");

			transform_t* transform = (transform_t*)std_ecs_get(&scene->ecs, cyberdemon_entity, VEGA_COMPONENT_TYPE_TRANSFORM);

			transform_set_position_xyz(transform, sector_x, 0.0, sector_z);
			//transform_set_euler_angles_pyr(transform, -90.0, 0.0, 0.0);

			sector_index++;
		}
	}

	while (g_platform_window_should_close == 0)
	{
		platform_window_begin_frame();

		platform_window_poll_events();

		scene_t* scene = scene_current();

		if (scene)
		{
			scene_update(scene);

			std_ecs_query(&scene->ecs, &s_game_update_query);
			std_ecs_for(&scene->ecs, &s_game_update_query, game_update_proc);
		}

		{
			vulkan_renderer_begin();

			{
				vulkan_renderer_compute_begin();
				vulkan_renderer_compute_end();
			}

			{
				vulkan_renderer_graphic_begin();

				vulkan_renderer_geometry_pass_begin();
				vulkan_renderer_geometry_pass();
				vulkan_renderer_imgui_pass(); // TODO: refactor this later..
				vulkan_renderer_geometry_pass_end();

				vulkan_renderer_graphic_end();
			}

			vulkan_renderer_end();
		}

		platform_window_end_frame();
	}

	scene_pop();

	scene_stack_free();

	asset_loader_free();

	platform_window_free();

	VEGA_ASSERT(g_heap_allocated_bytes == 0, "Memory leak detected");

	TRACY_ZONE_END

	return 0;
}
static void game_update_proc(ecs_t* ecs, uint64_t index, uint64_t entity)
{
	TRACY_ZONE_BEGIN

	transform_t* transform = (transform_t*)std_ecs_get(ecs, entity, VEGA_COMPONENT_TYPE_TRANSFORM);
	renderable_t* renderable = (renderable_t*)std_ecs_get(ecs, entity, VEGA_COMPONENT_TYPE_RENDERABLE);

	static double yaw = 0.0;

	yaw += 0.001;

	transform_set_euler_angles_pyr(transform, 0.0, yaw, 0.0);

	TRACY_ZONE_END
}
