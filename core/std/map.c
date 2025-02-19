#include <string.h>

#include <vega/core/heap.h>
#include <vega/core/macros.h>

#include <vega/core/debug/tracy.h>

#include <vega/core/std/map.h>

#ifndef TRACY_ZONE_BEGIN
	#define TRACY_ZONE_BEGIN TracyCZoneC(ctx, TRACY_COLOR_RED, 1U);
#endif // TRACY_ZONE_BEGIN

#ifndef TRACY_ZONE_END
	#define TRACY_ZONE_END TracyCZoneEnd(ctx);
#endif // TRACY_ZONE_END

#ifndef MAP_TABLE_COUNT
	#define MAP_TABLE_COUNT (128ULL)
#endif // MAP_TABLE_COUNT

#ifndef MAP_HASH_VALUE
	#define MAP_HASH_VALUE (5381ULL)
#endif // MAP_HASH_VALUE

#ifndef MAP_LOAD_FACTOR
	#define MAP_LOAD_FACTOR (75ULL)
#endif // MAP_LOAD_FACTOR

map_t std_map_alloc()
{
	TRACY_ZONE_BEGIN

	map_t map = { 0 };
	map.table = (map_pair_t**)heap_alloc(MAP_TABLE_COUNT * sizeof(map_pair_t*));
	map.table_size = MAP_TABLE_COUNT * sizeof(map_pair_t*);
	map.table_count = MAP_TABLE_COUNT;
	map.pair_count = 0;

	memset(map.table, 0, MAP_TABLE_COUNT * sizeof(map_pair_t*));

	TRACY_ZONE_END

	return map;
}
void* std_map_get(map_t* map, void const* key, uint64_t key_size, uint64_t* value_size)
{
	TRACY_ZONE_BEGIN

	void* existing_value = 0;

	uint64_t hash = std_map_hash(map, key, key_size, map->table_count);

	map_pair_t* curr = map->table[hash];
	while (curr)
	{
		if (curr->key_size == key_size)
		{
			if (memcmp(curr->key, key, curr->key_size) == 0)
			{
				existing_value = curr->value;

				if (value_size)
				{
					*value_size = curr->value_size;
				}

				break;
			}
		}

		curr = curr->next;
	}

	TRACY_ZONE_END

	return existing_value;
}
void* std_map_insert(map_t* map, void const* key, uint64_t key_size, void const* value, uint64_t value_size)
{
	TRACY_ZONE_BEGIN

	void* existing_value = 0;

	uint8_t load_factor = std_map_load_factor(map);

	if (load_factor > MAP_LOAD_FACTOR)
	{
		std_map_expand(map);
	}

	uint64_t hash = std_map_hash(map, key, key_size, map->table_count);

	map_pair_t* curr = map->table[hash];
	while (curr)
	{
		if (curr->key_size == key_size)
		{
			if (memcmp(curr->key, key, curr->key_size) == 0)
			{
				existing_value = curr->value;

				break;
			}
		}

		curr = curr->next;
	}

	if (existing_value == 0)
	{
		curr = (map_pair_t*)heap_alloc(sizeof(map_pair_t));
		memset(curr, 0, sizeof(map_pair_t));

		curr->next = map->table[hash];
		curr->key = (uint8_t*)heap_alloc(key_size);
		curr->value = (uint8_t*)heap_alloc(value_size);

		curr->key_size = key_size;
		memcpy(curr->key, key, key_size);

		if (value)
		{
			memcpy(curr->value, value, value_size);
		}
		else
		{
			memset(curr->value, 0, value_size);
		}

		curr->value_size = value_size;

		existing_value = curr->value;

		map->table[hash] = curr;
		map->pair_count++;
	}

	TRACY_ZONE_END

	return existing_value;
}
uint8_t std_map_remove(map_t* map, void const* key, uint64_t key_size)
{
	TRACY_ZONE_BEGIN

	uint8_t removed = 0;

	uint64_t hash = std_map_hash(map, key, key_size, map->table_count);

	map_pair_t* curr = map->table[hash];
	map_pair_t* prev = 0;
	while (curr)
	{
		if (curr->key_size == key_size)
		{
			if (memcmp(curr->key, key, curr->key_size) == 0)
			{
				if (prev)
				{
					prev->next = curr->next;
				}
				else
				{
					map->table[hash] = curr->next;
				}

				heap_free(curr->key);
				heap_free(curr->value);
				heap_free(curr);

				map->pair_count--;

				removed = 1;

				break;
			}
		}

		prev = curr;
		curr = curr->next;
	}

	TRACY_ZONE_END

	return removed;
}
uint8_t std_map_contains(map_t* map, void const* key, uint64_t key_size)
{
	TRACY_ZONE_BEGIN

	uint8_t contains = 0;

	uint64_t hash = std_map_hash(map, key, key_size, map->table_count);

	map_pair_t* curr = map->table[hash];
	while (curr)
	{
		if (curr->key_size == key_size)
		{
			if (memcmp(curr->key, key, curr->key_size) == 0)
			{
				contains = 1;

				break;
			}
		}

		curr = curr->next;
	}

	TRACY_ZONE_END

	return contains;
}
uint64_t std_map_count(map_t* map)
{
	TRACY_ZONE_BEGIN

	uint64_t pair_count = map->pair_count;

	TRACY_ZONE_END

	return pair_count;
}
void std_map_clear(map_t* map)
{
	TRACY_ZONE_BEGIN

	uint64_t table_index = 0;
	while (table_index < map->table_count)
	{
		map_pair_t* curr = map->table[table_index];
		while (curr)
		{
			map_pair_t* tmp = curr;

			curr = curr->next;

			heap_free(tmp->key);
			heap_free(tmp->value);
			heap_free(tmp);
		}

		table_index++;
	}

	memset(map->table, 0, map->table_size);

	map->pair_count = 0;

	TRACY_ZONE_END
}
map_iter_t std_map_begin(map_t* map)
{
	TRACY_ZONE_BEGIN

	map_iter_t iter = { 0 };

	iter.curr = 0;
	iter.map = map;
	iter.table_index = 0;

	TRACY_ZONE_END

	return iter;
}
void* std_map_key(map_iter_t* iter, uint64_t* key_size)
{
	TRACY_ZONE_BEGIN

	void* key = iter->curr->key;

	if (key_size)
	{
		*key_size = iter->curr->key_size;
	}

	TRACY_ZONE_END

	return key;
}
void* std_map_value(map_iter_t* iter, uint64_t* value_size)
{
	TRACY_ZONE_BEGIN

	void* value = iter->curr->value;

	if (value_size)
	{
		*value_size = iter->curr->value_size;
	}

	TRACY_ZONE_END

	return value;
}
uint8_t std_map_end(map_iter_t* iter)
{
	TRACY_ZONE_BEGIN

	uint8_t is_end = 1;

	if (iter->curr && iter->curr->next)
	{
		iter->curr = iter->curr->next;

		is_end = 0;
	}
	else if (iter->curr)
	{
		while (iter->table_index < iter->map->table_count)
		{
			iter->curr = iter->map->table[iter->table_index++];

			if (iter->curr)
			{
				is_end = 0;

				break;
			}
		}
	}
	else
	{
		while (iter->table_index < iter->map->table_count)
		{
			iter->curr = iter->map->table[iter->table_index++];

			if (iter->curr)
			{
				is_end = 0;

				break;
			}
		}
	}

	TRACY_ZONE_END

	return is_end;
}
void std_map_free(map_t* map)
{
	TRACY_ZONE_BEGIN

	uint64_t table_index = 0;
	while (table_index < map->table_count)
	{
		map_pair_t* curr = map->table[table_index];
		while (curr)
		{
			map_pair_t* tmp = curr;

			curr = curr->next;

			heap_free(tmp->key);
			heap_free(tmp->value);
			heap_free(tmp);
		}

		table_index++;
	}

	heap_free(map->table);

	memset(map, 0, sizeof(map_t));

	TRACY_ZONE_END
}
void std_map_expand(map_t* map)
{
	TRACY_ZONE_BEGIN

	uint64_t table_index = 0;

	uint64_t table_size = map->table_size * 2;
	uint64_t table_count = map->table_count * 2;

	map_pair_t** table = (map_pair_t**)heap_alloc(table_size);
	memset(table, 0, table_size);

	while (table_index < map->table_count)
	{
		map_pair_t* curr = map->table[table_index];
		while (curr)
		{
			uint64_t hash = std_map_hash(map, curr->key, curr->key_size, table_count);

			curr->next = table[hash];
			table[hash] = curr;
			curr = curr->next;
		}

		table_index++;
	}

	heap_free(map->table);

	map->table = table;
	map->table_size = table_size;
	map->table_count = table_count;

	TRACY_ZONE_END
}
uint64_t std_map_hash(map_t* map, void const* key, uint64_t key_size, uint64_t modulus)
{
	TRACY_ZONE_BEGIN

	uint64_t hash = MAP_HASH_VALUE;

	uint64_t key_index = 0;
	while (key_index < key_size)
	{
		hash = ((hash << 5) + hash) + *(((uint8_t*)key) + key_index);

		key_index++;
	}

	hash %= modulus;

	TRACY_ZONE_END

	return hash;
}
uint8_t std_map_load_factor(map_t* map)
{
	TRACY_ZONE_BEGIN

	uint8_t load_factor = (uint8_t)((((float)map->pair_count + 1.0F) / map->table_count) * 100);

	TRACY_ZONE_END

	return load_factor;
}
