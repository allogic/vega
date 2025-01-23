#include <string.h>

#include <vega/core/macros.h>

#include <vega/core/debug/tracy.h>

#include <vega/core/std/ecs.h>

#ifndef TRACY_ZONE_BEGIN
	#define TRACY_ZONE_BEGIN TracyCZoneC(ctx, TRACY_COLOR_RED, 1U)
#endif // TRACY_ZONE_BEGIN

#ifndef TRACY_ZONE_END
	#define TRACY_ZONE_END TracyCZoneEnd(ctx)
#endif // TRACY_ZONE_END

#ifndef ECS_INVALID_ENTITY
	#define ECS_INVALID_ENTITY (0xFFFFFFFFFFFFFFFFULL)
#endif // ECS_INVALID_ENTITY

#ifndef ECS_MAX_POOL_COUNT
	#define ECS_MAX_POOL_COUNT (0xFFFFFFFFFFFFFFFFULL)
#endif // ECS_MAX_POOL_COUNT

#ifndef ECS_MAX_POOLS
	#define ECS_MAX_POOLS (64ULL)
#endif // ECS_MAX_POOLS

ecs_t std_ecs_alloc(void)
{
	TRACY_ZONE_BEGIN

	ecs_t ecs;
	memset(&ecs, 0, sizeof(ecs_t));

	ecs.masks = std_fdat64_alloc();
	ecs.alloc_entities = std_fvector64_alloc();
	ecs.free_entities = std_fvector64_alloc();
	ecs.pools = std_vector_alloc(sizeof(dat_t));

	std_vector_resize(&ecs.pools, ECS_MAX_POOLS);

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
		entity = ecs->unique_entity++;
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

	uint64_t bit = 0;
	while (bit < ECS_MAX_POOLS)
	{
		if (IS_BIT_SET(mask, bit))
		{
			dat_t* pool = (dat_t*)std_vector_at(&ecs->pools, bit);

			std_dat_remove(pool, entity);
		}

		bit++;
	}

	std_fdat64_remove(&ecs->masks, entity);

	std_fvector64_remove(&ecs->alloc_entities, entity);
	std_fvector64_push(&ecs->free_entities, entity);

	TRACY_ZONE_END
}
void std_ecs_register(ecs_t* ecs, uint64_t bit, uint64_t value_size)
{
	TRACY_ZONE_BEGIN

	dat_t* pool = (dat_t*)std_vector_at(&ecs->pools, bit);

	if (IS_BIT_SET(ecs->active_pools, bit))
	{
		std_dat_free(pool);

		ecs->active_pool_count--;
	}

	ecs->active_pools = SET_BIT(ecs->active_pools, bit);
	ecs->active_pool_count++;

	*pool = std_dat_alloc(value_size);

	TRACY_ZONE_END
}
void std_ecs_unregister(ecs_t* ecs, uint64_t bit)
{
	TRACY_ZONE_BEGIN

	// TODO

	TRACY_ZONE_END
}
void std_ecs_attach(ecs_t* ecs, uint64_t entity, uint64_t bit, void const* value)
{
	TRACY_ZONE_BEGIN

	dat_t* pool = (dat_t*)std_vector_at(&ecs->pools, bit);
	std_dat_set(pool, entity, value);

	uint64_t mask = std_fdat64_get(&ecs->masks, entity);
	mask = SET_BIT(mask, bit);
	std_fdat64_set(&ecs->masks, entity, mask);

	TRACY_ZONE_END
}
void std_ecs_detach(ecs_t* ecs, uint64_t entity, uint64_t bit)
{
	TRACY_ZONE_BEGIN

	dat_t* pool = (dat_t*)std_vector_at(&ecs->pools, bit);
	std_dat_remove(pool, entity);

	uint64_t mask = std_fdat64_get(&ecs->masks, entity);
	mask = CLEAR_BIT(mask, bit);
	std_fdat64_set(&ecs->masks, entity, mask);

	TRACY_ZONE_END
}
void std_ecs_set(ecs_t* ecs, uint64_t entity, uint64_t bit, void const* value)
{
	TRACY_ZONE_BEGIN

	dat_t* pool = (dat_t*)std_vector_at(&ecs->pools, bit);
	std_dat_set(pool, entity, value);

	TRACY_ZONE_END
}
void* std_ecs_get(ecs_t* ecs, uint64_t entity, uint64_t bit)
{
	TRACY_ZONE_BEGIN

	dat_t* pool = (dat_t*)std_vector_at(&ecs->pools, bit);
	void* component = std_dat_get(pool, entity);

	TRACY_ZONE_END

	return component;
}
vector_t std_ecs_all(ecs_t* ecs, uint64_t mask)
{
	TRACY_ZONE_BEGIN

	vector_t view = std_vector_alloc(sizeof(dat_t*));

	uint64_t bit = 0;
	while (bit < ECS_MAX_POOLS)
	{
		dat_t* pool = (dat_t*)std_vector_at(&ecs->pools, bit);

		if (IS_BIT_SET(mask, bit))
		{
			std_vector_push(&view, &pool);
		}

		bit++;
	}

	TRACY_ZONE_END

	return view;
}
uint8_t std_ecs_contains(ecs_t* ecs, uint64_t entity, vector_t* view)
{
	TRACY_ZONE_BEGIN

	uint8_t contains = 1;

	uint64_t view_index = 0;
	uint64_t view_count = std_vector_count(view);
	while (view_index < view_count)
	{
		dat_t* pool = *(dat_t**)std_vector_at(view, view_index);;

		if (std_dat_contains(pool, entity) == 0)
		{
			contains = 0;

			break;
		}

		view_index++;
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
void* std_ecs_value(ecs_t* ecs, uint64_t entity, uint64_t bit, vector_t* view)
{
	TRACY_ZONE_BEGIN

	dat_t* pool = *(dat_t**)std_vector_at(view, bit);
	void* value = std_dat_value(pool, entity);

	TRACY_ZONE_END

	return value;
}
void std_ecs_for(ecs_t* ecs, vector_t* view, ecs_for_func_t for_func)
{
	TRACY_ZONE_BEGIN

	dat_t* smallest_pool = std_ecs_smallest(ecs, view);

	uint64_t entity_index = 0;
	uint64_t entity_count = std_dat_count(smallest_pool);
	while (entity_index < entity_count)
	{
		uint64_t entity = std_dat_id(smallest_pool, entity_index);

		for_func(ecs, entity, view);

		entity_index++;
	}

	TRACY_ZONE_END
}
void std_ecs_clear(ecs_t* ecs)
{
	TRACY_ZONE_BEGIN

	uint64_t bit = 0;
	while (bit < ECS_MAX_POOLS)
	{
		if (IS_BIT_SET(ecs->active_pools, bit))
		{
			dat_t* pool = (dat_t*)std_vector_at(&ecs->pools, bit);

			std_dat_clear(pool);
		}

		bit++;
	}

	ecs->unique_entity = 0;

	std_fdat64_clear(&ecs->masks);

	std_fvector64_clear(&ecs->alloc_entities);
	std_fvector64_clear(&ecs->free_entities);

	TRACY_ZONE_END
}
void std_ecs_free(ecs_t* ecs)
{
	TRACY_ZONE_BEGIN

	uint64_t bit = 0;
	while (bit < ECS_MAX_POOLS)
	{
		if (IS_BIT_SET(ecs->active_pools, bit))
		{
			dat_t* pool = (dat_t*)std_vector_at(&ecs->pools, bit);

			std_dat_free(pool);
		}

		bit++;
	}

	std_fdat64_free(&ecs->masks);

	std_fvector64_free(&ecs->alloc_entities);
	std_fvector64_free(&ecs->free_entities);
	std_vector_free(&ecs->pools);

	memset(ecs, 0, sizeof(ecs_t));

	TRACY_ZONE_END
}
dat_t* std_ecs_smallest(ecs_t* ecs, vector_t* view)
{
	TRACY_ZONE_BEGIN

	dat_t* smallest_pool = 0;

	uint64_t smallest_pool_count = ECS_MAX_POOL_COUNT;

	uint64_t view_index = 0;
	uint64_t view_count = std_vector_count(view);
	while (view_index < view_count)
	{
		dat_t* pool = *(dat_t**)std_vector_at(view, view_index);

		uint64_t pool_count = std_dat_count(pool);
		if (pool_count < smallest_pool_count)
		{
			smallest_pool = pool;
			smallest_pool_count = pool_count;
		}

		view_index++;
	}

	TRACY_ZONE_END

	return smallest_pool;
}
