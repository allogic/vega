#ifndef VEGA_CORE_MATH_H
#define VEGA_CORE_MATH_H

#include <math.h>
#include <stdint.h>

#include <vega/core/macros.h>

#include <vega/core/math/forward.h>

VEGA_EXTERN_C_BEGIN

///////////////////////////////////////////////////////////////
// Inline Definition
///////////////////////////////////////////////////////////////

__forceinline double math_deg_to_rad(double a)
{
	return a * MATH_DEG_TO_RAD;
}
__forceinline double math_rad_to_deg(double a)
{
	return a * MATH_RAD_TO_DEG;
}
__forceinline double math_fast_inverse_sqrt(double a)
{
	double r = a;

	uint64_t i = *(uint64_t*)&r;

	i = 0x5fe6eb50c7b537a9 - (i >> 1);
	r = *(double*)&i;
	r = r * (1.5 - (a * 0.5 * r * r));

	return r;
}
__forceinline double math_clamp(double a, double min, double max)
{
	if (a > max) a = max;
	else if (a < min) a = min;
	return a;
}

VEGA_EXTERN_C_END

#endif // VEGA_CORE_MATH_H
