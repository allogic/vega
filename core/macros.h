#ifndef VEGA_CORE_MACROS_H
#define VEGA_CORE_MACROS_H

#ifndef VEGA_ASSERT
	#define VEGA_ASSERT(EXPRESSION, MESSAGE) (assert((EXPRESSION) && MESSAGE))
#endif // VEGA_ASSERT

#ifndef VEGA_OFFSET_OF
	#define VEGA_OFFSET_OF(TYPE, MEMBER) ((uint64_t)&(((TYPE*)0)->MEMBER))
#endif // VEGA_OFFSET_OF

#ifndef VEGA_ARRAY_COUNT
	#define VEGA_ARRAY_COUNT(ARRAY) ((uint64_t)(sizeof(ARRAY) / sizeof((ARRAY)[0])))
#endif // VEGA_ARRAY_COUNT

#ifndef VEGA_MAX
	#define VEGA_MAX(A, B) (((A) > (B)) ? (A) : (B))
#endif // VEGA_MAX

#ifndef VEGA_MIN
	#define VEGA_MIN(A, B) (((A) < (B)) ? (A) : (B))
#endif // VEGA_MIN

#ifndef VEGA_ALIGN_DOWN_BY
	#define VEGA_ALIGN_DOWN_BY(VALUE, ALIGNMENT) (((uint64_t)(VALUE)) & ~(((uint64_t)(ALIGNMENT)) - 1))
#endif // VEGA_ALIGN_DOWN_BY

#ifndef VEGA_ALIGN_UP_BY
	#define VEGA_ALIGN_UP_BY(VALUE, ALIGNMENT) ((((uint64_t)(VALUE)) + (((uint64_t)(ALIGNMENT)) - 1)) & ~(((uint64_t)(ALIGNMENT)) - 1))
#endif // VEGA_ALIGN_UP_BY

#ifndef VEGA_IS_BIT_SET
	#define VEGA_IS_BIT_SET(VALUE, BIT) (((uint64_t)(VALUE)) & (((uint64_t)1) << (BIT)))
#endif // VEGA_IS_BIT_SET

#ifndef VEGA_IS_BIT_NOT_SET
	#define VEGA_IS_BIT_NOT_SET(VALUE, BIT) (!(((uint64_t)(VALUE)) & (((uint64_t)1) << (BIT))))
#endif // VEGA_IS_BIT_NOT_SET

#ifndef VEGA_SET_BIT
	#define VEGA_SET_BIT(VALUE, BIT) (((uint64_t)(VALUE)) | (((uint64_t)1) << (BIT)))
#endif // VEGA_SET_BIT

#ifndef VEGA_CLEAR_BIT
	#define VEGA_CLEAR_BIT(VALUE, BIT) (((uint64_t)(VALUE)) & (~(((uint64_t)1) << (BIT))))
#endif // VEGA_CLEAR_BIT

#ifndef VEGA_TOGGLE_BIT
	#define VEGA_TOGGLE_BIT(VALUE, BIT) (((uint64_t)(VALUE)) ^ (((uint64_t)1) << (BIT)))
#endif // VEGA_TOGGLE_BIT

#ifndef VEGA_EXTERN_C_BEGIN
	#ifdef __cplusplus
		#define VEGA_EXTERN_C_BEGIN extern "C" {
	#else
		#define VEGA_EXTERN_C_BEGIN
	#endif // __cplusplus
#endif // VEGA_EXTERN_C_BEGIN

#ifndef VEGA_EXTERN_C_END
	#ifdef __cplusplus
		#define VEGA_EXTERN_C_END }
	#else
		#define VEGA_EXTERN_C_END
	#endif // __cplusplus
#endif // VEGA_EXTERN_C_END

#endif // VEGA_CORE_MACROS_H
