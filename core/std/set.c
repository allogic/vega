#include <string.h>

#include <vega/core/heap.h>
#include <vega/core/macros.h>

#include <vega/core/debug/tracy.h>

#include <vega/core/std/set.h>

#ifndef TRACY_ZONE_BEGIN
	#define TRACY_ZONE_BEGIN TracyCZoneC(ctx, TRACY_COLOR_RED, 1U)
#endif // TRACY_ZONE_BEGIN

#ifndef TRACY_ZONE_END
	#define TRACY_ZONE_END TracyCZoneEnd(ctx)
#endif // TRACY_ZONE_END

#ifndef SET_TABLE_COUNT
	#define SET_TABLE_COUNT (128ULL)
#endif // SET_TABLE_COUNT

#ifndef SET_HASH_VALUE
	#define SET_HASH_VALUE (5381ULL)
#endif // SET_HASH_VALUE

#ifndef SET_LOAD_FACTOR
	#define SET_LOAD_FACTOR (75ULL)
#endif // SET_LOAD_FACTOR

set_t std_set_alloc(void)
{
	TRACY_ZONE_BEGIN

	set_t set;
	memset(&set, 0, sizeof(set_t));

	set.table = (set_entry_t**)heap_alloc(SET_TABLE_COUNT * sizeof(set_entry_t*));
	set.table_size = SET_TABLE_COUNT * sizeof(set_entry_t*);
	set.table_count = SET_TABLE_COUNT;
	set.entry_count = 0;

	memset(set.table, 0, SET_TABLE_COUNT * sizeof(set_entry_t*));

	TRACY_ZONE_END

	return set;
}
uint8_t std_set_insert(set_t* set, void const* key, uint64_t key_size)
{
	TRACY_ZONE_BEGIN

	uint8_t key_exists = 0;

	uint8_t load_factor = std_set_load_factor(set);
	if (load_factor > SET_LOAD_FACTOR)
	{
		std_set_expand(set);
	}

	uint64_t hash = std_set_hash(set, key, key_size, set->table_count);

	set_entry_t* curr = set->table[hash];
	while (curr)
	{
		if (memcmp(curr->key, key, MIN(curr->key_size, key_size)) == 0)
		{
			key_exists = 1;

			break;
		}

		curr = curr->next;
	}

	if (key_exists == 0)
	{
		curr = (set_entry_t*)heap_alloc(sizeof(set_entry_t));
		memset(curr, 0, sizeof(set_entry_t));

		curr->next = set->table[hash];
		curr->key = (uint8_t*)heap_alloc(key_size);

		curr->key_size = key_size;
		memcpy(curr->key, key, key_size);

		set->table[hash] = curr;
		set->entry_count++;
	}

	TRACY_ZONE_END

	return key_exists;
}
uint8_t std_set_remove(set_t* set, void const* key, uint64_t key_size)
{
	TRACY_ZONE_BEGIN

	uint8_t removed = 0;

	uint64_t hash = std_set_hash(set, key, key_size, set->table_count);

	set_entry_t* curr = set->table[hash];
	set_entry_t* prev = 0;
	while (curr)
	{
		if (memcmp(curr->key, key, MIN(curr->key_size, key_size)) == 0)
		{
			if (prev)
			{
				prev->next = curr->next;
			}
			else
			{
				set->table[hash] = curr->next;
			}

			heap_free(curr->key);
			heap_free(curr);

			set->entry_count--;

			removed = 1;

			break;
		}

		prev = curr;
		curr = curr->next;
	}

	TRACY_ZONE_END

	return removed;
}
uint8_t std_set_contains(set_t* set, void const* key, uint64_t key_size)
{
	TRACY_ZONE_BEGIN

	uint8_t contains = 0;

	uint64_t hash = std_set_hash(set, key, key_size, set->table_count);

	set_entry_t* curr = set->table[hash];
	while (curr)
	{
		if (memcmp(curr->key, key, MIN(curr->key_size, key_size)) == 0)
		{
			contains = 1;

			break;
		}

		curr = curr->next;
	}

	TRACY_ZONE_END

	return contains;
}
uint64_t std_set_count(set_t* set)
{
	TRACY_ZONE_BEGIN

	uint64_t entry_count = set->entry_count;

	TRACY_ZONE_END

	return entry_count;
}
void std_set_clear(set_t* set)
{
	TRACY_ZONE_BEGIN

	uint64_t table_index = 0;
	while (table_index < set->table_count)
	{
		set_entry_t* curr = set->table[table_index];
		while (curr)
		{
			set_entry_t* tmp = curr;

			curr = curr->next;

			heap_free(tmp->key);
			heap_free(tmp);
		}

		table_index++;
	}

	memset(set->table, 0, set->table_size);

	set->entry_count = 0;

	TRACY_ZONE_END
}
void std_set_free(set_t* set)
{
	TRACY_ZONE_BEGIN

	uint64_t table_index = 0;
	while (table_index < set->table_count)
	{
		set_entry_t* curr = set->table[table_index];
		while (curr)
		{
			set_entry_t* tmp = curr;

			curr = curr->next;

			heap_free(tmp->key);
			heap_free(tmp);
		}

		table_index++;
	}

	heap_free(set->table);

	memset(set, 0, sizeof(set_t));

	TRACY_ZONE_END
}
void std_set_expand(set_t* set)
{
	TRACY_ZONE_BEGIN

	uint64_t table_index = 0;

	uint64_t table_size = set->table_size * 2;
	uint64_t table_count = set->table_count * 2;

	set_entry_t** table = (set_entry_t**)heap_alloc(table_size);
	memset(table, 0, table_size);

	while (table_index < set->table_count)
	{
		set_entry_t* curr = set->table[table_index];
		while (curr)
		{
			uint64_t hash = std_set_hash(set, curr->key, curr->key_size, table_count);

			curr->next = table[hash];
			table[hash] = curr;
			curr = curr->next;
		}

		table_index++;
	}

	heap_free(set->table);

	set->table = table;
	set->table_size = table_size;
	set->table_count = table_count;

	TRACY_ZONE_END
}
uint64_t std_set_hash(set_t* set, void const* key, uint64_t key_size, uint64_t modulus)
{
	TRACY_ZONE_BEGIN

	uint64_t hash = SET_HASH_VALUE;

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
uint8_t std_set_load_factor(set_t* set)
{
	TRACY_ZONE_BEGIN

	uint8_t load_factor = (uint8_t)((((float)set->entry_count + 1.0F) / set->table_count) * 100);

	TRACY_ZONE_END

	return load_factor;
}
