#ifndef VEGA_CORE_STD_ECS_H
#define VEGA_CORE_STD_ECS_H

#include <stdint.h>

#include <vega/core/macros.h>

#include <vega/core/std/dat.h>
#include <vega/core/std/fdat64.h>
#include <vega/core/std/vector.h>
#include <vega/core/std/fvector64.h>

VEGA_EXTERN_C_BEGIN

typedef struct _ecs_t
{
	uint64_t unique_entity_index;
	uint64_t active_pools;
	uint64_t active_pool_count;
	fdat64_t masks;
	fvector64_t alloc_entities;
	fvector64_t free_entities;
	dat_t pools[64];
} ecs_t;

typedef struct _ecs_query_t
{
	uint64_t mask;
	uint64_t count;
	dat_t* pool;
} ecs_query_t;

typedef void (*ecs_for_proc_t)(ecs_t* ecs, uint64_t index, uint64_t entity);

///////////////////////////////////////////////////////////////
// Public API
///////////////////////////////////////////////////////////////

ecs_t std_ecs_alloc(void);
uint64_t std_ecs_create(ecs_t* ecs);
void std_ecs_delete(ecs_t* ecs, uint64_t entity);
void std_ecs_register(ecs_t* ecs, uint64_t component, uint64_t value_size);
void std_ecs_unregister(ecs_t* ecs, uint64_t component);
void* std_ecs_attach(ecs_t* ecs, uint64_t entity, uint64_t component, void const* value);
void std_ecs_detach(ecs_t* ecs, uint64_t entity, uint64_t component);
void std_ecs_set(ecs_t* ecs, uint64_t entity, uint64_t component, void const* value);
void* std_ecs_get(ecs_t* ecs, uint64_t entity, uint64_t component);
void std_ecs_query(ecs_t* ecs, ecs_query_t* query);
uint8_t std_ecs_contains(ecs_t* ecs, uint64_t entity);
uint64_t std_ecs_count(ecs_t* ecs);
fvector64_t* std_ecs_entities(ecs_t* ecs);
void std_ecs_for(ecs_t* ecs, ecs_query_t* query, ecs_for_proc_t for_proc);
void std_ecs_clear(ecs_t* ecs);
void std_ecs_free(ecs_t* ecs);

VEGA_EXTERN_C_END

#endif // VEGA_CORE_STD_ECS_H
