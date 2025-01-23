#include <stdio.h>
#include <stdint.h>

#include <windows.h>

#include <vega/core/debug/timer.h>

static LARGE_INTEGER s_freq = { 0 };
static LARGE_INTEGER s_begin = { 0 };
static LARGE_INTEGER s_end = { 0 };

void debug_timer_begin(void)
{
	QueryPerformanceFrequency(&s_freq);
	QueryPerformanceCounter(&s_begin);
}
void debug_timer_end(void)
{
	QueryPerformanceCounter(&s_end);
	int64_t elapsed_ns = ((int64_t)(s_end.QuadPart) * 1000000000LL - ((int64_t)s_begin.QuadPart) * 1000000000LL) / ((int64_t)s_freq.QuadPart);
	int64_t elapsed_us = elapsed_ns / 1000LL;
	int64_t elapsed_ms = elapsed_ns / 1000000LL;
	printf("%s:\n", __func__);
	printf("\t%zdns\n", elapsed_ns);
	printf("\t%zdus\n", elapsed_us);
	printf("\t%zdms\n", elapsed_ms);
}
