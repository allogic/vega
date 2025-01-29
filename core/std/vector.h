#ifndef VEGA_CORE_STD_VECTOR_H
#define VEGA_CORE_STD_VECTOR_H

#include <stdint.h>

typedef struct _vector_t
{
	uint8_t* buffer;
	uint8_t* swap_buffer;
	uint64_t value_size;
	uint64_t buffer_capacity;
	uint64_t buffer_size;
	uint64_t buffer_count;
} vector_t;

///////////////////////////////////////////////////////////////
// Public API
///////////////////////////////////////////////////////////////

vector_t std_vector_alloc(uint64_t value_size);
void std_vector_fill(vector_t* vector, void const* value);
void std_vector_push(vector_t* vector, void const* value);
void std_vector_pop(vector_t* vector, void* value);
void std_vector_resize(vector_t* vector, uint64_t count);
uint8_t std_vector_remove(vector_t* vector, void const* value);
void std_vector_swap(vector_t* vector, uint64_t left_index, uint64_t right_index);
void std_vector_clear(vector_t* vector);
void* std_vector_back(vector_t* vector);
void* std_vector_front(vector_t* vector);
void* std_vector_get(vector_t* vector, uint64_t index);
void* std_vector_buffer(vector_t* vector);
void std_vector_set(vector_t* vector, uint64_t index, void const* value);
uint8_t std_vector_empty(vector_t* vector);
uint64_t std_vector_size(vector_t* vector);
uint64_t std_vector_count(vector_t* vector);
uint64_t std_vector_capacity(vector_t* vector);
void std_vector_free(vector_t* vector);

///////////////////////////////////////////////////////////////
// Internal API
///////////////////////////////////////////////////////////////

void std_vector_expand(vector_t* vector);

#endif // VEGA_CORE_STD_VECTOR_H
