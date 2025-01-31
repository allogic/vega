#include <stdint.h>

#define WIN32_LEAN_AND_MEAN
#define NO_GUID_DEFS
#include <windows.h>

#include <vega/core/timer.h>

#include <vega/core/debug/tracy.h>

#ifndef TRACY_ZONE_BEGIN
	#define TRACY_ZONE_BEGIN TracyCZoneC(ctx, TRACY_COLOR_RED, 1U);
#endif // TRACY_ZONE_BEGIN

#ifndef TRACY_ZONE_END
	#define TRACY_ZONE_END TracyCZoneEnd(ctx);
#endif // TRACY_ZONE_END

static LARGE_INTEGER s_begin = { 0 };
static LARGE_INTEGER s_end = { 0 };

timer_t timer_init(void)
{
	TRACY_ZONE_BEGIN

	timer_t timer;
	memset(&timer, 0, sizeof(timer_t));

	LARGE_INTEGER freq;
	QueryPerformanceFrequency(&freq);

	timer.freq = (double)freq.QuadPart;
	timer.begin = 0.0;
	timer.ns = 0.0;

	TRACY_ZONE_END

	return timer;
}
void timer_begin(timer_t* timer)
{
	TRACY_ZONE_BEGIN

	QueryPerformanceCounter(&s_begin);

	timer->begin = (double)s_begin.QuadPart;

	TRACY_ZONE_END
}
void timer_end(timer_t* timer)
{
	TRACY_ZONE_BEGIN

	QueryPerformanceCounter(&s_end);

	timer->ns = ((double)(s_end.QuadPart) * 1000000000.0 - timer->begin * 1000000000.0) / timer->freq;

	TRACY_ZONE_END
}
