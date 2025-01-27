#include <stdlib.h>

#include <vega/core/heap.h>

#include <vega/core/debug/tracy.h>

#ifndef TRACY_MEMORY_ALLOC
	#define TRACY_MEMORY_ALLOC(BLOCK, SIZE) TracyCAlloc(BLOCK, SIZE);
#endif // TRACY_MEMORY_ALLOC

#ifndef TRACY_MEMORY_FREE
	#define TRACY_MEMORY_FREE(BLOCK) TracyCFree(BLOCK);
#endif // TRACY_MEMORY_FREE

#ifndef TRACY_ZONE_BEGIN
	#define TRACY_ZONE_BEGIN TracyCZoneC(ctx, TRACY_COLOR_RED, 1U);
#endif // TRACY_ZONE_BEGIN

#ifndef TRACY_ZONE_END
	#define TRACY_ZONE_END TracyCZoneEnd(ctx);
#endif // TRACY_ZONE_END

#ifdef VEGA_DEBUG
uint64_t g_heap_allocated_bytes = 0;
#endif // VEGA_DEBUG

void* heap_alloc(uint64_t size)
{
	TRACY_ZONE_BEGIN

	void* block = 0;

#ifdef VEGA_DEBUG
	uint64_t new_size = sizeof(uint64_t) + size;

	uint64_t* new_block = (long long unsigned*)malloc(new_size);

	TRACY_MEMORY_ALLOC(new_block, new_size);

	g_heap_allocated_bytes += new_size;

	*new_block = new_size;
	new_block++;

	block = new_block;
#else
	block = malloc(size);

	TRACY_MEMORY_ALLOC(block, size);
#endif // VEGA_DEBUG

	TRACY_ZONE_END

	return block;
}
void* heap_realloc(void* block, uint64_t size)
{
	TRACY_ZONE_BEGIN

#ifdef VEGA_DEBUG
	if (block)
	{
		uint64_t* old_block = (uint64_t*)block;

		old_block--;

		uint64_t old_size = *old_block;
		uint64_t new_size = sizeof(uint64_t) + size;

		g_heap_allocated_bytes -= old_size;

		TRACY_MEMORY_FREE(old_block);

		uint64_t* new_block = (uint64_t*)realloc(old_block, new_size);

		TRACY_MEMORY_ALLOC(new_block, new_size);

		g_heap_allocated_bytes += new_size;

		*new_block = new_size;
		new_block++;

		block = new_block;
	}
	else
	{
		block = heap_alloc(size);
	}
#else
	TRACY_MEMORY_FREE(block);

	block = realloc(block, size);

	TRACY_MEMORY_ALLOC(block, size);
#endif // VEGA_DEBUG

	TRACY_ZONE_END

	return block;
}
void heap_free(void* block)
{
	TRACY_ZONE_BEGIN

#ifdef VEGA_DEBUG
	uint64_t* old_block = (uint64_t*)block;

	old_block--;

	uint64_t old_size = *old_block;

	g_heap_allocated_bytes -= old_size;

	TRACY_MEMORY_FREE(old_block);

	free(old_block);
#else
	TRACY_MEMORY_FREE(block);

	free(block);
#endif // VEGA_DEBUG

	TRACY_ZONE_END
}
