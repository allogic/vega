#include <vega/engine/component/scene_controller.h>

#include <vega/engine/platform/event.h>
#include <vega/engine/platform/window.h>

#ifndef TRACY_ZONE_BEGIN
	#define TRACY_ZONE_BEGIN TracyCZoneC(ctx, TRACY_COLOR_GREEN, 1U);
#endif // TRACY_ZONE_BEGIN

#ifndef TRACY_ZONE_END
	#define TRACY_ZONE_END TracyCZoneEnd(ctx);
#endif // TRACY_ZONE_END

void scene_controller_handle_position(scene_controller_t* scene_controller, transform_t* transform)
{
	TRACY_ZONE_BEGIN

	double keyboard_move_speed = platform_event_is_keyboard_key_held(KEYBOARD_KEY_LEFT_SHIFT)
		? scene_controller->keyboard_move_speed_fast
		: scene_controller->keyboard_move_speed_normal;

	if (platform_event_is_keyboard_key_held(KEYBOARD_KEY_D))
	{
		vector3_t right_direction = math_vector3_mul_scalar(transform->local_right, keyboard_move_speed);

		scene_controller->velocity = math_vector3_add(scene_controller->velocity, right_direction);

		transform_set_relative_position(transform, right_direction);
	}

	if (platform_event_is_keyboard_key_held(KEYBOARD_KEY_A))
	{
		vector3_t left_direction = math_vector3_mul_scalar(transform->local_left, keyboard_move_speed);

		scene_controller->velocity = math_vector3_add(scene_controller->velocity, left_direction);

		transform_set_relative_position(transform, left_direction);
	}
	if (platform_event_is_keyboard_key_held(KEYBOARD_KEY_E))
	{
		vector3_t down_direction = math_vector3_mul_scalar(g_world_down, keyboard_move_speed);

		scene_controller->velocity = math_vector3_add(scene_controller->velocity, down_direction);

		transform_set_relative_position(transform, down_direction);
	}
	if (platform_event_is_keyboard_key_held(KEYBOARD_KEY_Q))
	{
		vector3_t up_direction = math_vector3_mul_scalar(g_world_up, keyboard_move_speed);

		scene_controller->velocity = math_vector3_add(scene_controller->velocity, up_direction);

		transform_set_relative_position(transform, up_direction);
	}
	if (platform_event_is_keyboard_key_held(KEYBOARD_KEY_W))
	{
		vector3_t front_direction = math_vector3_mul_scalar(transform->local_front, keyboard_move_speed);

		scene_controller->velocity = math_vector3_add(scene_controller->velocity, front_direction);

		transform_set_relative_position(transform, front_direction);
	}
	if (platform_event_is_keyboard_key_held(KEYBOARD_KEY_S))
	{
		vector3_t back_direction = math_vector3_mul_scalar(transform->local_back, keyboard_move_speed);

		scene_controller->velocity = math_vector3_add(scene_controller->velocity, back_direction);

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
		double mouse_x = (double)g_platform_window_mouse_position_x;
		double mouse_y = (double)g_platform_window_mouse_position_y;

		scene_controller->mouse_position_begin = math_vector3_xyz(mouse_x, mouse_y, 0.0);
	}

	if (platform_event_is_mouse_key_held(MOUSE_KEY_RIGHT) && platform_event_is_mouse_key_held(MOUSE_KEY_LEFT))
	{
		double mouse_x = (double)g_platform_window_mouse_position_x;
		double mouse_y = (double)g_platform_window_mouse_position_y;

		scene_controller->mouse_position_end = math_vector3_xyz(mouse_x, mouse_y, 0.0);
		scene_controller->mouse_position_delta = math_vector3_sub(scene_controller->mouse_position_begin, scene_controller->mouse_position_end);

		double mouse_move_speed = platform_event_is_keyboard_key_held(KEYBOARD_KEY_LEFT_SHIFT)
			? scene_controller->mouse_move_speed_fast
			: scene_controller->mouse_move_speed_normal;

		vector3_t mouse_delta_scaled = math_vector3_mul_scalar(scene_controller->mouse_position_delta, mouse_move_speed);
		vector3_t change_in_y = math_vector3_mul(g_world_up, mouse_delta_scaled);
		vector3_t change_in_x = math_vector3_mul(transform->local_right, mouse_delta_scaled);
		vector3_t mouse_offset = math_vector3_zero();

		mouse_offset = math_vector3_add(mouse_offset, change_in_x);
		mouse_offset = math_vector3_add(mouse_offset, change_in_y);

		vector3_t rotated_mouse_offset = math_vector3_rotate(mouse_offset, transform->local_rotation);
		vector3_t rotated_mouse_offset_scaled = math_vector3_mul_scalar(rotated_mouse_offset, g_platform_window_delta_time);

		transform_set_relative_position(transform, rotated_mouse_offset_scaled);
	}
	else if (platform_event_is_mouse_key_held(MOUSE_KEY_RIGHT))
	{
		double mouse_x = (double)g_platform_window_mouse_position_x;
		double mouse_y = (double)g_platform_window_mouse_position_y;

		scene_controller->mouse_position_end = math_vector3_xyz(mouse_x, mouse_y, 0.0);
		scene_controller->mouse_position_delta = math_vector3_sub(scene_controller->mouse_position_begin, scene_controller->mouse_position_end);

		double mouse_rotation_speed = platform_event_is_keyboard_key_held(KEYBOARD_KEY_LEFT_SHIFT)
			? scene_controller->mouse_rotation_speed_fast
			: scene_controller->mouse_rotation_speed_normal;

		vector3_t local_euler_angles = transform_get_local_euler_angles(transform);

		//local_euler_angles.x -= scene_controller->mouse_position_delta.y * mouse_rotation_speed * platform_window_delta_time();
		//local_euler_angles.y += scene_controller->mouse_position_delta.x * mouse_rotation_speed * platform_window_delta_time();

		//local_euler_angles.x = math_clamp(local_euler_angles.x, -90.0, 90.0);

		local_euler_angles.y += 0.01; // TODO

		transform_set_euler_angles(transform, local_euler_angles);
	}

	//mMousePositionStart -= mMousePositionDelta * mRotationDrag * Window::GetDeltaTime(); // TODO

	TRACY_ZONE_END
}
