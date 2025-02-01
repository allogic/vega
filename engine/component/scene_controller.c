#include <vega/engine/component/scene_controller.h>

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

	double move_speed = platform_window_is_keyboard_key_held(KEYBOARD_KEY_LEFT_SHIFT)
		? scene_controller->keyboard_move_speed_fast
		: scene_controller->keyboard_move_speed_normal;

	if (platform_window_is_keyboard_key_held(KEYBOARD_KEY_D))
	{
		vector3_t right_direction = math_vector3_mul_scalar(transform->local_right, move_speed);

		scene_controller->velocity = math_vector3_add(scene_controller->velocity, right_direction);

		transform_set_relative_position(transform, right_direction);
	}

	if (platform_window_is_keyboard_key_held(KEYBOARD_KEY_A))
	{
		vector3_t left_direction = math_vector3_mul_scalar(transform->local_left, move_speed);

		scene_controller->velocity = math_vector3_add(scene_controller->velocity, left_direction);

		transform_set_relative_position(transform, left_direction);
	}
	if (platform_window_is_keyboard_key_held(KEYBOARD_KEY_E))
	{
		vector3_t up_direction = math_vector3_mul_scalar(g_world_up, move_speed);

		scene_controller->velocity = math_vector3_add(scene_controller->velocity, up_direction);

		transform_set_relative_position(transform, up_direction);
	}
	if (platform_window_is_keyboard_key_held(KEYBOARD_KEY_Q))
	{
		vector3_t down_direction = math_vector3_mul_scalar(g_world_down, move_speed);

		scene_controller->velocity = math_vector3_add(scene_controller->velocity, down_direction);

		transform_set_relative_position(transform, down_direction);
	}
	if (platform_window_is_keyboard_key_held(KEYBOARD_KEY_W))
	{
		vector3_t front_direction = math_vector3_mul_scalar(transform->local_front, move_speed);

		scene_controller->velocity = math_vector3_add(scene_controller->velocity, front_direction);

		transform_set_relative_position(transform, front_direction);
	}
	if (platform_window_is_keyboard_key_held(KEYBOARD_KEY_S))
	{
		vector3_t back_direction = math_vector3_mul_scalar(transform->local_back, move_speed);

		scene_controller->velocity = math_vector3_add(scene_controller->velocity, back_direction);

		transform_set_relative_position(transform, back_direction);
	}

	/*
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

	/* TODO
	if (Window::RightMouseDown())
	{
		mMousePositionStart = Window::GetMousePosition();
	}

	if (Window::RightMouseHeld() && Window::LeftMouseHeld())
	{
		mMousePositionDelta = mMousePositionStart - Window::GetMousePosition();

		R32V3 positionOffset = {};

		R32 speed = Window::KeyHeld(KeyCode::LeftShift) ? mMouseMoveSpeedFast : mMouseMoveSpeedNormal;

		positionOffset += WorldUp * mMousePositionDelta.y * speed;
		positionOffset += transform->LocalRight * mMousePositionDelta.x * speed;

		transform->LocalPosition += transform->LocalRotation * positionOffset * Window::GetDeltaTime();
	}
	else if (Window::RightMouseHeld())
	{
		mMousePositionDelta = mMousePositionStart - Window::GetMousePosition();

		R32V3 eulerAngles = transform->LocalEulerAngles;

		R32 speed = Window::KeyHeld(KeyCode::LeftShift) ? mMouseRotationSpeedFast : mMouseRotationSpeedNormal;

		eulerAngles.x -= mMousePositionDelta.y * speed * Window::GetDeltaTime();
		eulerAngles.y += mMousePositionDelta.x * speed * Window::GetDeltaTime();

		if (eulerAngles.x < -89.9F) eulerAngles.x = -89.9F;
		if (eulerAngles.x > 89.9F) eulerAngles.x = 89.9F;

		transform->LocalEulerAngles = eulerAngles;
	}

	mMousePositionStart -= mMousePositionDelta * mRotationDrag * Window::GetDeltaTime();
	*/

	TRACY_ZONE_END
}
