#include <stdlib.h>

#include <vega/core/heap.h>

#include <vega/core/debug/tracy.h>

#ifndef TRACY_ZONE_BEGIN
	#define TRACY_ZONE_BEGIN TracyCZoneC(ctx, TRACY_COLOR_RED, 1U);
#endif // TRACY_ZONE_BEGIN

#ifndef TRACY_ZONE_END
	#define TRACY_ZONE_END TracyCZoneEnd(ctx);
#endif // TRACY_ZONE_END

void* heap_alloc(uint64_t size)
{
	TRACY_ZONE_BEGIN

	void* block = malloc(size);

	TracyCAlloc(block, size);

	TRACY_ZONE_END

	return block;
}
void* heap_realloc(void* block, uint64_t size)
{
	TRACY_ZONE_BEGIN

	if (block)
	{
		TracyCFree(block);
	}

	block = realloc(block, size);

	TracyCAlloc(block, size);

	TRACY_ZONE_END

	return block;
}
void heap_free(void* block)
{
	TRACY_ZONE_BEGIN

	TracyCFree(block);

	free(block);

	TRACY_ZONE_END
}
