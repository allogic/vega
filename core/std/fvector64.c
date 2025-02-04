#include <string.h>

#include <vega/core/heap.h>
#include <vega/core/macros.h>

#include <vega/core/debug/tracy.h>

#include <vega/core/std/fvector64.h>

#ifndef TRACY_ZONE_BEGIN
	#define TRACY_ZONE_BEGIN TracyCZoneC(ctx, TRACY_COLOR_RED, 1U);
#endif // TRACY_ZONE_BEGIN

#ifndef TRACY_ZONE_END
	#define TRACY_ZONE_END TracyCZoneEnd(ctx);
#endif // TRACY_ZONE_END

#ifndef FVECTOR64_BUFFER_CAPACITY
	#define FVECTOR64_BUFFER_CAPACITY (16ULL)
#endif // FVECTOR64_BUFFER_CAPACITY

#ifndef FVECTOR64_BUFFER_ALIGNMENT
	#define FVECTOR64_BUFFER_ALIGNMENT (16ULL)
#endif // FVECTOR64_BUFFER_ALIGNMENT

fvector64_t std_fvector64_alloc(void)
{
	TRACY_ZONE_BEGIN

	fvector64_t vector = { 0 };
	vector.buffer = (uint64_t*)heap_alloc(VEGA_ALIGN_UP_BY(FVECTOR64_BUFFER_CAPACITY, FVECTOR64_BUFFER_ALIGNMENT) * sizeof(uint64_t));
	vector.buffer_capacity = VEGA_ALIGN_UP_BY(FVECTOR64_BUFFER_CAPACITY, FVECTOR64_BUFFER_ALIGNMENT);
	vector.buffer_count = 0;

	TRACY_ZONE_END

	return vector;
}
void std_fvector64_fill(fvector64_t* vector, uint64_t value)
{
	uint64_t buffer_index = 0;
	while (buffer_index < vector->buffer_count)
	{
		*(vector->buffer + buffer_index) = value;
		buffer_index++;
	}
}
void std_fvector64_push(fvector64_t* vector, uint64_t value)
{
	TRACY_ZONE_BEGIN

	vector->buffer[vector->buffer_count] = value;
	vector->buffer_count++;

	if (vector->buffer_count >= vector->buffer_capacity)
	{
		std_fvector64_expand(vector);
	}

	TRACY_ZONE_END
}
void std_fvector64_pop(fvector64_t* vector, uint64_t* value)
{
	TRACY_ZONE_BEGIN

	vector->buffer_count--;
	*value = vector->buffer[vector->buffer_count];

	TRACY_ZONE_END
}
void std_fvector64_resize(fvector64_t* vector, uint64_t count)
{
	TRACY_ZONE_BEGIN

	uint64_t buffer_count = count;
	uint64_t buffer_capacity = 0;

	if (count)
	{
		buffer_capacity = VEGA_ALIGN_UP_BY(count, FVECTOR64_BUFFER_ALIGNMENT) * 2;
	}
	else
	{
		buffer_capacity = FVECTOR64_BUFFER_ALIGNMENT * 2;
	}

	vector->buffer_capacity = buffer_capacity;
	vector->buffer_count = buffer_count;
	vector->buffer = (uint64_t*)heap_realloc(vector->buffer, vector->buffer_capacity * sizeof(uint64_t));

	TRACY_ZONE_END
}
uint8_t std_fvector64_remove(fvector64_t* vector, uint64_t value)
{
	TRACY_ZONE_BEGIN

	uint8_t removed = 0;

	uint64_t buffer_index = 0;
	while (buffer_index < vector->buffer_count)
	{
		if (vector->buffer[buffer_index] == value)
		{
			break;
		}

		buffer_index++;
	}

	if (buffer_index < vector->buffer_count)
	{
		std_fvector64_swap(vector, buffer_index, vector->buffer_count - 1);

		vector->buffer_count--;

		removed = 1;
	}

	TRACY_ZONE_END

	return removed;
}
void std_fvector64_swap(fvector64_t* vector, uint64_t left_index, uint64_t right_index)
{
	TRACY_ZONE_BEGIN

	uint64_t swap_value = vector->buffer[left_index];
	vector->buffer[left_index] = vector->buffer[right_index];
	vector->buffer[right_index] = swap_value;

	TRACY_ZONE_END
}
void std_fvector64_clear(fvector64_t* vector)
{
	TRACY_ZONE_BEGIN

	vector->buffer_count = 0;

	TRACY_ZONE_END
}
uint64_t std_fvector64_back(fvector64_t* vector)
{
	TRACY_ZONE_BEGIN

	uint64_t value = vector->buffer[vector->buffer_count];

	TRACY_ZONE_END

	return value;
}
uint64_t std_fvector64_front(fvector64_t* vector)
{
	TRACY_ZONE_BEGIN

	uint64_t value = vector->buffer[0];

	TRACY_ZONE_END

	return value;
}
uint64_t std_fvector64_get(fvector64_t* vector, uint64_t index)
{
	TRACY_ZONE_BEGIN

	uint64_t value = vector->buffer[index];

	TRACY_ZONE_END

	return value;
}
uint64_t* std_fvector64_buffer(fvector64_t* vector)
{
	TRACY_ZONE_BEGIN

	uint64_t* buffer = vector->buffer;

	TRACY_ZONE_END

	return buffer;
}
void std_fvector64_set(fvector64_t* vector, uint64_t index, uint64_t value)
{
	TRACY_ZONE_BEGIN

	vector->buffer[index] = value;

	TRACY_ZONE_END
}
uint8_t std_fvector64_empty(fvector64_t* vector)
{
	TRACY_ZONE_BEGIN

	uint8_t empty = vector->buffer_count == 0;

	TRACY_ZONE_END

	return empty;
}
uint64_t std_fvector64_size(fvector64_t* vector)
{
	TRACY_ZONE_BEGIN

	uint64_t buffer_size = vector->buffer_count * sizeof(uint64_t);

	TRACY_ZONE_END

	return buffer_size;
}
uint64_t std_fvector64_count(fvector64_t* vector)
{
	TRACY_ZONE_BEGIN

	uint64_t buffer_count = vector->buffer_count;

	TRACY_ZONE_END

	return buffer_count;
}
uint64_t std_fvector64_capacity(fvector64_t* vector)
{
	TRACY_ZONE_BEGIN

	uint64_t buffer_capacity = vector->buffer_capacity;

	TRACY_ZONE_END

	return buffer_capacity;
}
void std_fvector64_free(fvector64_t* vector)
{
	TRACY_ZONE_BEGIN

	heap_free(vector->buffer);

	memset(vector, 0, sizeof(fvector64_t));

	TRACY_ZONE_END
}
void std_fvector64_expand(fvector64_t* vector)
{
	TRACY_ZONE_BEGIN

	vector->buffer_capacity = VEGA_ALIGN_UP_BY(vector->buffer_count, FVECTOR64_BUFFER_ALIGNMENT) * 2;
	vector->buffer = (uint64_t*)heap_realloc(vector->buffer, vector->buffer_capacity * sizeof(uint64_t));

	TRACY_ZONE_END
}
