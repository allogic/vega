#include <string.h>

#include <vega/core/macros.h>

#include <vega/core/debug/tracy.h>

#include <vega/core/std/ecs.h>

#ifndef TRACY_ZONE_BEGIN
	#define TRACY_ZONE_BEGIN TracyCZoneC(ctx, TRACY_COLOR_RED, 1U);
#endif // TRACY_ZONE_BEGIN

#ifndef TRACY_ZONE_END
	#define TRACY_ZONE_END TracyCZoneEnd(ctx);
#endif // TRACY_ZONE_END

#ifndef ECS_INVALID_ENTITY
	#define ECS_INVALID_ENTITY (0xFFFFFFFFFFFFFFFFULL)
#endif // ECS_INVALID_ENTITY

#ifndef ECS_MAX_POOL_COUNT
	#define ECS_MAX_POOL_COUNT (0xFFFFFFFFFFFFFFFFULL)
#endif // ECS_MAX_POOL_COUNT

ecs_t std_ecs_alloc(void)
{
	TRACY_ZONE_BEGIN

	ecs_t ecs = { 0 };
	ecs.masks = std_fdat64_alloc();
	ecs.alloc_entities = std_fvector64_alloc();
	ecs.free_entities = std_fvector64_alloc();

	memset(ecs.pools, 0, sizeof(dat_t) * 64);

	TRACY_ZONE_END

	return ecs;
}
uint64_t std_ecs_create(ecs_t* ecs)
{
	TRACY_ZONE_BEGIN

	uint64_t entity = ECS_INVALID_ENTITY;
	uint64_t free_entity_count = std_fvector64_count(&ecs->free_entities);

	if (free_entity_count)
	{
		std_fvector64_pop(&ecs->free_entities, &entity);
	}
	else
	{
		entity = ecs->unique_entity_index++;
	}

	std_fvector64_push(&ecs->alloc_entities, entity);

	std_fdat64_set(&ecs->masks, entity, 0);

	TRACY_ZONE_END

	return entity;
}
void std_ecs_delete(ecs_t* ecs, uint64_t entity)
{
	TRACY_ZONE_BEGIN

	uint64_t mask = std_fdat64_get(&ecs->masks, entity);

	uint64_t pool_index = 0;
	while (pool_index < 64)
	{
		if (VEGA_IS_BIT_SET(mask, pool_index))
		{
			std_dat_remove(&ecs->pools[pool_index], entity);
		}

		pool_index++;
	}

	std_fdat64_remove(&ecs->masks, entity);

	std_fvector64_remove(&ecs->alloc_entities, entity);
	std_fvector64_push(&ecs->free_entities, entity);

	TRACY_ZONE_END
}
void std_ecs_register(ecs_t* ecs, uint64_t component, uint64_t value_size)
{
	TRACY_ZONE_BEGIN

	if (VEGA_IS_BIT_SET(ecs->active_pools, component))
	{
		std_dat_free(&ecs->pools[component]);

		ecs->active_pool_count--;
	}

	ecs->active_pools = VEGA_SET_BIT(ecs->active_pools, component);
	ecs->active_pool_count++;

	ecs->pools[component] = std_dat_alloc(value_size);

	TRACY_ZONE_END
}
void std_ecs_unregister(ecs_t* ecs, uint64_t component)
{
	TRACY_ZONE_BEGIN

	// TODO

	TRACY_ZONE_END
}
void* std_ecs_attach(ecs_t* ecs, uint64_t entity, uint64_t component, void const* value)
{
	TRACY_ZONE_BEGIN

	std_dat_set(&ecs->pools[component], entity, value);

	uint64_t mask = std_fdat64_get(&ecs->masks, entity);
	mask = VEGA_SET_BIT(mask, component);
	std_fdat64_set(&ecs->masks, entity, mask);

	void* result = std_dat_get(&ecs->pools[component], entity);

	TRACY_ZONE_END

	return result;
}
void std_ecs_detach(ecs_t* ecs, uint64_t entity, uint64_t component)
{
	TRACY_ZONE_BEGIN

	std_dat_remove(&ecs->pools[component], entity);

	uint64_t mask = std_fdat64_get(&ecs->masks, entity);
	mask = VEGA_CLEAR_BIT(mask, component);
	std_fdat64_set(&ecs->masks, entity, mask);

	TRACY_ZONE_END
}
void std_ecs_set(ecs_t* ecs, uint64_t entity, uint64_t component, void const* value)
{
	TRACY_ZONE_BEGIN

	std_dat_set(&ecs->pools[component], entity, value);

	TRACY_ZONE_END
}
void* std_ecs_get(ecs_t* ecs, uint64_t entity, uint64_t component)
{
	TRACY_ZONE_BEGIN

	void* result = std_dat_get(&ecs->pools[component], entity);

	TRACY_ZONE_END

	return result;
}
void std_ecs_query(ecs_t* ecs, ecs_query_t* query)
{
	TRACY_ZONE_BEGIN

	// TODO: only execute this function if dirty..

	dat_t* smallest_pool = 0;

	uint64_t smallest_pool_count = ECS_MAX_POOL_COUNT;

	uint64_t pool_index = 0;
	while (pool_index < 64)
	{
		if (VEGA_IS_BIT_SET(query->mask, pool_index))
		{
			uint64_t pool_count = std_dat_count(&ecs->pools[pool_index]);

			if (pool_count < smallest_pool_count)
			{
				smallest_pool = &ecs->pools[pool_index];
				smallest_pool_count = pool_count;
			}
		}

		pool_index++;
	}

	query->pool = smallest_pool;
	query->count = smallest_pool_count;

	TRACY_ZONE_END
}
uint8_t std_ecs_contains(ecs_t* ecs, uint64_t entity)
{
	TRACY_ZONE_BEGIN

	uint8_t contains = 1;

	uint64_t pool_index = 0;
	while (pool_index < 64)
	{
		if (std_dat_contains(&ecs->pools[pool_index], entity) == 0)
		{
			contains = 0;

			break;
		}

		pool_index++;
	}

	TRACY_ZONE_END

	return contains;
}
uint64_t std_ecs_count(ecs_t* ecs)
{
	TRACY_ZONE_BEGIN

	uint64_t count = std_fvector64_count(&ecs->alloc_entities);

	TRACY_ZONE_END

	return count;
}
fvector64_t* std_ecs_entities(ecs_t* ecs)
{
	TRACY_ZONE_BEGIN

	fvector64_t* entities = &ecs->alloc_entities;

	TRACY_ZONE_END

	return entities;
}
void std_ecs_for(ecs_t* ecs, ecs_query_t* query, ecs_for_proc_t for_proc)
{
	TRACY_ZONE_BEGIN

	uint64_t entity_index = 0;
	uint64_t entity_count = query->count;
	while (entity_index < entity_count)
	{
		uint64_t entity = std_dat_id(query->pool, entity_index);

		for_proc(ecs, entity_index, entity);

		entity_index++;
	}

	TRACY_ZONE_END
}
void std_ecs_clear(ecs_t* ecs)
{
	TRACY_ZONE_BEGIN

	uint64_t pool_index = 0;
	while (pool_index < 64)
	{
		if (VEGA_IS_BIT_SET(ecs->active_pools, pool_index))
		{
			std_dat_clear(&ecs->pools[pool_index]);
		}

		pool_index++;
	}

	ecs->unique_entity_index = 0;

	std_fdat64_clear(&ecs->masks);

	std_fvector64_clear(&ecs->alloc_entities);
	std_fvector64_clear(&ecs->free_entities);

	TRACY_ZONE_END
}
void std_ecs_free(ecs_t* ecs)
{
	TRACY_ZONE_BEGIN

	uint64_t pool_index = 0;
	while (pool_index < 64)
	{
		if (VEGA_IS_BIT_SET(ecs->active_pools, pool_index))
		{
			std_dat_free(&ecs->pools[pool_index]);
		}

		pool_index++;
	}

	std_fdat64_free(&ecs->masks);

	std_fvector64_free(&ecs->alloc_entities);
	std_fvector64_free(&ecs->free_entities);

	memset(ecs, 0, sizeof(ecs_t));

	TRACY_ZONE_END
}
