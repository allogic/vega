#ifndef VEGA_CORE_STD_SET_H
#define VEGA_CORE_STD_SET_H

#include <stdint.h>

typedef struct _set_entry_t
{
	struct _set_entry_t* next;
	uint8_t* key;
	uint64_t key_size;
} set_entry_t;

typedef struct _set_t
{
	set_entry_t** table;
	uint64_t table_size;
	uint64_t table_count;
	uint64_t entry_count;
} set_t;

///////////////////////////////////////////////////////////////
// Public API
///////////////////////////////////////////////////////////////

set_t std_set_alloc(void);
uint8_t std_set_insert(set_t* set, void const* key, uint64_t key_size);
uint8_t std_set_remove(set_t* set, void const* key, uint64_t key_size);
uint8_t std_set_contains(set_t* set, void const* key, uint64_t key_size);
uint64_t std_set_count(set_t* set);
void std_set_clear(set_t* set);
void std_set_free(set_t* set);

///////////////////////////////////////////////////////////////
// Internal API
///////////////////////////////////////////////////////////////

void std_set_expand(set_t* set);
uint64_t std_set_hash(set_t* set, void const* key, uint64_t key_size, uint64_t modulus);
uint8_t std_set_load_factor(set_t* set);

#endif // VEGA_CORE_STD_SET_H
