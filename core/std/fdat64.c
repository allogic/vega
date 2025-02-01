#include <string.h>

#include <vega/core/heap.h>
#include <vega/core/macros.h>

#include <vega/core/debug/tracy.h>

#include <vega/core/std/fdat64.h>

#ifndef TRACY_ZONE_BEGIN
	#define TRACY_ZONE_BEGIN TracyCZoneC(ctx, TRACY_COLOR_RED, 1U);
#endif // TRACY_ZONE_BEGIN

#ifndef TRACY_ZONE_END
	#define TRACY_ZONE_END TracyCZoneEnd(ctx);
#endif // TRACY_ZONE_END

#ifndef FDAT64_SPARSE_COUNT
	#define FDAT64_SPARSE_COUNT (16ULL)
#endif // FDAT64_SPARSE_COUNT

#ifndef FDAT64_SPARSE_LIMIT
	#define FDAT64_SPARSE_LIMIT (16ULL)
#endif // FDAT64_SPARSE_LIMIT

#ifndef FDAT64_TOMBSTONE
	#define FDAT64_TOMBSTONE (0xFFFFFFFFFFFFFFFFULL)
#endif // FDAT64_TOMBSTONE

fdat64_t std_fdat64_alloc(void)
{
	TRACY_ZONE_BEGIN

	fdat64_t dat = { 0 };
	dat.pages = 0;
	dat.page_size = 0;
	dat.page_count = 0;
	dat.values = std_fvector64_alloc();
	dat.ids = std_fvector64_alloc();

	TRACY_ZONE_END

	return dat;
}
void std_fdat64_set(fdat64_t* dat, uint64_t id, uint64_t value)
{
	TRACY_ZONE_BEGIN

	uint64_t dense_index = std_fdat64_get_dense_index(dat, id);
	if (dense_index == FDAT64_TOMBSTONE)
	{
		uint64_t value_count = std_fvector64_count(&dat->values);

		std_fdat64_set_dense_index(dat, id, value_count);

		std_fvector64_push(&dat->values, value);
		std_fvector64_push(&dat->ids, id);
	}
	else
	{
		std_fvector64_set(&dat->values, dense_index, value);
		std_fvector64_set(&dat->ids, dense_index, id);
	}

	TRACY_ZONE_END
}
uint64_t std_fdat64_get(fdat64_t* dat, uint64_t id)
{
	TRACY_ZONE_BEGIN

	uint64_t value = 0;

	uint64_t dense_index = std_fdat64_get_dense_index(dat, id);
	if (dense_index != FDAT64_TOMBSTONE)
	{
		value = std_fvector64_get(&dat->values, dense_index);
	}

	TRACY_ZONE_END

	return value;
}
void std_fdat64_remove(fdat64_t* dat, uint64_t id)
{
	TRACY_ZONE_BEGIN

	uint64_t dense_index = std_fdat64_get_dense_index(dat, id);
	if (dense_index != FDAT64_TOMBSTONE)
	{
		uint64_t value_count = std_fvector64_count(&dat->values);
		uint64_t id_count = std_fvector64_count(&dat->ids);

		std_fdat64_set_dense_index(dat, id_count - 1, dense_index);
		std_fdat64_set_dense_index(dat, id, FDAT64_TOMBSTONE);

		std_fvector64_swap(&dat->values, dense_index, value_count - 1);
		std_fvector64_swap(&dat->ids, dense_index, id_count - 1);

		std_fvector64_pop(&dat->values, 0);
		std_fvector64_pop(&dat->ids, 0);
	}

	TRACY_ZONE_END
}
uint8_t std_fdat64_contains(fdat64_t* dat, uint64_t id)
{
	TRACY_ZONE_BEGIN

	uint8_t contains = std_fdat64_get_dense_index(dat, id) != FDAT64_TOMBSTONE;

	TRACY_ZONE_END

	return contains;
}
uint8_t std_fdat64_empty(fdat64_t* dat)
{
	TRACY_ZONE_BEGIN

	uint8_t empty = std_fvector64_empty(&dat->values);

	TRACY_ZONE_END

	return empty;
}
void std_fdat64_clear(fdat64_t* dat)
{
	TRACY_ZONE_BEGIN

	uint64_t page_index = 0;
	while (page_index < dat->page_count)
	{
		fpage64_t* page = &dat->pages[page_index];

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

	std_fvector64_clear(&dat->values);
	std_fvector64_clear(&dat->ids);

	TRACY_ZONE_END
}
uint64_t std_fdat64_count(fdat64_t* dat)
{
	TRACY_ZONE_BEGIN

	uint64_t count = std_fvector64_count(&dat->ids);

	TRACY_ZONE_END

	return count;
}
uint64_t std_fdat64_id(fdat64_t* dat, uint64_t index)
{
	TRACY_ZONE_BEGIN

	uint64_t id = std_fvector64_get(&dat->ids, index);

	TRACY_ZONE_END

	return id;
}
uint64_t std_fdat64_value(fdat64_t* dat, uint64_t index)
{
	TRACY_ZONE_BEGIN

	uint64_t value = std_fvector64_get(&dat->values, index);

	TRACY_ZONE_END

	return value;
}
void std_fdat64_free(fdat64_t* dat)
{
	TRACY_ZONE_BEGIN

	uint64_t page_index = 0;
	while (page_index < dat->page_count)
	{
		fpage64_t* page = &dat->pages[page_index];

		heap_free(page->sparse);

		page_index++;
	}

	if (dat->pages)
	{
		heap_free(dat->pages);
	}

	std_fvector64_free(&dat->values);
	std_fvector64_free(&dat->ids);

	memset(dat, 0, sizeof(fdat64_t));

	TRACY_ZONE_END
}
void std_fdat64_set_dense_index(fdat64_t* dat, uint64_t id, uint64_t index)
{
	TRACY_ZONE_BEGIN

	uint64_t page_index = id / FDAT64_SPARSE_LIMIT;
	uint64_t sparse_index = id % FDAT64_SPARSE_LIMIT;

	std_fdat64_pages_resize(dat, page_index + 1);

	fpage64_t* page = &dat->pages[page_index];

	std_fdat64_sparse_resize(dat, page, sparse_index + 1);

	page->sparse[sparse_index] = index;

	TRACY_ZONE_END
}
uint64_t std_fdat64_get_dense_index(fdat64_t* dat, uint64_t id)
{
	TRACY_ZONE_BEGIN

	uint64_t index = 0;

	uint64_t page_index = id / FDAT64_SPARSE_LIMIT;
	uint64_t sparse_index = id % FDAT64_SPARSE_LIMIT;

	if (page_index < dat->page_count)
	{
		fpage64_t* page = &dat->pages[page_index];

		if (sparse_index < page->sparse_count)
		{
			index = page->sparse[sparse_index];
		}
		else
		{
			index = FDAT64_TOMBSTONE;
		}
	}
	else
	{
		index = FDAT64_TOMBSTONE;
	}

	TRACY_ZONE_END

	return index;
}
void std_fdat64_pages_resize(fdat64_t* dat, uint64_t page_count)
{
	TRACY_ZONE_BEGIN

	if (page_count > dat->page_count)
	{
		dat->pages = (fpage64_t*)heap_realloc(dat->pages, page_count * sizeof(fpage64_t));

		uint64_t page_index = dat->page_count;
		while (page_index < page_count)
		{
			fpage64_t* page = &dat->pages[page_index];

			page->sparse = (uint64_t*)heap_alloc(FDAT64_SPARSE_COUNT * sizeof(uint64_t));
			page->sparse_size = FDAT64_SPARSE_COUNT * sizeof(uint64_t);
			page->sparse_count = FDAT64_SPARSE_COUNT;

			uint64_t sparse_index = 0;
			while (sparse_index < page->sparse_count)
			{
				page->sparse[sparse_index] = FDAT64_TOMBSTONE;

				sparse_index++;
			}

			page_index++;
		}

		dat->page_size = page_count * sizeof(fpage64_t);
		dat->page_count = page_count;
	}

	TRACY_ZONE_END
}
void std_fdat64_sparse_resize(fdat64_t* dat, fpage64_t* page, uint64_t sparse_count)
{
	TRACY_ZONE_BEGIN

	if (sparse_count > page->sparse_count)
	{
		page->sparse = (uint64_t*)heap_realloc(page->sparse, sparse_count * sizeof(uint64_t));

		uint64_t sparse_index = page->sparse_count;
		while (sparse_index < sparse_count)
		{
			page->sparse[sparse_index] = FDAT64_TOMBSTONE;

			sparse_index++;
		}

		page->sparse_size = sparse_count * sizeof(uint64_t);
		page->sparse_count = sparse_count;
	}

	TRACY_ZONE_END
}
