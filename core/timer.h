#ifndef VEGA_CORE_TIMER_H
#define VEGA_CORE_TIMER_H

typedef struct _timer_t
{
	double freq;
	double begin;
	double ns;
} timer_t;

///////////////////////////////////////////////////////////////
// Public API
///////////////////////////////////////////////////////////////

timer_t timer_init(void);
void timer_begin(timer_t* timer);
void timer_end(timer_t* timer);

__forceinline double timer_ns(timer_t* timer);
__forceinline double timer_us(timer_t* timer);
__forceinline double timer_ms(timer_t* timer);

///////////////////////////////////////////////////////////////
// Inline Definition
///////////////////////////////////////////////////////////////

__forceinline double timer_ns(timer_t* timer)
{
	return timer->ns;
}
__forceinline double timer_us(timer_t* timer)
{
	return timer->ns / 1000.0;
}
__forceinline double timer_ms(timer_t* timer)
{
	return timer->ns / 1000000.0;
}

#endif // VEGA_CORE_TIMER_H
