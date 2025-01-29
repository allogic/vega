#include <string.h>

#include <vega/core/heap.h>
#include <vega/core/macros.h>

#include <vega/core/debug/tracy.h>

#include <vega/core/std/vector.h>

#ifndef TRACY_ZONE_BEGIN
	#define TRACY_ZONE_BEGIN TracyCZoneC(ctx, TRACY_COLOR_RED, 1U);
#endif // TRACY_ZONE_BEGIN

#ifndef TRACY_ZONE_END
	#define TRACY_ZONE_END TracyCZoneEnd(ctx);
#endif // TRACY_ZONE_END

#ifndef VECTOR_BUFFER_CAPACITY
	#define VECTOR_BUFFER_CAPACITY (16ULL)
#endif // VECTOR_BUFFER_CAPACITY

#ifndef VECTOR_BUFFER_ALIGNMENT
	#define VECTOR_BUFFER_ALIGNMENT (16ULL)
#endif // VECTOR_BUFFER_ALIGNMENT

vector_t std_vector_alloc(uint64_t value_size)
{
	TRACY_ZONE_BEGIN

	vector_t vector;
	memset(&vector, 0, sizeof(vector_t));

	vector.buffer = (uint8_t*)heap_alloc(ALIGN_UP_BY(VECTOR_BUFFER_CAPACITY, VECTOR_BUFFER_ALIGNMENT) * value_size);
	vector.swap_buffer = (uint8_t*)heap_alloc(value_size);
	vector.value_size = value_size;
	vector.buffer_capacity = ALIGN_UP_BY(VECTOR_BUFFER_CAPACITY, VECTOR_BUFFER_ALIGNMENT);
	vector.buffer_size = 0;
	vector.buffer_count = 0;

	TRACY_ZONE_END

	return vector;
}
void std_vector_fill(vector_t* vector, void const* value)
{
	if (value)
	{
		uint64_t buffer_index = 0;
		uint64_t buffer_offset = 0;
		while (buffer_index < vector->buffer_count)
		{
			memcpy(vector->buffer + buffer_offset, value, vector->value_size);
			buffer_index++;
			buffer_offset += vector->value_size;
		}
	}
}
void std_vector_push(vector_t* vector, void const* value)
{
	TRACY_ZONE_BEGIN

	if (value)
	{
		memcpy(vector->buffer + vector->buffer_size, value, vector->value_size);
	}

	vector->buffer_count++;
	vector->buffer_size += vector->value_size;

	if (vector->buffer_count >= vector->buffer_capacity)
	{
		std_vector_expand(vector);
	}

	TRACY_ZONE_END
}
void std_vector_pop(vector_t* vector, void* value)
{
	TRACY_ZONE_BEGIN

	vector->buffer_count--;
	vector->buffer_size -= vector->value_size;

	if (value)
	{
		memcpy(value, vector->buffer + vector->buffer_size, vector->value_size);
	}

	TRACY_ZONE_END
}
void std_vector_resize(vector_t* vector, uint64_t count)
{
	TRACY_ZONE_BEGIN

	uint64_t buffer_count = count;
	uint64_t buffer_size = count * vector->value_size;
	uint64_t buffer_capacity = 0;

	if (count)
	{
		buffer_capacity = ALIGN_UP_BY(count, VECTOR_BUFFER_ALIGNMENT) * 2;
	}
	else
	{
		buffer_capacity = VECTOR_BUFFER_ALIGNMENT * 2;
	}

	vector->buffer_capacity = buffer_capacity;
	vector->buffer_size = buffer_size;
	vector->buffer_count = buffer_count;
	vector->buffer = (uint8_t*)heap_realloc(vector->buffer, vector->buffer_capacity * vector->value_size);

	TRACY_ZONE_END
}
uint8_t std_vector_remove(vector_t* vector, void const* value)
{
	TRACY_ZONE_BEGIN

	uint8_t removed = 0;

	if (value)
	{
		uint64_t buffer_index = 0;
		uint64_t buffer_offset = 0;
		while (buffer_index < vector->buffer_count)
		{
			if (memcmp(vector->buffer + buffer_offset, value, vector->value_size) == 0)
			{
				break;
			}

			buffer_index++;
			buffer_offset += vector->value_size;
		}

		if (buffer_index < vector->buffer_count)
		{
			std_vector_swap(vector, buffer_index, vector->buffer_count - 1);

			vector->buffer_count--;
			vector->buffer_size -= vector->value_size;

			removed = 1;
		}
	}

	TRACY_ZONE_END

	return removed;
}
void std_vector_swap(vector_t* vector, uint64_t left_index, uint64_t right_index)
{
	TRACY_ZONE_BEGIN

	uint8_t* left = vector->buffer + left_index * vector->value_size;
	uint8_t* right = vector->buffer + right_index * vector->value_size;

	memcpy(vector->swap_buffer, left, vector->value_size);
	memcpy(left, right, vector->value_size);
	memcpy(right, vector->swap_buffer, vector->value_size);

	TRACY_ZONE_END
}
void std_vector_clear(vector_t* vector)
{
	TRACY_ZONE_BEGIN

	vector->buffer_size = 0;
	vector->buffer_count = 0;

	TRACY_ZONE_END
}
void* std_vector_back(vector_t* vector)
{
	TRACY_ZONE_BEGIN

	void* value = vector->buffer + vector->buffer_size - vector->value_size;

	TRACY_ZONE_END

	return value;
}
void* std_vector_front(vector_t* vector)
{
	TRACY_ZONE_BEGIN

	void* value = vector->buffer;

	TRACY_ZONE_END

	return value;
}
void* std_vector_get(vector_t* vector, uint64_t index)
{
	TRACY_ZONE_BEGIN

	void* value = vector->buffer + index * vector->value_size;

	TRACY_ZONE_END

	return value;
}
void* std_vector_buffer(vector_t* vector)
{
	TRACY_ZONE_BEGIN

	void* buffer = vector->buffer;

	TRACY_ZONE_END

	return buffer;
}
void std_vector_set(vector_t* vector, uint64_t index, void const* value)
{
	TRACY_ZONE_BEGIN

	if (value)
	{
		memcpy(vector->buffer + index * vector->value_size, value, vector->value_size);
	}

	TRACY_ZONE_END
}
uint8_t std_vector_empty(vector_t* vector)
{
	TRACY_ZONE_BEGIN

	uint8_t empty = vector->buffer_count == 0;

	TRACY_ZONE_END

	return empty;
}
uint64_t std_vector_size(vector_t* vector)
{
	TRACY_ZONE_BEGIN

	uint64_t buffer_size = vector->buffer_size;

	TRACY_ZONE_END

	return buffer_size;
}
uint64_t std_vector_count(vector_t* vector)
{
	TRACY_ZONE_BEGIN

	uint64_t buffer_count = vector->buffer_count;

	TRACY_ZONE_END

	return buffer_count;
}
uint64_t std_vector_capacity(vector_t* vector)
{
	TRACY_ZONE_BEGIN

	uint64_t buffer_capacity = vector->buffer_capacity;

	TRACY_ZONE_END

	return buffer_capacity;
}
void std_vector_free(vector_t* vector)
{
	TRACY_ZONE_BEGIN

	heap_free(vector->buffer);
	heap_free(vector->swap_buffer);

	memset(vector, 0, sizeof(vector_t));

	TRACY_ZONE_END
}
void std_vector_expand(vector_t* vector)
{
	TRACY_ZONE_BEGIN

	vector->buffer_capacity = ALIGN_UP_BY(vector->buffer_count, VECTOR_BUFFER_ALIGNMENT) * 2;
	vector->buffer = (uint8_t*)heap_realloc(vector->buffer, vector->buffer_capacity * vector->value_size);

	TRACY_ZONE_END
}
