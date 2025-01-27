#include <assert.h>

#include <vega/engine/engine.h>

#ifndef TRACY_ZONE_BEGIN
	#define TRACY_ZONE_BEGIN TracyCZoneC(ctx, TRACY_COLOR_GREEN, 1U);
#endif // TRACY_ZONE_BEGIN

#ifndef TRACY_ZONE_END
	#define TRACY_ZONE_END TracyCZoneEnd(ctx);
#endif // TRACY_ZONE_END

void engine_alloc(void)
{
	TRACY_ZONE_BEGIN

	asset_loader_alloc();
	scene_stack_alloc();

	TRACY_ZONE_END
}
void engine_free(void)
{
	TRACY_ZONE_BEGIN

	scene_stack_free();
	asset_loader_free();

	assert(g_heap_allocated_bytes == 0);

	TRACY_ZONE_END
}
