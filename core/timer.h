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
double timer_ns(timer_t* timer);
double timer_us(timer_t* timer);
double timer_ms(timer_t* timer);

#endif // VEGA_CORE_TIMER_H
