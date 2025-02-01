#ifndef VEGA_CORE_STD_MAP_H
#define VEGA_CORE_STD_MAP_H

#include <stdint.h>

typedef struct _map_pair_t
{
	struct _map_pair_t* next;
	uint8_t* key;
	uint64_t key_size;
	uint8_t* value;
	uint64_t value_size;
} map_pair_t;

typedef struct _map_t
{
	map_pair_t** table;
	uint64_t table_size;
	uint64_t table_count;
	uint64_t pair_count;
} map_t;

typedef struct _map_iter_t
{
	map_pair_t* curr;
	map_t* map;
	uint64_t table_index;
} map_iter_t;

///////////////////////////////////////////////////////////////
// Public API
///////////////////////////////////////////////////////////////

map_t std_map_alloc(void);
void* std_map_get(map_t* map, void const* key, uint64_t key_size, uint64_t* value_size);
void* std_map_insert(map_t* map, void const* key, uint64_t key_size, void const* value, uint64_t value_size);
uint8_t std_map_remove(map_t* map, void const* key, uint64_t key_size);
uint8_t std_map_contains(map_t* map, void const* key, uint64_t key_size);
uint64_t std_map_count(map_t* map);
void std_map_clear(map_t* map);
map_iter_t std_map_begin(map_t* map);
void* std_map_key(map_iter_t* iter, uint64_t* key_size);
void* std_map_value(map_iter_t* iter, uint64_t* value_size);
uint8_t std_map_end(map_iter_t* iter);
void std_map_free(map_t* map);

///////////////////////////////////////////////////////////////
// Internal API
///////////////////////////////////////////////////////////////

void std_map_expand(map_t* map);
uint64_t std_map_hash(map_t* map, void const* key, uint64_t key_size, uint64_t modulus);
uint8_t std_map_load_factor(map_t* map);

#endif // VEGA_CORE_STD_MAP_H
