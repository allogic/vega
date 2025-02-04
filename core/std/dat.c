#include <string.h>

#include <vega/core/heap.h>
#include <vega/core/macros.h>

#include <vega/core/debug/tracy.h>

#include <vega/core/std/dat.h>

#ifndef TRACY_ZONE_BEGIN
	#define TRACY_ZONE_BEGIN TracyCZoneC(ctx, TRACY_COLOR_RED, 1U);
#endif // TRACY_ZONE_BEGIN

#ifndef TRACY_ZONE_END
	#define TRACY_ZONE_END TracyCZoneEnd(ctx);
#endif // TRACY_ZONE_END

#ifndef DAT_SPARSE_COUNT
	#define DAT_SPARSE_COUNT (16ULL)
#endif // DAT_SPARSE_COUNT

#ifndef DAT_SPARSE_LIMIT
	#define DAT_SPARSE_LIMIT (16ULL)
#endif // DAT_SPARSE_LIMIT

#ifndef DAT_TOMBSTONE
	#define DAT_TOMBSTONE (0xFFFFFFFFFFFFFFFFULL)
#endif // DAT_TOMBSTONE

dat_t std_dat_alloc(uint64_t value_size)
{
	TRACY_ZONE_BEGIN

	dat_t dat = { 0 };
	dat.pages = 0;
	dat.page_size = 0;
	dat.page_count = 0;
	dat.values = std_vector_alloc(value_size);
	dat.ids = std_fvector64_alloc();

	TRACY_ZONE_END

	return dat;
}
void std_dat_set(dat_t* dat, uint64_t id, void const* value)
{
	TRACY_ZONE_BEGIN

	uint64_t dense_index = std_dat_get_dense_index(dat, id);
	if (dense_index == DAT_TOMBSTONE)
	{
		uint64_t value_count = std_vector_count(&dat->values);

		std_dat_set_dense_index(dat, id, value_count);

		std_vector_push(&dat->values, value);
		std_fvector64_push(&dat->ids, id);
	}
	else
	{
		std_vector_set(&dat->values, dense_index, value);
		std_fvector64_set(&dat->ids, dense_index, id);
	}

	TRACY_ZONE_END
}
void* std_dat_get(dat_t* dat, uint64_t id)
{
	TRACY_ZONE_BEGIN

	void* value = 0;

	uint64_t dense_index = std_dat_get_dense_index(dat, id);
	if (dense_index != DAT_TOMBSTONE)
	{
		value = std_vector_get(&dat->values, dense_index);
	}

	TRACY_ZONE_END

	return value;
}
void std_dat_remove(dat_t* dat, uint64_t id)
{
	TRACY_ZONE_BEGIN

	uint64_t dense_index = std_dat_get_dense_index(dat, id);
	if (dense_index != DAT_TOMBSTONE)
	{
		uint64_t value_count = std_vector_count(&dat->values);
		uint64_t id_count = std_fvector64_count(&dat->ids);

		std_dat_set_dense_index(dat, id_count - 1, dense_index);
		std_dat_set_dense_index(dat, id, DAT_TOMBSTONE);

		std_vector_swap(&dat->values, dense_index, value_count - 1);
		std_fvector64_swap(&dat->ids, dense_index, id_count - 1);

		std_vector_pop(&dat->values, 0);
		std_fvector64_pop(&dat->ids, 0);
	}

	TRACY_ZONE_END
}
uint8_t std_dat_contains(dat_t* dat, uint64_t id)
{
	TRACY_ZONE_BEGIN

	uint8_t contains = std_dat_get_dense_index(dat, id) != DAT_TOMBSTONE;

	TRACY_ZONE_END

	return contains;
}
uint8_t std_dat_empty(dat_t* dat)
{
	TRACY_ZONE_BEGIN

	uint8_t empty = std_fvector64_empty(&dat->ids);

	TRACY_ZONE_END

	return empty;
}
void std_dat_clear(dat_t* dat)
{
	TRACY_ZONE_BEGIN

	uint64_t page_index = 0;
	while (page_index < dat->page_count)
	{
		page_t* page = &dat->pages[page_index];

		heap_free(page->sparse);

		page_index++;
	}

	if (dat->pages)
	{
		heap_free(dat->pages);

		dat->pages = 0;
		dat->page_size = 0;
		dat->page_count = 0;
	}

	std_vector_clear(&dat->values);
	std_fvector64_clear(&dat->ids);

	TRACY_ZONE_END
}
uint64_t std_dat_count(dat_t* dat)
{
	TRACY_ZONE_BEGIN

	uint64_t count = std_fvector64_count(&dat->ids);

	TRACY_ZONE_END

	return count;
}
uint64_t std_dat_id(dat_t* dat, uint64_t index)
{
	TRACY_ZONE_BEGIN

	uint64_t id = std_fvector64_get(&dat->ids, index);

	TRACY_ZONE_END

	return id;
}
void std_dat_free(dat_t* dat)
{
	TRACY_ZONE_BEGIN

	uint64_t page_index = 0;
	while (page_index < dat->page_count)
	{
		page_t* page = &dat->pages[page_index];

		heap_free(page->sparse);

		page_index++;
	}

	if (dat->pages)
	{
		heap_free(dat->pages);
	}

	std_vector_free(&dat->values);
	std_fvector64_free(&dat->ids);

	memset(dat, 0, sizeof(dat_t));

	TRACY_ZONE_END
}
void std_dat_set_dense_index(dat_t* dat, uint64_t id, uint64_t index)
{
	TRACY_ZONE_BEGIN

	uint64_t page_index = id / DAT_SPARSE_LIMIT;
	uint64_t sparse_index = id % DAT_SPARSE_LIMIT;

	std_dat_pages_resize(dat, page_index + 1);

	page_t* page = &dat->pages[page_index];

	std_dat_sparse_resize(dat, page, sparse_index + 1);

	page->sparse[sparse_index] = index;

	TRACY_ZONE_END
}
uint64_t std_dat_get_dense_index(dat_t* dat, uint64_t id)
{
	TRACY_ZONE_BEGIN

	uint64_t index = 0;

	uint64_t page_index = id / DAT_SPARSE_LIMIT;
	uint64_t sparse_index = id % DAT_SPARSE_LIMIT;

	if (page_index < dat->page_count)
	{
		page_t* page = &dat->pages[page_index];

		if (sparse_index < page->sparse_count)
		{
			index = page->sparse[sparse_index];
		}
		else
		{
			index = DAT_TOMBSTONE;
		}
	}
	else
	{
		index = DAT_TOMBSTONE;
	}

	TRACY_ZONE_END

	return index;
}
void std_dat_pages_resize(dat_t* dat, uint64_t page_count)
{
	TRACY_ZONE_BEGIN

	if (page_count > dat->page_count)
	{
		dat->pages = (page_t*)heap_realloc(dat->pages, page_count * sizeof(page_t));

		uint64_t page_index = dat->page_count;
		while (page_index < page_count)
		{
			page_t* page = &dat->pages[page_index];

			page->sparse = (uint64_t*)heap_alloc(DAT_SPARSE_COUNT * sizeof(uint64_t));
			page->sparse_size = DAT_SPARSE_COUNT * sizeof(uint64_t);
			page->sparse_count = DAT_SPARSE_COUNT;

			uint64_t sparse_index = 0;
			while (sparse_index < page->sparse_count)
			{
				page->sparse[sparse_index] = DAT_TOMBSTONE;

				sparse_index++;
			}

			page_index++;
		}

		dat->page_size = page_count * sizeof(page_t);
		dat->page_count = page_count;
	}

	TRACY_ZONE_END
}
void std_dat_sparse_resize(dat_t* dat, page_t* page, uint64_t sparse_count)
{
	TRACY_ZONE_BEGIN

	if (sparse_count > page->sparse_count)
	{
		page->sparse = (uint64_t*)heap_realloc(page->sparse, sparse_count * sizeof(uint64_t));

		uint64_t sparse_index = page->sparse_count;
		while (sparse_index < sparse_count)
		{
			page->sparse[sparse_index] = DAT_TOMBSTONE;

			sparse_index++;
		}

		page->sparse_size = sparse_count * sizeof(uint64_t);
		page->sparse_count = sparse_count;
	}

	TRACY_ZONE_END
}
