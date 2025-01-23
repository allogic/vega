#ifndef VEGA_CORE_STD_STRING_H
#define VEGA_CORE_STD_STRING_H

#include <stdint.h>

typedef struct _string_t
{
	char* buffer;
	uint64_t buffer_capacity;
	uint64_t buffer_size;
} string_t;

///////////////////////////////////////////////////////////////
// Public API
///////////////////////////////////////////////////////////////

string_t std_string_alloc(void);
string_t std_string_from(char const* value, uint64_t size);
void std_string_fill(string_t* string, char value);
uint8_t std_string_equal(string_t* string, string_t* ref);
void std_string_append(string_t* string, char const* value, uint64_t size);
void std_string_resize(string_t* string, uint64_t size);
void std_string_clear(string_t* string);
char std_string_at(string_t* string, uint64_t index);
char* std_string_buffer(string_t* string);
void std_string_set(string_t* string, char const* value);
uint8_t std_string_empty(string_t* string);
uint64_t std_string_size(string_t* string);
uint64_t std_string_capacity(string_t* string);
void std_string_free(string_t* string);

///////////////////////////////////////////////////////////////
// Internal API
///////////////////////////////////////////////////////////////

void std_string_expand(string_t* string);

#endif // VEGA_CORE_STD_STRING_H
