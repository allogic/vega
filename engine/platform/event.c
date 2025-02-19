#include <vega/engine/platform/event.h>

#ifndef TRACY_ZONE_BEGIN
	#define TRACY_ZONE_BEGIN TracyCZoneC(ctx, TRACY_COLOR_GREEN, 1U);
#endif // TRACY_ZONE_BEGIN

#ifndef TRACY_ZONE_END
	#define TRACY_ZONE_END TracyCZoneEnd(ctx);
#endif // TRACY_ZONE_END

key_state_t g_platform_event_keyboard_key_states[0xFF] = { 0 };
key_state_t g_platform_event_mouse_key_states[0x3] = { 0 };

uint8_t platform_event_is_keyboard_key_pressed(keyboard_key_t key)
{
	TRACY_ZONE_BEGIN

	uint8_t pressed = g_platform_event_keyboard_key_states[key] == KEY_STATE_PRESSED;

	TRACY_ZONE_END

	return pressed;
}
uint8_t platform_event_is_keyboard_key_held(keyboard_key_t key)
{
	TRACY_ZONE_BEGIN

	uint8_t held = (g_platform_event_keyboard_key_states[key] == KEY_STATE_DOWN) || (g_platform_event_keyboard_key_states[key] == KEY_STATE_PRESSED);

	TRACY_ZONE_END

	return held;
}
uint8_t platform_event_is_keyboard_key_released(keyboard_key_t key)
{
	TRACY_ZONE_BEGIN

	uint8_t released = g_platform_event_keyboard_key_states[key] == KEY_STATE_RELEASED;

	TRACY_ZONE_END

	return released;
}
uint8_t platform_event_is_mouse_key_pressed(mouse_key_t key)
{
	TRACY_ZONE_BEGIN

	uint8_t pressed = g_platform_event_mouse_key_states[key] == KEY_STATE_PRESSED;

	TRACY_ZONE_END

	return pressed;
}
uint8_t platform_event_is_mouse_key_held(mouse_key_t key)
{
	TRACY_ZONE_BEGIN

	uint8_t held = (g_platform_event_mouse_key_states[key] == KEY_STATE_DOWN) || (g_platform_event_mouse_key_states[key] == KEY_STATE_PRESSED);

	TRACY_ZONE_END

	return held;
}
uint8_t platform_event_is_mouse_key_released(mouse_key_t key)
{
	TRACY_ZONE_BEGIN

	uint8_t released = g_platform_event_mouse_key_states[key] == KEY_STATE_RELEASED;

	TRACY_ZONE_END

	return released;
}
