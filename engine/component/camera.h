#ifndef VEGA_ENGINE_COMPONENT_CAMERA_H
#define VEGA_ENGINE_COMPONENT_CAMERA_H

#include <stdint.h>

#include <vega/core/core.h>

VEGA_EXTERN_C_BEGIN

typedef enum _camera_mode_t
{
	CAMERA_MODE_ORTHO,
	CAMERA_MODE_PERSP,
} camera_mode_t;

typedef struct _camera_t
{
	camera_mode_t mode;
	struct
	{
		double left;
		double right;
		double top;
		double bottom;
	};
	struct
	{
		double fov;
	};
	double near_z;
	double far_z;
} camera_t;

///////////////////////////////////////////////////////////////
// Public API
///////////////////////////////////////////////////////////////

void camera_init(camera_t* camera);

VEGA_EXTERN_C_END

#endif // VEGA_ENGINE_COMPONENT_CAMERA_H
