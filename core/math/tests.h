#ifndef VEGA_CORE_MATH_TESTS_H
#define VEGA_CORE_MATH_TESTS_H

#ifdef VEGA_DEBUG
	#define VEGA_MATH_TESTS math_tests();
#else
	#define VEGA_MATH_TESTS
#endif // VEGA_DEBUG

#ifdef VEGA_DEBUG
void math_tests();
#endif // VEGA_DEBUG

#endif // VEGA_CORE_MATH_TESTS_H
