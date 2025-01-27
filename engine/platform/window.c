#include <windows.h>

#include <vega/engine/platform/window.h>

#ifndef TRACY_ZONE_BEGIN
	#define TRACY_ZONE_BEGIN TracyCZoneC(ctx, TRACY_COLOR_GREEN, 1U);
#endif // TRACY_ZONE_BEGIN

#ifndef TRACY_ZONE_END
	#define TRACY_ZONE_END TracyCZoneEnd(ctx);
#endif // TRACY_ZONE_END

typedef enum _keyboard_key_state
{
	KEYBOARD_KEY_STATE_UP,
	KEYBOARD_KEY_STATE_PRESSED,
	KEYBOARD_KEY_STATE_DOWN,
	KEYBOARD_KEY_STATE_RELEASED,
} keyboard_key_state;
typedef enum _mouse_key_state
{
	MOUSE_KEY_STATE_UP,
	MOUSE_KEY_STATE_PRESSED,
	MOUSE_KEY_STATE_DOWN,
	MOUSE_KEY_STATE_RELEASED,
} mouse_key_state;

static LRESULT platform_window_message_proc(HWND window, UINT message, WPARAM w_param, LPARAM l_param);

void* g_platform_module_handle = 0;
void* g_platform_window_handle = 0;

uint8_t g_platform_window_should_close = 0;

int32_t g_platform_window_width = 0;
int32_t g_platform_window_height = 0;

int32_t g_platform_mouse_position_x = 0;
int32_t g_platform_mouse_position_y = 0;

int32_t g_platform_mouse_wheel_delta = 0;

static LPCSTR s_platform_window_class = "VegaClass";

static MSG s_platform_message = { 0 };

static keyboard_key_state s_platform_keyboard_key_states[0xFF] = { 0 };
static mouse_key_state s_platform_mouse_key_states[0x3] = { 0 };

void platform_window_alloc(char const* title, uint32_t width, uint32_t height)
{
	TRACY_ZONE_BEGIN

	g_platform_module_handle = GetModuleHandle(0);

	g_platform_window_width = width;
	g_platform_window_height = height;

	WNDCLASSEX window_class_ex = { 0 };
	window_class_ex.cbSize = sizeof(WNDCLASSEX);
	window_class_ex.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
	window_class_ex.lpfnWndProc = platform_window_message_proc;
	window_class_ex.cbClsExtra = 0;
	window_class_ex.cbWndExtra = 0;
	window_class_ex.hInstance = g_platform_module_handle;
	window_class_ex.hIcon = LoadIconA(0, IDI_APPLICATION);
	window_class_ex.hCursor = LoadCursorA(0, IDC_ARROW);
	window_class_ex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	window_class_ex.lpszMenuName = 0;
	window_class_ex.lpszClassName = s_platform_window_class;
	window_class_ex.hIconSm = LoadIconA(0, IDI_APPLICATION);

	RegisterClassExA(&window_class_ex);

	INT screen_width = GetSystemMetrics(SM_CXSCREEN);
	INT screen_height = GetSystemMetrics(SM_CYSCREEN);
	INT position_x = (screen_width - width) / 2;
	INT position_y = (screen_height - height) / 2;

	g_platform_window_handle = CreateWindowExA(0, s_platform_window_class, title, WS_OVERLAPPEDWINDOW | WS_CLIPSIBLINGS | WS_CLIPCHILDREN, position_x, position_y, width, height, 0, 0, g_platform_module_handle, 0);

	ShowWindow(g_platform_window_handle, SW_SHOW);
	UpdateWindow(g_platform_window_handle);

	TRACY_ZONE_END
}
void platform_window_poll_events(void)
{
	TRACY_ZONE_BEGIN

	g_platform_mouse_wheel_delta = 0;

	for (UCHAR key_index = 0; key_index < 0xFF; key_index++)
	{
		if (s_platform_keyboard_key_states[key_index] == KEYBOARD_KEY_STATE_PRESSED)
		{
			s_platform_keyboard_key_states[key_index] = KEYBOARD_KEY_STATE_DOWN;
		}
		else if (s_platform_keyboard_key_states[key_index] == KEYBOARD_KEY_STATE_RELEASED)
		{
			s_platform_keyboard_key_states[key_index] = KEYBOARD_KEY_STATE_UP;
		}
	}

	for (UCHAR key_index = 0; key_index < 0x3; key_index++)
	{
		if (s_platform_mouse_key_states[key_index] == MOUSE_KEY_STATE_PRESSED)
		{
			s_platform_mouse_key_states[key_index] = MOUSE_KEY_STATE_DOWN;
		}
		else if (s_platform_mouse_key_states[key_index] == MOUSE_KEY_STATE_RELEASED)
		{
			s_platform_mouse_key_states[key_index] = MOUSE_KEY_STATE_UP;
		}
	}

	while (PeekMessageA(&s_platform_message, 0, 0, 0, PM_REMOVE))
	{
		TranslateMessage(&s_platform_message);
		DispatchMessageA(&s_platform_message);
	}

	TRACY_ZONE_END
}
uint8_t platform_window_is_keyboard_key_pressed(keyboard_key Key)
{
	TRACY_ZONE_BEGIN

	uint8_t pressed = s_platform_keyboard_key_states[Key] == KEYBOARD_KEY_STATE_PRESSED;

	TRACY_ZONE_END

	return pressed;
}
uint8_t platform_window_is_keyboard_key_held(keyboard_key Key)
{
	TRACY_ZONE_BEGIN

	uint8_t held = (s_platform_keyboard_key_states[Key] == KEYBOARD_KEY_STATE_DOWN) || (s_platform_keyboard_key_states[Key] == KEYBOARD_KEY_STATE_PRESSED);

	TRACY_ZONE_END

	return held;
}
uint8_t platform_window_is_keyboard_key_released(keyboard_key Key)
{
	TRACY_ZONE_BEGIN

	uint8_t released = s_platform_keyboard_key_states[Key] == KEYBOARD_KEY_STATE_RELEASED;

	TRACY_ZONE_END

	return released;
}
uint8_t platform_window_is_mouse_key_pressed(mouse_key Key)
{
	TRACY_ZONE_BEGIN

	uint8_t pressed = s_platform_mouse_key_states[Key] == MOUSE_KEY_STATE_PRESSED;

	TRACY_ZONE_END

	return pressed;
}
uint8_t platform_window_is_mouse_key_held(mouse_key Key)
{
	TRACY_ZONE_BEGIN

	uint8_t held = (s_platform_mouse_key_states[Key] == MOUSE_KEY_STATE_DOWN) || (s_platform_mouse_key_states[Key] == MOUSE_KEY_STATE_PRESSED);

	TRACY_ZONE_END

	return held;
}
uint8_t platform_window_is_mouse_key_released(mouse_key Key)
{
	TRACY_ZONE_BEGIN

	uint8_t released = s_platform_mouse_key_states[Key] == MOUSE_KEY_STATE_RELEASED;

	TRACY_ZONE_END

	return released;
}
void platform_window_free(void)
{
	TRACY_ZONE_BEGIN

	DestroyWindow(g_platform_window_handle);

	UnregisterClassA(s_platform_window_class, g_platform_module_handle);

	TRACY_ZONE_END
}
static LRESULT platform_window_message_proc(HWND window, UINT message, WPARAM w_param, LPARAM l_param)
{
	switch (message)
	{
		case WM_CLOSE:
		{
			g_platform_window_should_close = 1;

			break;
		}
		case WM_WINDOWPOSCHANGED:
		{
			RECT client_rect = { 0 };
			GetClientRect(window, &client_rect);

			INT width = client_rect.right - client_rect.left;
			INT height = client_rect.bottom - client_rect.top;

			if ((width > 0) && (height > 0))
			{
				g_platform_window_width = width;
				g_platform_window_height = height;
			}

			break;
		}
		case WM_KEYDOWN:
		case WM_SYSKEYDOWN:
		{
			s_platform_keyboard_key_states[w_param] = ((s_platform_keyboard_key_states[w_param] == KEYBOARD_KEY_STATE_UP) || (s_platform_keyboard_key_states[w_param] == KEYBOARD_KEY_STATE_RELEASED)) ? KEYBOARD_KEY_STATE_PRESSED : KEYBOARD_KEY_STATE_DOWN;

			break;
		}
		case WM_KEYUP:
		case WM_SYSKEYUP:
		{
			s_platform_keyboard_key_states[w_param] = ((s_platform_keyboard_key_states[w_param] == KEYBOARD_KEY_STATE_DOWN) || (s_platform_keyboard_key_states[w_param] == KEYBOARD_KEY_STATE_PRESSED)) ? KEYBOARD_KEY_STATE_RELEASED : KEYBOARD_KEY_STATE_UP;

			break;
		}
		case WM_LBUTTONDOWN:
		{
			s_platform_mouse_key_states[MOUSE_KEY_LEFT] = ((s_platform_mouse_key_states[MOUSE_KEY_LEFT] == MOUSE_KEY_STATE_UP) || (s_platform_mouse_key_states[MOUSE_KEY_LEFT] == MOUSE_KEY_STATE_RELEASED)) ? MOUSE_KEY_STATE_PRESSED : MOUSE_KEY_STATE_DOWN;

			break;
		}
		case WM_LBUTTONUP:
		{
			s_platform_mouse_key_states[MOUSE_KEY_LEFT] = ((s_platform_mouse_key_states[MOUSE_KEY_LEFT] == MOUSE_KEY_STATE_DOWN) || (s_platform_mouse_key_states[MOUSE_KEY_LEFT] == MOUSE_KEY_STATE_PRESSED)) ? MOUSE_KEY_STATE_RELEASED : MOUSE_KEY_STATE_UP;

			break;
		}
		case WM_MBUTTONDOWN:
		{
			s_platform_mouse_key_states[MOUSE_KEY_MIDDLE] = ((s_platform_mouse_key_states[MOUSE_KEY_MIDDLE] == MOUSE_KEY_STATE_UP) || (s_platform_mouse_key_states[MOUSE_KEY_MIDDLE] == MOUSE_KEY_STATE_RELEASED)) ? MOUSE_KEY_STATE_PRESSED : MOUSE_KEY_STATE_DOWN;

			break;
		}
		case WM_MBUTTONUP:
		{
			s_platform_mouse_key_states[MOUSE_KEY_MIDDLE] = ((s_platform_mouse_key_states[MOUSE_KEY_MIDDLE] == MOUSE_KEY_STATE_DOWN) || (s_platform_mouse_key_states[MOUSE_KEY_MIDDLE] == MOUSE_KEY_STATE_PRESSED)) ? MOUSE_KEY_STATE_RELEASED : MOUSE_KEY_STATE_UP;

			break;
		}
		case WM_RBUTTONDOWN:
		{
			s_platform_mouse_key_states[MOUSE_KEY_RIGHT] = ((s_platform_mouse_key_states[MOUSE_KEY_RIGHT] == MOUSE_KEY_STATE_UP) || (s_platform_mouse_key_states[MOUSE_KEY_RIGHT] == MOUSE_KEY_STATE_RELEASED)) ? MOUSE_KEY_STATE_PRESSED : MOUSE_KEY_STATE_DOWN;

			break;
		}
		case WM_RBUTTONUP:
		{
			s_platform_mouse_key_states[MOUSE_KEY_RIGHT] = ((s_platform_mouse_key_states[MOUSE_KEY_RIGHT] == MOUSE_KEY_STATE_DOWN) || (s_platform_mouse_key_states[MOUSE_KEY_RIGHT] == MOUSE_KEY_STATE_PRESSED)) ? MOUSE_KEY_STATE_RELEASED : MOUSE_KEY_STATE_UP;

			break;
		}
		case WM_LBUTTONDBLCLK:
		{
			break;
		}
		case WM_MBUTTONDBLCLK:
		{
			break;
		}
		case WM_RBUTTONDBLCLK:
		{
			break;
		}
		case WM_MOUSEMOVE:
		{
			INT position_x = LOWORD(l_param);
			INT position_y = HIWORD(l_param);

			g_platform_mouse_position_x = position_x;
			g_platform_mouse_position_y = position_y;

			break;
		}
		case WM_MOUSEWHEEL:
		{
			g_platform_mouse_wheel_delta = GET_WHEEL_DELTA_WPARAM(w_param);

			break;
		}
		default:
		{
			return DefWindowProcA(window, message, w_param, l_param);
		}
	}

	return 0;
}
