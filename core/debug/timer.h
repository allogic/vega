#ifndef VEGA_CORE_DEBUG_TIMER_H
#define VEGA_CORE_DEBUG_TIMER_H

#ifdef VEGA_DEBUG
	#define DEBUG_TIMER_BEGIN debug_timer_begin();
	#define DEBUG_TIMER_END debug_timer_end();
#else
	#define DEBUG_TIMER_BEGIN
	#define DEBUG_TIMER_END
#endif // VEGA_DEBUG

void debug_timer_begin(void);
void debug_timer_end(void);

#endif // VEGA_CORE_DEBUG_TIMER_H
