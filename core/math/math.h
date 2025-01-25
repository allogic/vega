#ifndef VEGA_CORE_MATH_H
#define VEGA_CORE_MATH_H

#include <math.h>

#include <vega/core/math/forward.h>

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

#endif // VEGA_CORE_MATH_H
