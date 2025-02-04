#ifndef VEGA_CORE_STD_FDAT64_H
#define VEGA_CORE_STD_FDAT64_H

#include <stdint.h>

#include <vega/core/macros.h>

#include <vega/core/std/vector.h>
#include <vega/core/std/fvector64.h>

VEGA_EXTERN_C_BEGIN

typedef struct _fpage64_t
{
	uint64_t* sparse;
	uint64_t sparse_size;
	uint64_t sparse_count;
} fpage64_t;

typedef struct _fdat64_t
{
	fpage64_t* pages;
	uint64_t page_size;
	uint64_t page_count;
	fvector64_t values;
	fvector64_t ids;
} fdat64_t;

///////////////////////////////////////////////////////////////
// Public API
///////////////////////////////////////////////////////////////

fdat64_t std_fdat64_alloc(void);
void std_fdat64_set(fdat64_t* dat, uint64_t id, uint64_t value);
uint64_t std_fdat64_get(fdat64_t* dat, uint64_t id);
void std_fdat64_remove(fdat64_t* dat, uint64_t id);
uint8_t std_fdat64_contains(fdat64_t* dat, uint64_t id);
uint8_t std_fdat64_empty(fdat64_t* dat);
void std_fdat64_clear(fdat64_t* dat);
uint64_t std_fdat64_count(fdat64_t* dat);
uint64_t std_fdat64_id(fdat64_t* dat, uint64_t index);
uint64_t std_fdat64_value(fdat64_t* dat, uint64_t index);
void std_fdat64_free(fdat64_t* dat);

///////////////////////////////////////////////////////////////
// Internal API
///////////////////////////////////////////////////////////////

void std_fdat64_set_dense_index(fdat64_t* dat, uint64_t id, uint64_t index);
uint64_t std_fdat64_get_dense_index(fdat64_t* dat, uint64_t id);
void std_fdat64_pages_resize(fdat64_t* dat, uint64_t page_count);
void std_fdat64_sparse_resize(fdat64_t* dat, fpage64_t* page, uint64_t sparse_count);

VEGA_EXTERN_C_END

#endif // VEGA_CORE_STD_FDAT64_H
