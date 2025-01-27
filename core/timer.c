#include <stdint.h>

#include <windows.h>

#include <vega/core/timer.h>

static LARGE_INTEGER s_begin = { 0 };
static LARGE_INTEGER s_end = { 0 };

timer_t timer_init(void)
{
	timer_t timer;
	memset(&timer, 0, sizeof(timer_t));

	LARGE_INTEGER freq;
	QueryPerformanceFrequency(&freq);

	timer.freq = (double)freq.QuadPart;
	timer.begin = 0.0;
	timer.ns = 0.0;

	return timer;
}
void timer_begin(timer_t* timer)
{
	QueryPerformanceCounter(&s_begin);

	timer->begin = (double)s_begin.QuadPart;
}
void timer_end(timer_t* timer)
{
	QueryPerformanceCounter(&s_end);

	timer->ns = ((double)(s_end.QuadPart) * 1000000000.0 - timer->begin * 1000000000.0) / timer->freq;
}
double timer_ns(timer_t* timer)
{
	return timer->ns;
}
double timer_us(timer_t* timer)
{
	return timer->ns / 1000.0;
}
double timer_ms(timer_t* timer)
{
	return timer->ns / 1000000.0;
}
