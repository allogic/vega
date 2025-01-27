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

	engine_alloc();
	scene_push();

	//asset_t* cyberdemon_model = asset_loader_load_model(0, "cyberdemon", "fbx", "C:\\Users\\mialb\\Downloads\\vega\\assets\\cyberdemon", "C:\\Users\\mialb\\Downloads\\vega\\assets\\cyberdemon\\cyberdemon.fbx");
	//asset_t* cockpit_model = asset_loader_load_model(0, "cockpit", "fbx", "C:\\Users\\mialb\\Downloads\\vega\\assets\\cockpit", "C:\\Users\\mialb\\Downloads\\vega\\assets\\cockpit\\cockpit2.fbx");

	platform_window_alloc("VEGA", 1920, 1080);

	while (platform_window_should_not_close())
	{
		platform_window_begin_frame();
		platform_window_poll_events();
		platform_window_render_frame();
		platform_window_begin_frame();
	}

	platform_window_free();

	scene_pop();
	engine_free();

	TRACY_ZONE_END

	return 0;
}
