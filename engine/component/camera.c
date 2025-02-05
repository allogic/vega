#include <vega/engine/component/camera.h>

#ifndef TRACY_ZONE_BEGIN
	#define TRACY_ZONE_BEGIN TracyCZoneC(ctx, TRACY_COLOR_GREEN, 1U);
#endif // TRACY_ZONE_BEGIN

#ifndef TRACY_ZONE_END
	#define TRACY_ZONE_END TracyCZoneEnd(ctx);
#endif // TRACY_ZONE_END

void camera_init(camera_t* camera)
{
	TRACY_ZONE_BEGIN

	camera->mode = CAMERA_MODE_PERSP;
	camera->fov = math_deg_to_rad(45.0);
	camera->near_z = 1.0E-3;
	camera->far_z = 1.0E+4;

	TRACY_ZONE_END
}
