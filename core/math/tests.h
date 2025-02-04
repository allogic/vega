#ifndef VEGA_CORE_MATH_TESTS_H
#define VEGA_CORE_MATH_TESTS_H

#include <stdint.h>

#include <vega/core/macros.h>

#ifdef VEGA_DEBUG
	#define VEGA_MATH_TESTS math_tests();
#else
	#define VEGA_MATH_TESTS
#endif // VEGA_DEBUG

VEGA_EXTERN_C_BEGIN

#ifdef VEGA_DEBUG
void math_tests();
#endif // VEGA_DEBUG

VEGA_EXTERN_C_END

#endif // VEGA_CORE_MATH_TESTS_H
