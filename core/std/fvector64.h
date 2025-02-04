#ifndef VEGA_CORE_STD_FVECTOR64_H
#define VEGA_CORE_STD_FVECTOR64_H

#include <stdint.h>

#include <vega/core/macros.h>

VEGA_EXTERN_C_BEGIN

typedef struct _fvector64_t
{
	uint64_t* buffer;
	uint64_t buffer_capacity;
	uint64_t buffer_count;
} fvector64_t;

///////////////////////////////////////////////////////////////
// Public API
///////////////////////////////////////////////////////////////

fvector64_t std_fvector64_alloc(void);
void std_fvector64_fill(fvector64_t* vector, uint64_t value);
void std_fvector64_push(fvector64_t* vector, uint64_t value);
void std_fvector64_pop(fvector64_t* vector, uint64_t* value);
void std_fvector64_resize(fvector64_t* vector, uint64_t count);
uint8_t std_fvector64_remove(fvector64_t* vector, uint64_t value);
void std_fvector64_swap(fvector64_t* vector, uint64_t left_index, uint64_t right_index);
void std_fvector64_clear(fvector64_t* vector);
uint64_t std_fvector64_back(fvector64_t* vector);
uint64_t std_fvector64_front(fvector64_t* vector);
uint64_t std_fvector64_get(fvector64_t* vector, uint64_t index);
uint64_t* std_fvector64_buffer(fvector64_t* vector);
void std_fvector64_set(fvector64_t* vector, uint64_t index, uint64_t value);
uint8_t std_fvector64_empty(fvector64_t* vector);
uint64_t std_fvector64_size(fvector64_t* vector);
uint64_t std_fvector64_count(fvector64_t* vector);
uint64_t std_fvector64_capacity(fvector64_t* vector);
void std_fvector64_free(fvector64_t* vector);

///////////////////////////////////////////////////////////////
// Internal API
///////////////////////////////////////////////////////////////

void std_fvector64_expand(fvector64_t* vector);

VEGA_EXTERN_C_END

#endif // VEGA_CORE_STD_FVECTOR64_H
