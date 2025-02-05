#include <vega/engine/component/renderable.h>

#ifndef TRACY_ZONE_BEGIN
	#define TRACY_ZONE_BEGIN TracyCZoneC(ctx, TRACY_COLOR_GREEN, 1U);
#endif // TRACY_ZONE_BEGIN

#ifndef TRACY_ZONE_END
	#define TRACY_ZONE_END TracyCZoneEnd(ctx);
#endif // TRACY_ZONE_END

void renderable_init(renderable_t* renderable)
{
	TRACY_ZONE_BEGIN

	renderable->mesh_asset = 0;
	renderable->material_asset = 0;

	TRACY_ZONE_END
}
