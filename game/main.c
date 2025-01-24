#include <assert.h>

#include <windows.h> // TODO

#include <vega/engine/engine.h>

#ifndef TRACY_ZONE_BEGIN
	#define TRACY_ZONE_BEGIN TracyCZoneC(ctx, TRACY_COLOR_GREEN, 1U)
#endif // TRACY_ZONE_BEGIN

#ifndef TRACY_ZONE_END
	#define TRACY_ZONE_END TracyCZoneEnd(ctx)
#endif // TRACY_ZONE_END

static void ecs_for_proc(ecs_t* ecs, uint64_t entity, vector_t* view);

static ecs_t s_ecs = { 0 };

int main(int argc, char** argv, char** envp)
{
	TRACY_ZONE_BEGIN

	{
		{
			vector2_t c = math_vector2_zero();
			assert(c.x == 0.0);
			assert(c.y == 0.0);
		}
		{
			vector2_t c = math_vector2_set(1.0, 2.0);
			assert(c.x == 1.0);
			assert(c.y == 2.0);
		}
		{
			vector2_t a = math_vector2_set(3.0, 4.0);
			vector2_t b = math_vector2_set(2.0, 1.0);
			vector2_t c = math_vector2_add(a, b);
			assert(c.x == 5.0);
			assert(c.y == 5.0);
		}
		{
			vector2_t a = math_vector2_set(3.0, 4.0);
			vector2_t b = math_vector2_set(2.0, 1.0);
			vector2_t c = math_vector2_sub(a, b);
			assert(c.x == 1.0);
			assert(c.y == 3.0);
		}
		{
			vector2_t a = math_vector2_set(3.0, 4.0);
			vector2_t b = math_vector2_set(2.0, 1.0);
			vector2_t c = math_vector2_mul(a, b);
			assert(c.x == 6.0);
			assert(c.y == 4.0);
		}
		{
			vector2_t a = math_vector2_set(4.0, 8.0);
			vector2_t b = math_vector2_set(2.0, 2.0);
			vector2_t c = math_vector2_div(a, b);
			assert(c.x == 2.0);
			assert(c.y == 4.0);
		}
		{
			vector2_t a = math_vector2_set(3.0, 4.0);
			vector2_t c = math_vector2_add_scalar(a, 2.0);
			assert(c.x == 5.0);
			assert(c.y == 6.0);
		}
		{
			vector2_t a = math_vector2_set(3.0, 4.0);
			vector2_t c = math_vector2_sub_scalar(a, 2.0);
			assert(c.x == 1.0);
			assert(c.y == 2.0);
		}
		{
			vector2_t a = math_vector2_set(3.0, 4.0);
			vector2_t c = math_vector2_mul_scalar(a, 2.0);
			assert(c.x == 6.0);
			assert(c.y == 8.0);
		}
		{
			vector2_t a = math_vector2_set(4.0, 8.0);
			vector2_t c = math_vector2_div_scalar(a, 2.0);
			assert(c.x == 2.0);
			assert(c.y == 4.0);
		}
		{
			vector2_t a = math_vector2_set(3.0, 4.0);
			vector2_t b = math_vector2_set(2.0, 1.0);
			double c = math_vector2_dot(a, b);
			assert(c == 10.0);
		}
		{
			vector2_t a = math_vector2_set(3.0, 4.0);
			double c = math_vector2_length(a);
			assert(c == 5.0);
		}
		{
			vector2_t a = math_vector2_set(3.0, 4.0);
			double c = math_vector2_length2(a);
			assert(c == 25.0);
		}
	}

	{
		{
			vector3_t c = math_vector3_zero();
			assert(c.x == 0.0);
			assert(c.y == 0.0);
			assert(c.z == 0.0);
		}
		{
			vector3_t c = math_vector3_set(1.0, 2.0, 3.0);
			assert(c.x == 1.0);
			assert(c.y == 2.0);
			assert(c.z == 3.0);
		}
		{
			vector3_t a = math_vector3_set(3.0, 4.0, 2.0);
			vector3_t b = math_vector3_set(2.0, 1.0, 3.0);
			vector3_t c = math_vector3_add(a, b);
			assert(c.x == 5.0);
			assert(c.y == 5.0);
			assert(c.z == 5.0);
		}
		{
			vector3_t a = math_vector3_set(3.0, 4.0, 2.0);
			vector3_t b = math_vector3_set(2.0, 1.0, 3.0);
			vector3_t c = math_vector3_sub(a, b);
			assert(c.x == 1.0);
			assert(c.y == 3.0);
			assert(c.z == -1.0);
		}
		{
			vector3_t a = math_vector3_set(3.0, 4.0, 2.0);
			vector3_t b = math_vector3_set(2.0, 1.0, 3.0);
			vector3_t c = math_vector3_mul(a, b);
			assert(c.x == 6.0);
			assert(c.y == 4.0);
			assert(c.z == 6.0);
		}
		{
			vector3_t a = math_vector3_set(4.0, 8.0, 16.0);
			vector3_t b = math_vector3_set(2.0, 2.0, 2.0);
			vector3_t c = math_vector3_div(a, b);
			assert(c.x == 2.0);
			assert(c.y == 4.0);
			assert(c.z == 8.0);
		}
		{
			vector3_t a = math_vector3_set(3.0, 4.0, 2.0);
			vector3_t c = math_vector3_add_scalar(a, 2.0);
			assert(c.x == 5.0);
			assert(c.y == 6.0);
			assert(c.z == 4.0);
		}
		{
			vector3_t a = math_vector3_set(3.0, 4.0, 2.0);
			vector3_t c = math_vector3_sub_scalar(a, 2.0);
			assert(c.x == 1.0);
			assert(c.y == 2.0);
			assert(c.z == 0.0);
		}
		{
			vector3_t a = math_vector3_set(3.0, 4.0, 2.0);
			vector3_t c = math_vector3_mul_scalar(a, 2.0);
			assert(c.x == 6.0);
			assert(c.y == 8.0);
			assert(c.z == 4.0);
		}
		{
			vector3_t a = math_vector3_set(4.0, 8.0, 2.0);
			vector3_t c = math_vector3_div_scalar(a, 2.0);
			assert(c.x == 2.0);
			assert(c.y == 4.0);
			assert(c.z == 1.0);
		}
		{
			vector3_t a = math_vector3_set(3.0, 4.0, 2.0);
			vector3_t b = math_vector3_set(2.0, 1.0, 3.0);
			double c = math_vector3_dot(a, b);
			assert(c == 16.0);
		}
		{
			vector3_t a = math_vector3_set(3.0, 4.0, 2.0);
			double c = math_vector3_length(a);
			assert(c == 5.3851648071345037);
		}
		{
			vector3_t a = math_vector3_set(3.0, 4.0, 2.0);
			double c = math_vector3_length2(a);
			assert(c == 29.0);
		}
		{
			vector3_t a = math_vector3_set(1.0, 0.0, 0.0);
			vector3_t b = math_vector3_set(0.0, 1.0, 0.0);
			vector3_t c = math_vector3_cross(a, b);
			assert(c.x == 0.0);
			assert(c.y == 0.0);
			assert(c.z == 1.0);
		}
	}

	{
		{
			vector4_t c = math_vector4_zero();
			assert(c.x == 0.0);
			assert(c.y == 0.0);
			assert(c.z == 0.0);
			assert(c.w == 0.0);
		}
		{
			vector4_t c = math_vector4_set(1.0, 2.0, 3.0, 4.0);
			assert(c.x == 1.0);
			assert(c.y == 2.0);
			assert(c.z == 3.0);
			assert(c.w == 4.0);
		}
		{
			vector4_t a = math_vector4_set(3.0, 4.0, 2.0, 5.0);
			vector4_t b = math_vector4_set(2.0, 1.0, 3.0, 6.0);
			vector4_t c = math_vector4_add(a, b);
			assert(c.x == 5.0);
			assert(c.y == 5.0);
			assert(c.z == 5.0);
			assert(c.w == 11.0);
		}
		{
			vector4_t a = math_vector4_set(3.0, 4.0, 2.0, 5.0);
			vector4_t b = math_vector4_set(2.0, 1.0, 3.0, 6.0);
			vector4_t c = math_vector4_sub(a, b);
			assert(c.x == 1.0);
			assert(c.y == 3.0);
			assert(c.z == -1.0);
			assert(c.w == -1.0);
		}
		{
			vector4_t a = math_vector4_set(3.0, 4.0, 2.0, 5.0);
			vector4_t b = math_vector4_set(2.0, 1.0, 3.0, 6.0);
			vector4_t c = math_vector4_mul(a, b);
			assert(c.x == 6.0);
			assert(c.y == 4.0);
			assert(c.z == 6.0);
			assert(c.w == 30.0);
		}
		{
			vector4_t a = math_vector4_set(4.0, 8.0, 16.0, 32.0);
			vector4_t b = math_vector4_set(2.0, 2.0, 2.0, 2.0);
			vector4_t c = math_vector4_div(a, b);
			assert(c.x == 2.0);
			assert(c.y == 4.0);
			assert(c.z == 8.0);
			assert(c.w == 16.0);
		}
		{
			vector4_t a = math_vector4_set(3.0, 4.0, 2.0, 5.0);
			vector4_t c = math_vector4_add_scalar(a, 2.0);
			assert(c.x == 5.0);
			assert(c.y == 6.0);
			assert(c.z == 4.0);
			assert(c.w == 7.0);
		}
		{
			vector4_t a = math_vector4_set(3.0, 4.0, 2.0, 5.0);
			vector4_t c = math_vector4_sub_scalar(a, 2.0);
			assert(c.x == 1.0);
			assert(c.y == 2.0);
			assert(c.z == 0.0);
			assert(c.w == 3.0);
		}
		{
			vector4_t a = math_vector4_set(3.0, 4.0, 2.0, 5.0);
			vector4_t c = math_vector4_mul_scalar(a, 2.0);
			assert(c.x == 6.0);
			assert(c.y == 8.0);
			assert(c.z == 4.0);
			assert(c.w == 10.0);
		}
		{
			vector4_t a = math_vector4_set(4.0, 8.0, 2.0, 6.0);
			vector4_t c = math_vector4_div_scalar(a, 2.0);
			assert(c.x == 2.0);
			assert(c.y == 4.0);
			assert(c.z == 1.0);
			assert(c.w == 3.0);
		}
	}

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

	while (g_window_should_close == 0)
	{
		platform_window_poll_events();

		std_ecs_for(&s_ecs, &view, ecs_for_proc);

		//Sleep(10);
	}

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
