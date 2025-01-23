#ifndef VEGA_ENGINE_PLATFORM_WINDOW_H
#define VEGA_ENGINE_PLATFORM_WINDOW_H

#include <stdint.h>

#include <vega/core/core.h>

typedef enum _keyboard_key
{
	KEYBOARD_KEY_BACK = 0x08,
	KEYBOARD_KEY_TAB = 0x09,
	KEYBOARD_KEY_RETURN = 0x0D,
	KEYBOARD_KEY_SHIFT = 0x10,
	KEYBOARD_KEY_CONTROL = 0x11,
	KEYBOARD_KEY_MENU = 0x12,
	KEYBOARD_KEY_PAUSE = 0x13,
	KEYBOARD_KEY_CAPITAL = 0x14,
	KEYBOARD_KEY_ESCAPE = 0x1B,
	KEYBOARD_KEY_SPACE = 0x20,
	KEYBOARD_KEY_PRIOR = 0x21,
	KEYBOARD_KEY_NEXT = 0x22,
	KEYBOARD_KEY_END = 0x23,
	KEYBOARD_KEY_HOME = 0x24,
	KEYBOARD_KEY_LEFT = 0x25,
	KEYBOARD_KEY_UP = 0x26,
	KEYBOARD_KEY_RIGHT = 0x27,
	KEYBOARD_KEY_DOWN = 0x28,
	KEYBOARD_KEY_SELECT = 0x29,
	KEYBOARD_KEY_PRINT = 0x2A,
	KEYBOARD_KEY_EXE = 0x2B,
	KEYBOARD_KEY_SNAPSHOT = 0x2C,
	KEYBOARD_KEY_INSERT = 0x2D,
	KEYBOARD_KEY_DELETE = 0x2E,
	KEYBOARD_KEY_HELP = 0x2F,
	KEYBOARD_KEY_0 = '0',
	KEYBOARD_KEY_1 = '1',
	KEYBOARD_KEY_2 = '2',
	KEYBOARD_KEY_3 = '3',
	KEYBOARD_KEY_4 = '4',
	KEYBOARD_KEY_5 = '5',
	KEYBOARD_KEY_6 = '6',
	KEYBOARD_KEY_7 = '7',
	KEYBOARD_KEY_8 = '8',
	KEYBOARD_KEY_9 = '9',
	KEYBOARD_KEY_A = 'A',
	KEYBOARD_KEY_B = 'B',
	KEYBOARD_KEY_C = 'C',
	KEYBOARD_KEY_D = 'D',
	KEYBOARD_KEY_E = 'E',
	KEYBOARD_KEY_F = 'F',
	KEYBOARD_KEY_G = 'G',
	KEYBOARD_KEY_H = 'H',
	KEYBOARD_KEY_I = 'I',
	KEYBOARD_KEY_J = 'J',
	KEYBOARD_KEY_K = 'K',
	KEYBOARD_KEY_L = 'L',
	KEYBOARD_KEY_M = 'M',
	KEYBOARD_KEY_N = 'N',
	KEYBOARD_KEY_O = 'O',
	KEYBOARD_KEY_P = 'P',
	KEYBOARD_KEY_Q = 'Q',
	KEYBOARD_KEY_R = 'R',
	KEYBOARD_KEY_S = 'S',
	KEYBOARD_KEY_T = 'T',
	KEYBOARD_KEY_U = 'U',
	KEYBOARD_KEY_V = 'V',
	KEYBOARD_KEY_W = 'W',
	KEYBOARD_KEY_X = 'X',
	KEYBOARD_KEY_Y = 'Y',
	KEYBOARD_KEY_Z = 'Z',
	KEYBOARD_KEY_NUMPAD0 = 0x60,
	KEYBOARD_KEY_NUMPAD1 = 0x61,
	KEYBOARD_KEY_NUMPAD2 = 0x62,
	KEYBOARD_KEY_NUMPAD3 = 0x63,
	KEYBOARD_KEY_NUMPAD4 = 0x64,
	KEYBOARD_KEY_NUMPAD5 = 0x65,
	KEYBOARD_KEY_NUMPAD6 = 0x66,
	KEYBOARD_KEY_NUMPAD7 = 0x67,
	KEYBOARD_KEY_NUMPAD8 = 0x68,
	KEYBOARD_KEY_NUMPAD9 = 0x69,
	KEYBOARD_KEY_MULTIPLY = 0x6A,
	KEYBOARD_KEY_ADD = 0x6B,
	KEYBOARD_KEY_SEPARATOR = 0x6C,
	KEYBOARD_KEY_SUBTRACT = 0x6D,
	KEYBOARD_KEY_DECIMAL = 0x6E,
	KEYBOARD_KEY_DIVIDE = 0x6F,
	KEYBOARD_KEY_F1 = 0x70,
	KEYBOARD_KEY_F2 = 0x71,
	KEYBOARD_KEY_F3 = 0x72,
	KEYBOARD_KEY_F4 = 0x73,
	KEYBOARD_KEY_F5 = 0x74,
	KEYBOARD_KEY_F6 = 0x75,
	KEYBOARD_KEY_F7 = 0x76,
	KEYBOARD_KEY_F8 = 0x77,
	KEYBOARD_KEY_F9 = 0x78,
	KEYBOARD_KEY_F10 = 0x79,
	KEYBOARD_KEY_F11 = 0x7A,
	KEYBOARD_KEY_F12 = 0x7B,
	KEYBOARD_KEY_F13 = 0x7C,
	KEYBOARD_KEY_F14 = 0x7D,
	KEYBOARD_KEY_F15 = 0x7E,
	KEYBOARD_KEY_F16 = 0x7F,
	KEYBOARD_KEY_F17 = 0x80,
	KEYBOARD_KEY_F18 = 0x81,
	KEYBOARD_KEY_F19 = 0x82,
	KEYBOARD_KEY_F20 = 0x83,
	KEYBOARD_KEY_F21 = 0x84,
	KEYBOARD_KEY_F22 = 0x85,
	KEYBOARD_KEY_F23 = 0x86,
	KEYBOARD_KEY_F24 = 0x87,
	KEYBOARD_KEY_LEFT_SHIFT = 0xA0,
	KEYBOARD_KEY_RIGHT_SHIFT = 0xA1,
	KEYBOARD_KEY_LEFT_CONTROL = 0xA2,
	KEYBOARD_KEY_RIGHT_CONTROL = 0xA3,
	KEYBOARD_KEY_LEFT_MENU = 0xA4,
	KEYBOARD_KEY_RIGHT_MENU = 0xA5,
} keyboard_key;
typedef enum _mouse_key
{
	MOUSE_KEY_LEFT,
	MOUSE_KEY_MIDDLE,
	MOUSE_KEY_RIGHT,
} mouse_key;

extern void* g_module_handle;
extern void* g_window_handle;

extern uint8_t g_window_should_close;

extern int32_t g_window_width;
extern int32_t g_window_height;

extern int32_t g_mouse_position_x;
extern int32_t g_mouse_position_y;

extern int32_t g_mouse_wheel_delta;

///////////////////////////////////////////////////////////////
// Public API
///////////////////////////////////////////////////////////////

void platform_window_alloc(char const* title, uint32_t width, uint32_t height);
void platform_window_poll_events(void);
uint8_t platform_window_is_keyboard_key_pressed(keyboard_key Key);
uint8_t platform_window_is_keyboard_key_held(keyboard_key Key);
uint8_t platform_window_is_keyboard_key_released(keyboard_key Key);
uint8_t platform_window_is_mouse_key_pressed(mouse_key Key);
uint8_t platform_window_is_mouse_key_held(mouse_key Key);
uint8_t platform_window_is_mouse_key_released(mouse_key Key);
void platform_window_free(void);

#endif // VEGA_ENGINE_PLATFORM_WINDOW_H
