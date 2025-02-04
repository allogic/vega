#ifndef VEGA_CORE_STD_DAT_H
#define VEGA_CORE_STD_DAT_H

#include <stdint.h>

#include <vega/core/macros.h>

#include <vega/core/std/vector.h>
#include <vega/core/std/fvector64.h>

VEGA_EXTERN_C_BEGIN

typedef struct _page_t
{
	uint64_t* sparse;
	uint64_t sparse_size;
	uint64_t sparse_count;
} page_t;

typedef struct _dat_t
{
	page_t* pages;
	uint64_t page_size;
	uint64_t page_count;
	vector_t values;
	fvector64_t ids;
} dat_t;

///////////////////////////////////////////////////////////////
// Public API
///////////////////////////////////////////////////////////////

dat_t std_dat_alloc(uint64_t value_size);
void std_dat_set(dat_t* dat, uint64_t id, void const* value);
void* std_dat_get(dat_t* dat, uint64_t id);
void std_dat_remove(dat_t* dat, uint64_t id);
uint8_t std_dat_contains(dat_t* dat, uint64_t id);
uint8_t std_dat_empty(dat_t* dat);
void std_dat_clear(dat_t* dat);
uint64_t std_dat_count(dat_t* dat);
uint64_t std_dat_id(dat_t* dat, uint64_t index);
void std_dat_free(dat_t* dat);

///////////////////////////////////////////////////////////////
// Internal API
///////////////////////////////////////////////////////////////

void std_dat_set_dense_index(dat_t* dat, uint64_t id, uint64_t index);
uint64_t std_dat_get_dense_index(dat_t* dat, uint64_t id);
void std_dat_pages_resize(dat_t* dat, uint64_t page_count);
void std_dat_sparse_resize(dat_t* dat, page_t* page, uint64_t sparse_count);

VEGA_EXTERN_C_END

#endif // VEGA_CORE_STD_DAT_H
