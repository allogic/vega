#ifndef VEGA_CORE_MACROS_H
#define VEGA_CORE_MACROS_H

// TODO: rename macros to VEGA_

#ifndef VEGA_ASSERT
	#define VEGA_ASSERT(EXPRESSION, MESSAGE) (assert((EXPRESSION) && MESSAGE))
#endif // VEGA_ASSERT

#ifndef VEGA_OFFSET_OF
	#define VEGA_OFFSET_OF(TYPE, MEMBER) ((uint64_t)&(((TYPE*)0)->MEMBER))
#endif // VEGA_OFFSET_OF

#ifndef ARRAY_COUNT
	#define ARRAY_COUNT(ARRAY) ((uint64_t)(sizeof(ARRAY) / sizeof((ARRAY)[0])))
#endif // ARRAY_COUNT

#ifndef MAX
	#define MAX(A, B) (((A) > (B)) ? (A) : (B))
#endif // MAX

#ifndef MIN
	#define MIN(A, B) (((A) < (B)) ? (A) : (B))
#endif // MIN

#ifndef ALIGN_DOWN_BY
	#define ALIGN_DOWN_BY(VALUE, ALIGNMENT) (((uint64_t)(VALUE)) & ~(((uint64_t)(ALIGNMENT)) - 1))
#endif // ALIGN_DOWN_BY

#ifndef ALIGN_UP_BY
	#define ALIGN_UP_BY(VALUE, ALIGNMENT) ((((uint64_t)(VALUE)) + (((uint64_t)(ALIGNMENT)) - 1)) & ~(((uint64_t)(ALIGNMENT)) - 1))
#endif // ALIGN_UP_BY

#ifndef IS_BIT_SET
	#define IS_BIT_SET(VALUE, BIT) (((uint64_t)(VALUE)) & (((uint64_t)1) << (BIT)))
#endif // IS_BIT_SET

#ifndef IS_BIT_NOT_SET
	#define IS_BIT_NOT_SET(VALUE, BIT) (!(((uint64_t)(VALUE)) & (((uint64_t)1) << (BIT))))
#endif // IS_BIT_NOT_SET

#ifndef SET_BIT
	#define SET_BIT(VALUE, BIT) (((uint64_t)(VALUE)) | (((uint64_t)1) << (BIT)))
#endif // SET_BIT

#ifndef CLEAR_BIT
	#define CLEAR_BIT(VALUE, BIT) (((uint64_t)(VALUE)) & (~(((uint64_t)1) << (BIT))))
#endif // CLEAR_BIT

#ifndef TOGGLE_BIT
	#define TOGGLE_BIT(VALUE, BIT) (((uint64_t)(VALUE)) ^ (((uint64_t)1) << (BIT)))
#endif // TOGGLE_BIT

#endif // VEGA_CORE_MACROS_H
