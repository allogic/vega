#include <vega/engine/component/scene_controller.h>

#include <vega/engine/platform/event.h>
#include <vega/engine/platform/window.h>

#ifndef TRACY_ZONE_BEGIN
	#define TRACY_ZONE_BEGIN TracyCZoneC(ctx, TRACY_COLOR_GREEN, 1U);
#endif // TRACY_ZONE_BEGIN

#ifndef TRACY_ZONE_END
	#define TRACY_ZONE_END TracyCZoneEnd(ctx);
#endif // TRACY_ZONE_END

void scene_controller_init(scene_controller_t* scene_controller)
{
	TRACY_ZONE_BEGIN

	scene_controller->velocity = math_vector3_zero();
	scene_controller->mouse_begin = math_vector3_zero();
	scene_controller->mouse_end = math_vector3_zero();
	scene_controller->mouse_delta = math_vector3_zero();
	scene_controller->keyboard_move_speed_fast = 2000.0;
	scene_controller->keyboard_move_speed_normal = 500.0;
	scene_controller->mouse_move_speed_fast = 20.0;
	scene_controller->mouse_move_speed_normal = 5.0;
	scene_controller->mouse_rotation_speed = 0.1;
	scene_controller->move_drag = 2.5;
	scene_controller->rotation_drag = 50.0;

	TRACY_ZONE_END
}
void scene_controller_handle_position(scene_controller_t* scene_controller, transform_t* transform)
{
	TRACY_ZONE_BEGIN

	double keyboard_move_speed = platform_event_is_keyboard_key_held(KEYBOARD_KEY_LEFT_SHIFT)
		? scene_controller->keyboard_move_speed_fast
		: scene_controller->keyboard_move_speed_normal;

	if (platform_event_is_keyboard_key_held(KEYBOARD_KEY_D))
	{
		vector3_t right_direction = math_vector3_mul_scalar(transform->local_right, keyboard_move_speed);

		right_direction = math_vector3_mul_scalar(right_direction, g_platform_window_delta_time);

		transform_set_relative_position(transform, right_direction);
	}

	if (platform_event_is_keyboard_key_held(KEYBOARD_KEY_A))
	{
		vector3_t left_direction = math_vector3_mul_scalar(transform->local_left, keyboard_move_speed);

		left_direction = math_vector3_mul_scalar(left_direction, g_platform_window_delta_time);

		transform_set_relative_position(transform, left_direction);
	}
	if (platform_event_is_keyboard_key_held(KEYBOARD_KEY_E))
	{
		vector3_t down_direction = math_vector3_mul_scalar(transform->local_down, keyboard_move_speed);

		down_direction = math_vector3_mul_scalar(down_direction, g_platform_window_delta_time);

		transform_set_relative_position(transform, down_direction);
	}
	if (platform_event_is_keyboard_key_held(KEYBOARD_KEY_Q))
	{
		vector3_t up_direction = math_vector3_mul_scalar(transform->local_up, keyboard_move_speed);

		up_direction = math_vector3_mul_scalar(up_direction, g_platform_window_delta_time);

		transform_set_relative_position(transform, up_direction);
	}
	if (platform_event_is_keyboard_key_held(KEYBOARD_KEY_W))
	{
		vector3_t front_direction = math_vector3_mul_scalar(transform->local_front, keyboard_move_speed);

		front_direction = math_vector3_mul_scalar(front_direction, g_platform_window_delta_time);

		transform_set_relative_position(transform, front_direction);
	}
	if (platform_event_is_keyboard_key_held(KEYBOARD_KEY_S))
	{
		vector3_t back_direction = math_vector3_mul_scalar(transform->local_back, keyboard_move_speed);

		back_direction = math_vector3_mul_scalar(back_direction, g_platform_window_delta_time);

		transform_set_relative_position(transform, back_direction);
	}

	/* TODO
	vector3_t negative_velocity = scene_controller->velocity;

	negative_velocity.x = -negative_velocity.x;
	negative_velocity.y = -negative_velocity.y;
	negative_velocity.z = -negative_velocity.z;

	negative_velocity = math_vector3_mul_scalar(negative_velocity, scene_controller->move_drag);
	negative_velocity = math_vector3_mul_scalar(negative_velocity, g_platform_window_delta_time);

	scene_controller->velocity = math_vector3_add(scene_controller->velocity, negative_velocity);

	vector3_t rotated_velocity = math_vector3_rotate(scene_controller->velocity, transform->local_rotation);

	transform_set_relative_position(transform, rotated_velocity);
	*/

	TRACY_ZONE_END
}
void scene_controller_handle_rotation(scene_controller_t* scene_controller, transform_t* transform)
{
	TRACY_ZONE_BEGIN

	if (platform_event_is_mouse_key_pressed(MOUSE_KEY_RIGHT))
	{
		scene_controller->mouse_begin = math_vector3_xyz(g_platform_window_mouse_position_x, g_platform_window_mouse_position_y, 0.0);
	}
	else if (platform_event_is_mouse_key_held(MOUSE_KEY_RIGHT) && platform_event_is_mouse_key_held(MOUSE_KEY_LEFT))
	{
		double mouse_move_speed = platform_event_is_keyboard_key_held(KEYBOARD_KEY_LEFT_SHIFT)
			? scene_controller->mouse_move_speed_fast
			: scene_controller->mouse_move_speed_normal;

		scene_controller->mouse_end = math_vector3_xyz(g_platform_window_mouse_position_x, g_platform_window_mouse_position_y, 0.0);
		scene_controller->mouse_delta = math_vector3_sub(scene_controller->mouse_begin, scene_controller->mouse_end);

		double x_delta = scene_controller->mouse_delta.x * mouse_move_speed * g_platform_window_delta_time;
		double y_delta = scene_controller->mouse_delta.y * mouse_move_speed * g_platform_window_delta_time;

		vector3_t change_in_x = math_vector3_mul_scalar(transform->local_right, x_delta);
		vector3_t change_in_y = math_vector3_mul_scalar(transform->local_up, y_delta);

		transform_set_relative_position(transform, change_in_x);
		transform_set_relative_position(transform, change_in_y);
	}
	else if (platform_event_is_mouse_key_held(MOUSE_KEY_RIGHT))
	{
		scene_controller->mouse_end = math_vector3_xyz(g_platform_window_mouse_position_x, g_platform_window_mouse_position_y, 0.0);
		scene_controller->mouse_delta = math_vector3_sub(scene_controller->mouse_begin, scene_controller->mouse_end);

		double p_delta = scene_controller->mouse_delta.y * scene_controller->mouse_rotation_speed * g_platform_window_delta_time;
		double y_delta = -scene_controller->mouse_delta.x * scene_controller->mouse_rotation_speed * g_platform_window_delta_time;

		quaternion_t local_rotation = transform_get_local_rotation(transform);

		quaternion_t qx = math_quaternion_angle_axis(p_delta, g_world_right);
		quaternion_t qy = math_quaternion_angle_axis(y_delta, g_world_up);

		local_rotation = math_quaternion_mul(qy, local_rotation);
		local_rotation = math_quaternion_mul(local_rotation, qx);

		transform_set_rotation(transform, local_rotation);
	}

	vector3_t mouse_damping = math_vector3_mul_scalar(scene_controller->mouse_delta, scene_controller->rotation_drag);

	mouse_damping = math_vector3_mul_scalar(mouse_damping, g_platform_window_delta_time);

	scene_controller->mouse_begin = math_vector3_sub(scene_controller->mouse_begin, mouse_damping);

	TRACY_ZONE_END
}
