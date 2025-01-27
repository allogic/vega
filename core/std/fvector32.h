#ifndef VEGA_CORE_STD_FVECTOR32_H
#define VEGA_CORE_STD_FVECTOR32_H

#include <stdint.h>

typedef struct _fvector32_t
{
	uint32_t* buffer;
	uint64_t buffer_capacity;
	uint64_t buffer_count;
} fvector32_t;

///////////////////////////////////////////////////////////////
// Public API
///////////////////////////////////////////////////////////////

fvector32_t std_fvector32_alloc(void);
void std_fvector32_push(fvector32_t* vector, uint32_t value);
void std_fvector32_pop(fvector32_t* vector, uint32_t* value);
void std_fvector32_resize(fvector32_t* vector, uint64_t count);
uint8_t std_fvector32_remove(fvector32_t* vector, uint32_t value);
void std_fvector32_swap(fvector32_t* vector, uint64_t left_index, uint64_t right_index);
void std_fvector32_clear(fvector32_t* vector);
uint32_t std_fvector32_back(fvector32_t* vector);
uint32_t std_fvector32_front(fvector32_t* vector);
uint32_t std_fvector32_at(fvector32_t* vector, uint64_t index);
uint32_t* std_fvector32_buffer(fvector32_t* vector);
void std_fvector32_set(fvector32_t* vector, uint64_t index, uint32_t value);
uint8_t std_fvector32_empty(fvector32_t* vector);
uint64_t std_fvector32_size(fvector32_t* vector);
uint64_t std_fvector32_count(fvector32_t* vector);
uint64_t std_fvector32_capacity(fvector32_t* vector);
void std_fvector32_free(fvector32_t* vector);

///////////////////////////////////////////////////////////////
// Internal API
///////////////////////////////////////////////////////////////

void std_fvector32_expand(fvector32_t* vector);

#endif // VEGA_CORE_STD_FVECTOR32_H
