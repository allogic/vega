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

int main(int argc, char** argv, char** envp)
{
	TRACY_ZONE_BEGIN

	platform_window_alloc("VEGA", 1920, 1080);

	asset_loader_alloc();

	asset_loader_load_model("cyberdemon", "cyberdemon", "fbx", "C:\\Users\\mialb\\Downloads\\vega\\assets\\cyberdemon", "C:\\Users\\mialb\\Downloads\\vega\\assets\\cyberdemon\\cyberdemon.fbx");

	scene_stack_alloc();

	scene_push();

	scene_t* scene = scene_current();

	if (scene)
	{
		//uint64_t cyberdemon_entity = scene_create_entity_from_model_asset(scene, 0);
	}

	while (platform_window_should_not_close())
	{
		platform_window_begin_frame();
		platform_window_poll_events();
		platform_window_render_frame();
		platform_window_begin_frame();
	}

	scene_pop();

	scene_stack_free();

	asset_loader_free();

	platform_window_free();

	assert(g_heap_allocated_bytes == 0);

	TRACY_ZONE_END

	return 0;
}
