#ifndef VEGA_ENGINE_PLATFORM_WINDOW_H
#define VEGA_ENGINE_PLATFORM_WINDOW_H

#include <stdint.h>

#include <vega/core/core.h>

VEGA_EXTERN_C_BEGIN

extern void* g_platform_window_module_handle;
extern void* g_platform_window_handle;

extern double g_platform_window_time;
extern double g_platform_window_delta_time;

extern int32_t g_platform_window_mouse_position_x;
extern int32_t g_platform_window_mouse_position_y;

extern int32_t g_platform_window_mouse_wheel_delta;

extern uint8_t g_platform_window_should_close;

///////////////////////////////////////////////////////////////
// Public API
///////////////////////////////////////////////////////////////

void platform_window_alloc(char const* title, uint32_t width, uint32_t height);
void platform_window_begin_frame(void);
void platform_window_poll_events(void);
void platform_window_end_frame(void);
void platform_window_free(void);

///////////////////////////////////////////////////////////////
// Internal API
///////////////////////////////////////////////////////////////

void platform_window_resize(uint32_t width, uint32_t height);

VEGA_EXTERN_C_END

#endif // VEGA_ENGINE_PLATFORM_WINDOW_H
