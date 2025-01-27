#ifndef VEGA_ENGINE_COMPONENT_TRANSFORM_H
#define VEGA_ENGINE_COMPONENT_TRANSFORM_H

#include <stdint.h>

#include <vega/core/core.h>

typedef struct _transform_t
{
	struct _transform_t* parent;
	vector3_t local_right;
	vector3_t local_up;
	vector3_t local_front;
	vector3_t local_left;
	vector3_t local_down;
	vector3_t local_back;
	vector3_t local_position;
	quaternion_t local_rotation;
	vector3_t local_scale;
	vector3_t world_position;
	quaternion_t world_rotation;
	vector3_t world_scale;
} transform_t;

///////////////////////////////////////////////////////////////
// Public API
///////////////////////////////////////////////////////////////

void transform_init(transform_t* transform);

__forceinline vector3_t transform_get_world_position(transform_t* transform);
__forceinline quaternion_t transform_get_world_rotation(transform_t* transform);
__forceinline vector3_t transform_get_world_euler_angles(transform_t* transform);
__forceinline vector3_t transform_get_world_scale(transform_t* transform);

__forceinline vector3_t transform_get_local_position(transform_t* transform);
__forceinline quaternion_t transform_get_local_rotation(transform_t* transform);
__forceinline vector3_t transform_get_local_euler_angles(transform_t* transform);
__forceinline vector3_t transform_get_local_scale(transform_t* transform);

void transform_set_position(transform_t* transform, vector3_t position);
void transform_set_position_xyz(transform_t* transform, double x, double y, double z);
void transform_set_relative_position(transform_t* transform, vector3_t position);
void transform_set_relative_position_xyz(transform_t* transform, double x, double y, double z);
void transform_set_rotation(transform_t* transform, quaternion_t rotation);
void transform_set_rotation_xyzw(transform_t* transform, double x, double y, double z, double w);
void transform_set_relative_rotation(transform_t* transform, quaternion_t rotation);
void transform_set_relative_rotation_xyzw(transform_t* transform, double x, double y, double z, double w);
void transform_set_euler_angles(transform_t* transform, vector3_t rotation);
void transform_set_euler_angles_pyr(transform_t* transform, double p, double y, double r);
void transform_set_relative_euler_angles(transform_t* transform, vector3_t rotation);
void transform_set_relative_euler_angles_pyr(transform_t* transform, double p, double y, double r);
void transform_set_scale(transform_t* transform, vector3_t scale);
void transform_set_scale_xyz(transform_t* transform, double x, double y, double z);
void transform_set_relative_scale(transform_t* transform, vector3_t scale);
void transform_set_relative_scale_xyz(transform_t* transform, double x, double y, double z);

matrix4_t transform_matrix(transform_t* transform);

///////////////////////////////////////////////////////////////
// Internal API
///////////////////////////////////////////////////////////////

__forceinline void transform_compute_world_position(transform_t* transform);
__forceinline void transform_compute_world_rotation(transform_t* transform);
__forceinline void transform_compute_world_scale(transform_t* transform);

///////////////////////////////////////////////////////////////
// Inline Definition
///////////////////////////////////////////////////////////////

__forceinline vector3_t transform_get_world_position(transform_t* transform)
{
	return transform->world_position;
}
__forceinline quaternion_t transform_get_world_rotation(transform_t* transform)
{
	return transform->world_rotation;
}
__forceinline vector3_t transform_get_world_euler_angles(transform_t* transform)
{
	return math_quaternion_to_euler_angles(transform->world_rotation);
}
__forceinline vector3_t transform_get_world_scale(transform_t* transform)
{
	return transform->world_scale;
}
__forceinline vector3_t transform_get_local_position(transform_t* transform)
{
	return transform->local_position;
}
__forceinline quaternion_t transform_get_local_rotation(transform_t* transform)
{
	return transform->local_rotation;
}
__forceinline vector3_t transform_get_local_euler_angles(transform_t* transform)
{
	return math_quaternion_to_euler_angles(transform->local_rotation);
}
__forceinline vector3_t transform_get_local_scale(transform_t* transform)
{
	return transform->local_scale;
}
__forceinline void transform_compute_world_position(transform_t* transform)
{
	if (transform->parent)
	{
		transform->world_position = math_vector3_add(transform->world_position, math_vector3_rotate(transform->local_position, transform->parent->world_rotation));
	}
	else
	{
		transform->world_position = transform->local_position;
	}
}
__forceinline void transform_compute_world_rotation(transform_t* transform)
{
	if (transform->parent)
	{
		transform->world_rotation = math_quaternion_mul(transform->local_rotation, transform->parent->world_rotation);
	}
	else
	{
		transform->world_rotation = transform->local_rotation;
	}
}
__forceinline void transform_compute_world_scale(transform_t* transform)
{
	if (transform->parent)
	{
		transform->world_scale = math_vector3_mul(transform->local_scale, transform->parent->world_scale);
	}
	else
	{
		transform->world_scale = transform->local_scale;
	}
}

#endif // VEGA_ENGINE_COMPONENT_TRANSFORM_H
