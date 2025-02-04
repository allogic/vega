#ifndef VEGA_INSTANCE_COMPONENT_SCENE_CONTROLLER_H
#define VEGA_INSTANCE_COMPONENT_SCENE_CONTROLLER_H

#include <stdint.h>

#include <vega/core/core.h>

#include <vega/engine/component/transform.h>

VEGA_EXTERN_C_BEGIN

typedef struct _scene_controller_t
{
	vector3_t velocity; // TODO: remove this..
	vector3_t mouse_position_begin;
	vector3_t mouse_position_end;
	vector3_t mouse_position_delta;
	double keyboard_move_speed_fast;
	double keyboard_move_speed_normal;
	double mouse_move_speed_fast;
	double mouse_move_speed_normal;
	double mouse_rotation_speed_fast;
	double mouse_rotation_speed_normal;
	double move_drag; // TODO: remove this..
	double rotation_drag; // TODO: remove this..
} scene_controller_t;

///////////////////////////////////////////////////////////////
// Public API
///////////////////////////////////////////////////////////////

void scene_controller_handle_position(scene_controller_t* scene_controller, transform_t* transform);
void scene_controller_handle_rotation(scene_controller_t* scene_controller, transform_t* transform);

VEGA_EXTERN_C_END

#endif // VEGA_INSTANCE_COMPONENT_SCENE_CONTROLLER_H
