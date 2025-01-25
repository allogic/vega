#include <stdio.h>

#include <windows.h> // TODO

#include <vega/engine/engine.h>

#ifndef TRACY_ZONE_BEGIN
	#define TRACY_ZONE_BEGIN TracyCZoneC(ctx, TRACY_COLOR_GREEN, 1U);
#endif // TRACY_ZONE_BEGIN

#ifndef TRACY_ZONE_END
	#define TRACY_ZONE_END TracyCZoneEnd(ctx);
#endif // TRACY_ZONE_END

static void ecs_for_proc(ecs_t* ecs, uint64_t entity, vector_t* view);

static ecs_t s_ecs = { 0 };

int main(int argc, char** argv, char** envp)
{
	TRACY_ZONE_BEGIN

	VEGA_MATH_TESTS

	s_ecs = std_ecs_alloc();

	std_ecs_register(&s_ecs, 0, sizeof(uint64_t));
	std_ecs_register(&s_ecs, 1, sizeof(uint64_t));
	std_ecs_register(&s_ecs, 2, sizeof(uint64_t));
	std_ecs_register(&s_ecs, 3, sizeof(uint64_t));
	std_ecs_register(&s_ecs, 4, sizeof(uint64_t));
	std_ecs_register(&s_ecs, 5, sizeof(uint64_t));
	std_ecs_register(&s_ecs, 6, sizeof(uint64_t));

	uint64_t component_0 = 42;
	uint64_t component_1 = 43;
	uint64_t component_2 = 44;
	uint64_t component_3 = 45;
	uint64_t component_4 = 46;
	uint64_t component_5 = 47;
	uint64_t component_6 = 48;

	uint64_t entity_index = 0;
	uint64_t entity_count = 100;
	while (entity_index < entity_count)
	{
		uint64_t entity = std_ecs_create(&s_ecs);

		std_ecs_attach(&s_ecs, entity, 0, &component_0);
		std_ecs_attach(&s_ecs, entity, 1, &component_1);
		std_ecs_attach(&s_ecs, entity, 2, &component_2);
		std_ecs_attach(&s_ecs, entity, 3, &component_3);
		std_ecs_attach(&s_ecs, entity, 4, &component_4);
		std_ecs_attach(&s_ecs, entity, 5, &component_5);
		std_ecs_attach(&s_ecs, entity, 6, &component_6);

		entity_index++;
	}

	vector_t view = std_ecs_all(&s_ecs, 0b1111111);

	platform_window_alloc("VEGA", 1920, 1080);
	
	vulkan_instance_alloc();
	vulkan_swap_chain_alloc();

	while (g_platform_window_should_close == 0)
	{
		platform_window_poll_events();

		std_ecs_for(&s_ecs, &view, ecs_for_proc);

		//Sleep(10);
	}

	vulkan_swap_chain_free();
	vulkan_instance_free();

	platform_window_free();

	std_ecs_free(&s_ecs);

	TRACY_ZONE_END

	return 0;
}
static void ecs_for_proc(ecs_t* ecs, uint64_t entity, vector_t* view)
{
	TRACY_ZONE_BEGIN

	uint64_t* component_0 = (uint64_t*)std_ecs_value(ecs, entity, 0, view);
	uint64_t* component_1 = (uint64_t*)std_ecs_value(ecs, entity, 1, view);
	uint64_t* component_2 = (uint64_t*)std_ecs_value(ecs, entity, 2, view);
	uint64_t* component_3 = (uint64_t*)std_ecs_value(ecs, entity, 3, view);
	uint64_t* component_4 = (uint64_t*)std_ecs_value(ecs, entity, 4, view);
	uint64_t* component_5 = (uint64_t*)std_ecs_value(ecs, entity, 5, view);
	uint64_t* component_6 = (uint64_t*)std_ecs_value(ecs, entity, 6, view);

	TRACY_ZONE_END
}
