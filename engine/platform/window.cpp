#define WIN32_LEAN_AND_MEAN
#define NO_GUID_DEFS
#include <windows.h>

#include <vega/engine/platform/event.h>
#include <vega/engine/platform/window.h>

#include <vega/engine/vulkan/command_pool.h>
#include <vega/engine/vulkan/device.h>
#include <vega/engine/vulkan/instance.h>
#include <vega/engine/vulkan/surface.h>
#include <vega/engine/vulkan/swap_chain.h>
#include <vega/engine/vulkan/renderer.h>

#ifndef TRACY_ZONE_BEGIN
	#define TRACY_ZONE_BEGIN TracyCZoneC(ctx, TRACY_COLOR_GREEN, 1U);
#endif // TRACY_ZONE_BEGIN

#ifndef TRACY_ZONE_END
	#define TRACY_ZONE_END TracyCZoneEnd(ctx);
#endif // TRACY_ZONE_END

extern LRESULT ImGui_ImplWin32_WndProcHandler(HWND window, UINT message, WPARAM w_param, LPARAM l_param);

static LRESULT platform_window_message_proc(HWND window, UINT message, WPARAM w_param, LPARAM l_param);

void* g_platform_window_module_handle = 0;
void* g_platform_window_handle = 0;

double g_platform_window_time = 0.0;
double g_platform_window_delta_time = 0.0;

int32_t g_platform_window_mouse_position_x = 0;
int32_t g_platform_window_mouse_position_y = 0;

int32_t g_platform_window_mouse_wheel_delta = 0;

uint8_t g_platform_window_should_close = 0;

static timer_t s_platform_window_timer = {};

static LPCSTR s_platform_window_class = "VegaClass";

static MSG s_platform_window_message = {};

static uint8_t s_platform_window_allocated = 0;

void platform_window_alloc(char const* title, uint32_t width, uint32_t height)
{
	TRACY_ZONE_BEGIN

	g_platform_window_module_handle = GetModuleHandle(0);

	WNDCLASSEX window_class_ex = {};
	window_class_ex.cbSize = sizeof(WNDCLASSEX);
	window_class_ex.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
	window_class_ex.lpfnWndProc = platform_window_message_proc;
	window_class_ex.cbClsExtra = 0;
	window_class_ex.cbWndExtra = 0;
	window_class_ex.hInstance = (HINSTANCE)g_platform_window_module_handle;
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

	g_platform_window_handle = CreateWindowExA(0, s_platform_window_class, title, WS_OVERLAPPEDWINDOW | WS_CLIPSIBLINGS | WS_CLIPCHILDREN, position_x, position_y, width, height, 0, 0, (HINSTANCE)g_platform_window_module_handle, 0);

	ShowWindow((HWND)g_platform_window_handle, SW_SHOW);
	UpdateWindow((HWND)g_platform_window_handle);

	s_platform_window_timer = timer_init();

	TRACY_ZONE_END
}
void platform_window_begin_frame(void)
{
	TRACY_ZONE_BEGIN

	timer_begin(&s_platform_window_timer);

	TRACY_ZONE_END
}
void platform_window_poll_events(void)
{
	TRACY_ZONE_BEGIN

	g_platform_window_mouse_wheel_delta = 0;

	platform_event_update_key_states();

	while (PeekMessageA(&s_platform_window_message, 0, 0, 0, PM_REMOVE))
	{
		TranslateMessage(&s_platform_window_message);
		DispatchMessageA(&s_platform_window_message);
	}

	TRACY_ZONE_END
}
void platform_window_end_frame(void)
{
	TRACY_ZONE_BEGIN

	timer_end(&s_platform_window_timer);

	g_platform_window_delta_time = timer_ms(&s_platform_window_timer);
	g_platform_window_time += g_platform_window_delta_time;

	TRACY_ZONE_END
}
void platform_window_free(void)
{
	TRACY_ZONE_BEGIN

	if (s_platform_window_allocated)
	{
		s_platform_window_allocated = 0;

		vulkan_renderer_free();
		vulkan_swap_chain_free();
		vulkan_command_pool_free();
		vulkan_device_free();
		vulkan_surface_free();
		vulkan_instance_free();
	}

	DestroyWindow((HWND)g_platform_window_handle);

	UnregisterClassA(s_platform_window_class, (HINSTANCE)g_platform_window_module_handle);

	TRACY_ZONE_END
}
void platform_window_resize(uint32_t width, uint32_t height)
{
	TRACY_ZONE_BEGIN

	if (s_platform_window_allocated)
	{
		vulkan_renderer_resize_before();
		vulkan_swap_chain_resize_before();

		vulkan_surface_resize();
		vulkan_renderer_resize();

		vulkan_swap_chain_resize_after();
		vulkan_renderer_resize_after();
	}
	else
	{
		s_platform_window_allocated = 1;

		vulkan_instance_alloc();
		vulkan_surface_alloc();

		vulkan_device_find_physical();
		vulkan_device_find_physical_queue_families();
		vulkan_device_check_physical_extensions();
		vulkan_device_alloc();

		vulkan_surface_check_capabilities();
		vulkan_surface_resize();

		vulkan_command_pool_alloc();
		vulkan_swap_chain_alloc();
		vulkan_renderer_alloc();
	}

	TRACY_ZONE_END
}
static LRESULT platform_window_message_proc(HWND window, UINT message, WPARAM w_param, LPARAM l_param)
{
	if (ImGui_ImplWin32_WndProcHandler(window, message, w_param, l_param))
	{
		return 1;
	}

	switch (message)
	{
		case WM_CLOSE:
		{
			g_platform_window_should_close = 1;

			break;
		}
		case WM_WINDOWPOSCHANGED:
		{
			RECT client_rect = {};
			GetClientRect(window, &client_rect);

			INT width = client_rect.right - client_rect.left;
			INT height = client_rect.bottom - client_rect.top;

			if ((width > 0) && (height > 0) && (g_vulkan_surface_width != width) && (g_vulkan_surface_height != height))
			{
				platform_window_resize(width, height);
			}

			break;
		}
		case WM_KEYDOWN:
		case WM_SYSKEYDOWN:
		{
			UINT scan_code = MapVirtualKeyA((UINT)w_param, MAPVK_VK_TO_VSC);
			UINT virtual_key = MapVirtualKeyExA(scan_code, MAPVK_VSC_TO_VK_EX, GetKeyboardLayout(0));

			switch (virtual_key)
			{
				case KEYBOARD_KEY_LEFT_SHIFT: g_platform_event_keyboard_key_states[KEYBOARD_KEY_LEFT_SHIFT] = ((g_platform_event_keyboard_key_states[KEYBOARD_KEY_LEFT_SHIFT] == KEY_STATE_UP) || (g_platform_event_keyboard_key_states[KEYBOARD_KEY_LEFT_SHIFT] == KEY_STATE_RELEASED)) ? KEY_STATE_PRESSED : KEY_STATE_DOWN; break;
				case KEYBOARD_KEY_RIGHT_SHIFT: g_platform_event_keyboard_key_states[KEYBOARD_KEY_RIGHT_SHIFT] = ((g_platform_event_keyboard_key_states[KEYBOARD_KEY_RIGHT_SHIFT] == KEY_STATE_UP) || (g_platform_event_keyboard_key_states[KEYBOARD_KEY_RIGHT_SHIFT] == KEY_STATE_RELEASED)) ? KEY_STATE_PRESSED : KEY_STATE_DOWN; break;
				case KEYBOARD_KEY_LEFT_CONTROL: g_platform_event_keyboard_key_states[KEYBOARD_KEY_LEFT_CONTROL] = ((g_platform_event_keyboard_key_states[KEYBOARD_KEY_LEFT_CONTROL] == KEY_STATE_UP) || (g_platform_event_keyboard_key_states[KEYBOARD_KEY_LEFT_CONTROL] == KEY_STATE_RELEASED)) ? KEY_STATE_PRESSED : KEY_STATE_DOWN; break;
				case KEYBOARD_KEY_RIGHT_CONTROL: g_platform_event_keyboard_key_states[KEYBOARD_KEY_RIGHT_CONTROL] = ((g_platform_event_keyboard_key_states[KEYBOARD_KEY_RIGHT_CONTROL] == KEY_STATE_UP) || (g_platform_event_keyboard_key_states[KEYBOARD_KEY_RIGHT_CONTROL] == KEY_STATE_RELEASED)) ? KEY_STATE_PRESSED : KEY_STATE_DOWN; break;
				case KEYBOARD_KEY_LEFT_MENU: g_platform_event_keyboard_key_states[KEYBOARD_KEY_LEFT_MENU] = ((g_platform_event_keyboard_key_states[KEYBOARD_KEY_LEFT_MENU] == KEY_STATE_UP) || (g_platform_event_keyboard_key_states[KEYBOARD_KEY_LEFT_MENU] == KEY_STATE_RELEASED)) ? KEY_STATE_PRESSED : KEY_STATE_DOWN; break;
				case KEYBOARD_KEY_RIGHT_MENU: g_platform_event_keyboard_key_states[KEYBOARD_KEY_RIGHT_MENU] = ((g_platform_event_keyboard_key_states[KEYBOARD_KEY_RIGHT_MENU] == KEY_STATE_UP) || (g_platform_event_keyboard_key_states[KEYBOARD_KEY_RIGHT_MENU] == KEY_STATE_RELEASED)) ? KEY_STATE_PRESSED : KEY_STATE_DOWN; break;
				default: g_platform_event_keyboard_key_states[virtual_key] = ((g_platform_event_keyboard_key_states[virtual_key] == KEY_STATE_UP) || (g_platform_event_keyboard_key_states[virtual_key] == KEY_STATE_RELEASED)) ? KEY_STATE_PRESSED : KEY_STATE_DOWN; break;
			}

			break;
		}
		case WM_KEYUP:
		case WM_SYSKEYUP:
		{
			UINT scan_code = MapVirtualKeyA((UINT)w_param, MAPVK_VK_TO_VSC);
			UINT virtual_key = MapVirtualKeyExA(scan_code, MAPVK_VSC_TO_VK_EX, GetKeyboardLayout(0));

			switch (virtual_key)
			{
				case KEYBOARD_KEY_LEFT_SHIFT: g_platform_event_keyboard_key_states[KEYBOARD_KEY_LEFT_SHIFT] = ((g_platform_event_keyboard_key_states[KEYBOARD_KEY_LEFT_SHIFT] == KEY_STATE_DOWN) || (g_platform_event_keyboard_key_states[KEYBOARD_KEY_LEFT_SHIFT] == KEY_STATE_PRESSED)) ? KEY_STATE_RELEASED : KEY_STATE_UP; break;
				case KEYBOARD_KEY_RIGHT_SHIFT: g_platform_event_keyboard_key_states[KEYBOARD_KEY_RIGHT_SHIFT] = ((g_platform_event_keyboard_key_states[KEYBOARD_KEY_RIGHT_SHIFT] == KEY_STATE_DOWN) || (g_platform_event_keyboard_key_states[KEYBOARD_KEY_RIGHT_SHIFT] == KEY_STATE_PRESSED)) ? KEY_STATE_RELEASED : KEY_STATE_UP; break;
				case KEYBOARD_KEY_LEFT_CONTROL: g_platform_event_keyboard_key_states[KEYBOARD_KEY_LEFT_CONTROL] = ((g_platform_event_keyboard_key_states[KEYBOARD_KEY_LEFT_CONTROL] == KEY_STATE_DOWN) || (g_platform_event_keyboard_key_states[KEYBOARD_KEY_LEFT_CONTROL] == KEY_STATE_PRESSED)) ? KEY_STATE_RELEASED : KEY_STATE_UP; break;
				case KEYBOARD_KEY_RIGHT_CONTROL: g_platform_event_keyboard_key_states[KEYBOARD_KEY_RIGHT_CONTROL] = ((g_platform_event_keyboard_key_states[KEYBOARD_KEY_RIGHT_CONTROL] == KEY_STATE_DOWN) || (g_platform_event_keyboard_key_states[KEYBOARD_KEY_RIGHT_CONTROL] == KEY_STATE_PRESSED)) ? KEY_STATE_RELEASED : KEY_STATE_UP; break;
				case KEYBOARD_KEY_LEFT_MENU: g_platform_event_keyboard_key_states[KEYBOARD_KEY_LEFT_MENU] = ((g_platform_event_keyboard_key_states[KEYBOARD_KEY_LEFT_MENU] == KEY_STATE_DOWN) || (g_platform_event_keyboard_key_states[KEYBOARD_KEY_LEFT_MENU] == KEY_STATE_PRESSED)) ? KEY_STATE_RELEASED : KEY_STATE_UP; break;
				case KEYBOARD_KEY_RIGHT_MENU: g_platform_event_keyboard_key_states[KEYBOARD_KEY_RIGHT_MENU] = ((g_platform_event_keyboard_key_states[KEYBOARD_KEY_RIGHT_MENU] == KEY_STATE_DOWN) || (g_platform_event_keyboard_key_states[KEYBOARD_KEY_RIGHT_MENU] == KEY_STATE_PRESSED)) ? KEY_STATE_RELEASED : KEY_STATE_UP; break;
				default: g_platform_event_keyboard_key_states[virtual_key] = ((g_platform_event_keyboard_key_states[virtual_key] == KEY_STATE_DOWN) || (g_platform_event_keyboard_key_states[virtual_key] == KEY_STATE_PRESSED)) ? KEY_STATE_RELEASED : KEY_STATE_UP; break;
			}

			break;
		}
		case WM_LBUTTONDOWN:
		{
			g_platform_event_mouse_key_states[MOUSE_KEY_LEFT] = ((g_platform_event_mouse_key_states[MOUSE_KEY_LEFT] == KEY_STATE_UP) || (g_platform_event_mouse_key_states[MOUSE_KEY_LEFT] == KEY_STATE_RELEASED)) ? KEY_STATE_PRESSED : KEY_STATE_DOWN;

			break;
		}
		case WM_LBUTTONUP:
		{
			g_platform_event_mouse_key_states[MOUSE_KEY_LEFT] = ((g_platform_event_mouse_key_states[MOUSE_KEY_LEFT] == KEY_STATE_DOWN) || (g_platform_event_mouse_key_states[MOUSE_KEY_LEFT] == KEY_STATE_PRESSED)) ? KEY_STATE_RELEASED : KEY_STATE_UP;

			break;
		}
		case WM_MBUTTONDOWN:
		{
			g_platform_event_mouse_key_states[MOUSE_KEY_MIDDLE] = ((g_platform_event_mouse_key_states[MOUSE_KEY_MIDDLE] == KEY_STATE_UP) || (g_platform_event_mouse_key_states[MOUSE_KEY_MIDDLE] == KEY_STATE_RELEASED)) ? KEY_STATE_PRESSED : KEY_STATE_DOWN;

			break;
		}
		case WM_MBUTTONUP:
		{
			g_platform_event_mouse_key_states[MOUSE_KEY_MIDDLE] = ((g_platform_event_mouse_key_states[MOUSE_KEY_MIDDLE] == KEY_STATE_DOWN) || (g_platform_event_mouse_key_states[MOUSE_KEY_MIDDLE] == KEY_STATE_PRESSED)) ? KEY_STATE_RELEASED : KEY_STATE_UP;

			break;
		}
		case WM_RBUTTONDOWN:
		{
			g_platform_event_mouse_key_states[MOUSE_KEY_RIGHT] = ((g_platform_event_mouse_key_states[MOUSE_KEY_RIGHT] == KEY_STATE_UP) || (g_platform_event_mouse_key_states[MOUSE_KEY_RIGHT] == KEY_STATE_RELEASED)) ? KEY_STATE_PRESSED : KEY_STATE_DOWN;

			break;
		}
		case WM_RBUTTONUP:
		{
			g_platform_event_mouse_key_states[MOUSE_KEY_RIGHT] = ((g_platform_event_mouse_key_states[MOUSE_KEY_RIGHT] == KEY_STATE_DOWN) || (g_platform_event_mouse_key_states[MOUSE_KEY_RIGHT] == KEY_STATE_PRESSED)) ? KEY_STATE_RELEASED : KEY_STATE_UP;

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
			INT mouse_x = LOWORD(l_param);
			INT mouse_y = HIWORD(l_param);

			g_platform_window_mouse_position_x = mouse_x;
			g_platform_window_mouse_position_y = mouse_y;

			break;
		}
		case WM_MOUSEWHEEL:
		{
			g_platform_window_mouse_wheel_delta = GET_WHEEL_DELTA_WPARAM(w_param);

			break;
		}
		default:
		{
			return DefWindowProcA(window, message, w_param, l_param);
		}
	}

	return 1;
}
