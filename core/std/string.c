#include <string.h>

#include <vega/core/heap.h>
#include <vega/core/macros.h>

#include <vega/core/debug/tracy.h>

#include <vega/core/std/string.h>

#ifndef TRACY_ZONE_BEGIN
	#define TRACY_ZONE_BEGIN TracyCZoneC(ctx, TRACY_COLOR_RED, 1U);
#endif // TRACY_ZONE_BEGIN

#ifndef TRACY_ZONE_END
	#define TRACY_ZONE_END TracyCZoneEnd(ctx);
#endif // TRACY_ZONE_END

#ifndef STRING_BUFFER_CAPACITY
	#define STRING_BUFFER_CAPACITY (16ULL)
#endif // STRING_BUFFER_CAPACITY

#ifndef STRING_BUFFER_ALIGNMENT
	#define STRING_BUFFER_ALIGNMENT (16ULL)
#endif // STRING_BUFFER_ALIGNMENT

string_t std_string_alloc(void)
{
	TRACY_ZONE_BEGIN

	string_t string = { 0 };
	string.buffer = (char*)heap_alloc(VEGA_ALIGN_UP_BY(STRING_BUFFER_CAPACITY, STRING_BUFFER_ALIGNMENT));
	string.buffer_capacity = VEGA_ALIGN_UP_BY(STRING_BUFFER_CAPACITY, STRING_BUFFER_ALIGNMENT);
	string.buffer_size = 0;
	string.buffer[0] = 0;

	TRACY_ZONE_END

	return string;
}
string_t std_string_from(char const* value, uint64_t size)
{
	TRACY_ZONE_BEGIN

	string_t string = std_string_alloc();

	if (value)
	{
		std_string_append(&string, value, size);
	}

	TRACY_ZONE_END

	return string;
}
void std_string_fill(string_t* string, char value)
{
	TRACY_ZONE_BEGIN

	if (value)
	{
		memset(string->buffer, value, string->buffer_size);

		string->buffer[string->buffer_size] = 0;
	}

	TRACY_ZONE_END
}
void std_string_append(string_t* string, char const* value, uint64_t size)
{
	TRACY_ZONE_BEGIN

	if (value)
	{
		string->buffer_size += size;

		while (string->buffer_size >= string->buffer_capacity)
		{
			std_string_expand(string);
		}

		memcpy(string->buffer + string->buffer_size - size, value, size);

		string->buffer[string->buffer_size] = 0;
	}

	TRACY_ZONE_END
}
void std_string_resize(string_t* string, uint64_t size)
{
	TRACY_ZONE_BEGIN

	uint64_t buffer_size = size;
	uint64_t buffer_capacity = 0;

	if (size)
	{
		buffer_capacity = VEGA_ALIGN_UP_BY(size + 1, STRING_BUFFER_ALIGNMENT);
	}
	else
	{
		buffer_capacity = VEGA_ALIGN_UP_BY(STRING_BUFFER_CAPACITY, STRING_BUFFER_ALIGNMENT);
	}

	string->buffer = (char*)heap_realloc(string->buffer, buffer_capacity);
	string->buffer_capacity = buffer_capacity;
	string->buffer_size = buffer_size;
	string->buffer[string->buffer_size] = 0;

	TRACY_ZONE_END
}
void std_string_clear(string_t* string)
{
	TRACY_ZONE_BEGIN

	string->buffer_size = 0;
	string->buffer[string->buffer_size] = 0;

	TRACY_ZONE_END
}
char std_string_at(string_t* string, uint64_t index)
{
	TRACY_ZONE_BEGIN

	char character = string->buffer[index];

	TRACY_ZONE_END

	return character;
}
char* std_string_buffer(string_t* string)
{
	TRACY_ZONE_BEGIN

	char* buffer = string->buffer;

	TRACY_ZONE_END

	return buffer;
}
void std_string_set(string_t* string, char const* value)
{
	TRACY_ZONE_BEGIN

	if (value)
	{
		uint64_t value_size = strlen(value);

		string->buffer_size = value_size;

		while (string->buffer_size >= string->buffer_capacity)
		{
			std_string_expand(string);
		}

		memcpy(string->buffer + string->buffer_size - value_size, value, value_size);

		string->buffer[string->buffer_size] = 0;
	}

	TRACY_ZONE_END
}
uint8_t std_string_empty(string_t* string)
{
	TRACY_ZONE_BEGIN

	uint8_t empty = string->buffer_size == 0;

	TRACY_ZONE_END

	return empty;
}
uint64_t std_string_size(string_t* string)
{
	TRACY_ZONE_BEGIN

	uint64_t size = string->buffer_size;

	TRACY_ZONE_END

	return size;
}
uint64_t std_string_capacity(string_t* string)
{
	TRACY_ZONE_BEGIN

	uint64_t capacity = string->buffer_capacity;

	TRACY_ZONE_END

	return capacity;
}
void std_string_free(string_t* string)
{
	TRACY_ZONE_BEGIN

	heap_free(string->buffer);

	memset(string, 0, sizeof(string_t));

	TRACY_ZONE_END
}
void std_string_expand(string_t* string)
{
	TRACY_ZONE_BEGIN

	uint64_t buffer_capacity = VEGA_ALIGN_UP_BY(string->buffer_size + 1, STRING_BUFFER_ALIGNMENT);

	string->buffer = (char*)heap_realloc(string->buffer, buffer_capacity);
	string->buffer_capacity = buffer_capacity;

	TRACY_ZONE_END
}
