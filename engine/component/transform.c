#include <string.h>

#include <vega/engine/component/transform.h>

#ifndef TRACY_ZONE_BEGIN
	#define TRACY_ZONE_BEGIN TracyCZoneC(ctx, TRACY_COLOR_GREEN, 1U);
#endif // TRACY_ZONE_BEGIN

#ifndef TRACY_ZONE_END
	#define TRACY_ZONE_END TracyCZoneEnd(ctx);
#endif // TRACY_ZONE_END

void transform_init(transform_t* transform, transform_t* parent)
{
	TRACY_ZONE_BEGIN

	transform->parent = parent;
	transform->local_right = g_world_right;
	transform->local_up = g_world_up;
	transform->local_front = g_world_front;
	transform->local_left = g_world_left;
	transform->local_down = g_world_down;
	transform->local_back = g_world_back;
	transform->local_position = math_vector3_zero();
	transform->local_rotation = math_quaternion_identity();
	transform->local_scale = math_vector3_one();
	transform->world_position = math_vector3_zero();
	transform->world_rotation = math_quaternion_identity();
	transform->world_scale = math_vector3_one();
	transform->children = std_fvector64_alloc(); // TODO: free this..

	if (parent)
	{
		std_fvector64_push(&parent->children, (uint64_t)transform);
	}

	TRACY_ZONE_END
}
void transform_set_position(transform_t* transform, vector3_t position)
{
	TRACY_ZONE_BEGIN

	transform->local_position = position;

	transform_compute_world_position(transform);

	TRACY_ZONE_END
}
void transform_set_position_xyz(transform_t* transform, double x, double y, double z)
{
	TRACY_ZONE_BEGIN

	transform->local_position.x = x;
	transform->local_position.y = y;
	transform->local_position.z = z;

	transform_compute_world_position(transform);

	TRACY_ZONE_END
}
void transform_set_relative_position(transform_t* transform, vector3_t position)
{
	TRACY_ZONE_BEGIN

	transform->local_position.x += position.x;
	transform->local_position.y += position.y;
	transform->local_position.z += position.z;

	transform_compute_world_position(transform);

	TRACY_ZONE_END
}
void transform_set_relative_position_xyz(transform_t* transform, double x, double y, double z)
{
	TRACY_ZONE_BEGIN

	transform->local_position.x += x;
	transform->local_position.y += y;
	transform->local_position.z += z;

	transform_compute_world_position(transform);

	TRACY_ZONE_END
}
void transform_set_rotation(transform_t* transform, quaternion_t rotation)
{
	TRACY_ZONE_BEGIN

	transform->local_rotation = rotation;
	transform->local_rotation = math_quaternion_norm(transform->local_rotation);

	transform->local_right = math_quaternion_right(transform->local_rotation);
	transform->local_up = math_quaternion_up(transform->local_rotation);
	transform->local_front = math_quaternion_front(transform->local_rotation);

	transform->local_left = math_vector3_invert(transform->local_right);
	transform->local_down = math_vector3_invert(transform->local_up);
	transform->local_back = math_vector3_invert(transform->local_front);

	transform_compute_world_rotation(transform);
	transform_compute_world_position(transform);

	TRACY_ZONE_END
}
void transform_set_rotation_xyzw(transform_t* transform, double x, double y, double z, double w)
{
	TRACY_ZONE_BEGIN

	transform->local_rotation = math_quaternion_xyzw(x, y, z, w);
	transform->local_rotation = math_quaternion_norm(transform->local_rotation);

	transform->local_right = math_quaternion_right(transform->local_rotation);
	transform->local_up = math_quaternion_up(transform->local_rotation);
	transform->local_front = math_quaternion_front(transform->local_rotation);

	transform->local_left = math_vector3_invert(transform->local_right);
	transform->local_down = math_vector3_invert(transform->local_up);
	transform->local_back = math_vector3_invert(transform->local_front);

	transform_compute_world_rotation(transform);
	transform_compute_world_position(transform);

	TRACY_ZONE_END
}
void transform_set_relative_rotation(transform_t* transform, quaternion_t rotation)
{
	TRACY_ZONE_BEGIN

	transform->local_rotation = math_quaternion_mul(transform->local_rotation, rotation);
	transform->local_rotation = math_quaternion_norm(transform->local_rotation);

	transform->local_right = math_quaternion_right(transform->local_rotation);
	transform->local_up = math_quaternion_up(transform->local_rotation);
	transform->local_front = math_quaternion_front(transform->local_rotation);

	transform->local_left = math_vector3_invert(transform->local_right);
	transform->local_down = math_vector3_invert(transform->local_up);
	transform->local_back = math_vector3_invert(transform->local_front);

	transform_compute_world_rotation(transform);
	transform_compute_world_position(transform);

	TRACY_ZONE_END
}
void transform_set_relative_rotation_xyzw(transform_t* transform, double x, double y, double z, double w)
{
	TRACY_ZONE_BEGIN

	transform->local_rotation = math_quaternion_mul(transform->local_rotation, math_quaternion_xyzw(x, y, z, w));
	transform->local_rotation = math_quaternion_norm(transform->local_rotation);

	transform->local_right = math_quaternion_right(transform->local_rotation);
	transform->local_up = math_quaternion_up(transform->local_rotation);
	transform->local_front = math_quaternion_front(transform->local_rotation);

	transform->local_left = math_vector3_invert(transform->local_right);
	transform->local_down = math_vector3_invert(transform->local_up);
	transform->local_back = math_vector3_invert(transform->local_front);

	transform_compute_world_rotation(transform);
	transform_compute_world_position(transform);

	TRACY_ZONE_END
}
void transform_set_euler_angles(transform_t* transform, vector3_t rotation)
{
	TRACY_ZONE_BEGIN

	double p = math_deg_to_rad(rotation.x);
	double y = math_deg_to_rad(rotation.y);
	double r = math_deg_to_rad(rotation.z);

	if (transform->parent)
	{
		quaternion_t qx = math_quaternion_angle_axis(p, transform->parent->local_right);
		quaternion_t qy = math_quaternion_angle_axis(y, transform->parent->local_up);
		quaternion_t qz = math_quaternion_angle_axis(r, transform->parent->local_front);

		transform->local_rotation = math_quaternion_mul(qy, math_quaternion_mul(qx, qz));
		transform->local_rotation = math_quaternion_norm(transform->local_rotation);

		transform->local_right = math_quaternion_right(transform->local_rotation);
		transform->local_up = math_quaternion_up(transform->local_rotation);
		transform->local_front = math_quaternion_front(transform->local_rotation);

		transform->local_left = math_vector3_invert(transform->local_right);
		transform->local_down = math_vector3_invert(transform->local_up);
		transform->local_back = math_vector3_invert(transform->local_front);
	}
	else
	{
		quaternion_t qx = math_quaternion_angle_axis(p, g_world_right);
		quaternion_t qy = math_quaternion_angle_axis(y, g_world_up);
		quaternion_t qz = math_quaternion_angle_axis(r, g_world_front);

		transform->local_rotation = math_quaternion_mul(qy, math_quaternion_mul(qx, qz));
		transform->local_rotation = math_quaternion_norm(transform->local_rotation);

		transform->local_right = math_quaternion_right(transform->local_rotation);
		transform->local_up = math_quaternion_up(transform->local_rotation);
		transform->local_front = math_quaternion_front(transform->local_rotation);

		transform->local_left = math_vector3_invert(transform->local_right);
		transform->local_down = math_vector3_invert(transform->local_up);
		transform->local_back = math_vector3_invert(transform->local_front);
	}

	transform_compute_world_rotation(transform);
	transform_compute_world_position(transform);

	TRACY_ZONE_END
}
void transform_set_euler_angles_pyr(transform_t* transform, double p, double y, double r)
{
	TRACY_ZONE_BEGIN

	p = math_deg_to_rad(p);
	y = math_deg_to_rad(y);
	r = math_deg_to_rad(r);

	if (transform->parent)
	{
		quaternion_t qx = math_quaternion_angle_axis(p, transform->parent->local_right);
		quaternion_t qy = math_quaternion_angle_axis(y, transform->parent->local_up);
		quaternion_t qz = math_quaternion_angle_axis(r, transform->parent->local_front);

		transform->local_rotation = math_quaternion_mul(qy, math_quaternion_mul(qx, qz));
		transform->local_rotation = math_quaternion_norm(transform->local_rotation);

		transform->local_right = math_quaternion_right(transform->local_rotation);
		transform->local_up = math_quaternion_up(transform->local_rotation);
		transform->local_front = math_quaternion_front(transform->local_rotation);

		transform->local_left = math_vector3_invert(transform->local_right);
		transform->local_down = math_vector3_invert(transform->local_up);
		transform->local_back = math_vector3_invert(transform->local_front);
	}
	else
	{
		quaternion_t qx = math_quaternion_angle_axis(p, g_world_right);
		quaternion_t qy = math_quaternion_angle_axis(y, g_world_up);
		quaternion_t qz = math_quaternion_angle_axis(r, g_world_front);

		transform->local_rotation = math_quaternion_mul(qy, math_quaternion_mul(qx, qz));
		transform->local_rotation = math_quaternion_norm(transform->local_rotation);

		transform->local_right = math_quaternion_right(transform->local_rotation);
		transform->local_up = math_quaternion_up(transform->local_rotation);
		transform->local_front = math_quaternion_front(transform->local_rotation);

		transform->local_left = math_vector3_invert(transform->local_right);
		transform->local_down = math_vector3_invert(transform->local_up);
		transform->local_back = math_vector3_invert(transform->local_front);
	}

	transform_compute_world_rotation(transform);
	transform_compute_world_position(transform);

	TRACY_ZONE_END
}
void transform_set_relative_euler_angles(transform_t* transform, vector3_t rotation)
{
	TRACY_ZONE_BEGIN

	double p = math_deg_to_rad(rotation.x);
	double y = math_deg_to_rad(rotation.y);
	double r = math_deg_to_rad(rotation.z);

	if (transform->parent)
	{
		quaternion_t qx = math_quaternion_angle_axis(p, transform->parent->local_right);
		quaternion_t qy = math_quaternion_angle_axis(y, transform->parent->local_up);
		quaternion_t qz = math_quaternion_angle_axis(r, transform->parent->local_front);

		transform->local_rotation = math_quaternion_mul(transform->local_rotation, math_quaternion_mul(qy, math_quaternion_mul(qx, qz)));
		transform->local_rotation = math_quaternion_norm(transform->local_rotation);

		transform->local_right = math_quaternion_right(transform->local_rotation);
		transform->local_up = math_quaternion_up(transform->local_rotation);
		transform->local_front = math_quaternion_front(transform->local_rotation);

		transform->local_left = math_vector3_invert(transform->local_right);
		transform->local_down = math_vector3_invert(transform->local_up);
		transform->local_back = math_vector3_invert(transform->local_front);
	}
	else
	{
		quaternion_t qx = math_quaternion_angle_axis(p, g_world_right);
		quaternion_t qy = math_quaternion_angle_axis(y, g_world_up);
		quaternion_t qz = math_quaternion_angle_axis(r, g_world_front);

		transform->local_rotation = math_quaternion_mul(transform->local_rotation, math_quaternion_mul(qy, math_quaternion_mul(qx, qz)));
		transform->local_rotation = math_quaternion_norm(transform->local_rotation);

		transform->local_right = math_quaternion_right(transform->local_rotation);
		transform->local_up = math_quaternion_up(transform->local_rotation);
		transform->local_front = math_quaternion_front(transform->local_rotation);

		transform->local_left = math_vector3_invert(transform->local_right);
		transform->local_down = math_vector3_invert(transform->local_up);
		transform->local_back = math_vector3_invert(transform->local_front);
	}

	transform_compute_world_rotation(transform);
	transform_compute_world_position(transform);

	TRACY_ZONE_END
}
void transform_set_relative_euler_angles_pyr(transform_t* transform, double p, double y, double r)
{
	TRACY_ZONE_BEGIN

	p = math_deg_to_rad(p);
	y = math_deg_to_rad(y);
	r = math_deg_to_rad(r);

	if (transform->parent)
	{
		quaternion_t qx = math_quaternion_angle_axis(p, transform->parent->local_right);
		quaternion_t qy = math_quaternion_angle_axis(y, transform->parent->local_up);
		quaternion_t qz = math_quaternion_angle_axis(r, transform->parent->local_front);

		transform->local_rotation = math_quaternion_mul(transform->local_rotation, math_quaternion_mul(qy, math_quaternion_mul(qx, qz)));
		transform->local_rotation = math_quaternion_norm(transform->local_rotation);

		transform->local_right = math_quaternion_right(transform->local_rotation);
		transform->local_up = math_quaternion_up(transform->local_rotation);
		transform->local_front = math_quaternion_front(transform->local_rotation);

		transform->local_left = math_vector3_invert(transform->local_right);
		transform->local_down = math_vector3_invert(transform->local_up);
		transform->local_back = math_vector3_invert(transform->local_front);
	}
	else
	{
		quaternion_t qx = math_quaternion_angle_axis(p, g_world_right);
		quaternion_t qy = math_quaternion_angle_axis(y, g_world_up);
		quaternion_t qz = math_quaternion_angle_axis(r, g_world_front);

		transform->local_rotation = math_quaternion_mul(transform->local_rotation, math_quaternion_mul(qy, math_quaternion_mul(qx, qz)));
		transform->local_rotation = math_quaternion_norm(transform->local_rotation);

		transform->local_right = math_quaternion_right(transform->local_rotation);
		transform->local_up = math_quaternion_up(transform->local_rotation);
		transform->local_front = math_quaternion_front(transform->local_rotation);

		transform->local_left = math_vector3_invert(transform->local_right);
		transform->local_down = math_vector3_invert(transform->local_up);
		transform->local_back = math_vector3_invert(transform->local_front);
	}

	transform_compute_world_rotation(transform);
	transform_compute_world_position(transform);

	TRACY_ZONE_END
}
void transform_set_scale(transform_t* transform, vector3_t scale)
{
	TRACY_ZONE_BEGIN

	transform->local_scale = scale;

	transform_compute_world_scale(transform);

	TRACY_ZONE_END
}
void transform_set_scale_xyz(transform_t* transform, double x, double y, double z)
{
	TRACY_ZONE_BEGIN

	transform->local_scale.x = x;
	transform->local_scale.y = y;
	transform->local_scale.z = z;

	transform_compute_world_scale(transform);

	TRACY_ZONE_END
}
void transform_set_relative_scale(transform_t* transform, vector3_t scale)
{
	TRACY_ZONE_BEGIN

	transform->local_scale.x += scale.x;
	transform->local_scale.y += scale.y;
	transform->local_scale.z += scale.z;

	transform_compute_world_scale(transform);

	TRACY_ZONE_END
}
void transform_set_relative_scale_xyz(transform_t* transform, double x, double y, double z)
{
	TRACY_ZONE_BEGIN

	transform->local_scale.x += x;
	transform->local_scale.y += y;
	transform->local_scale.z += z;

	transform_compute_world_scale(transform);

	TRACY_ZONE_END
}
matrix4_t transform_matrix(transform_t* transform)
{
	TRACY_ZONE_BEGIN

	double xx = transform->world_rotation.x * transform->world_rotation.x;
	double yy = transform->world_rotation.y * transform->world_rotation.y;
	double zz = transform->world_rotation.z * transform->world_rotation.z;

	double xy = transform->world_rotation.x * transform->world_rotation.y;
	double xz = transform->world_rotation.x * transform->world_rotation.z;
	double yz = transform->world_rotation.y * transform->world_rotation.z;

	double wx = transform->world_rotation.w * transform->world_rotation.x;
	double wy = transform->world_rotation.w * transform->world_rotation.y;
	double wz = transform->world_rotation.w * transform->world_rotation.z;

	matrix4_t r =
	{
		.m00 = (1.0 - 2.0 * (yy + zz)) * transform->world_scale.x, .m01 =        2.0 * (xy - wz)  * transform->world_scale.y, .m02 =        2.0 * (xz + wy)  * transform->world_scale.z, .m03 = 0.0,
		.m10 =        2.0 * (xy + wz)  * transform->world_scale.x, .m11 = (1.0 - 2.0 * (xx + zz)) * transform->world_scale.y, .m12 =        2.0 * (yz - wx)  * transform->world_scale.z, .m13 = 0.0,
		.m20 =        2.0 * (xz - wy)  * transform->world_scale.x, .m21 =        2.0 * (yz + wx)  * transform->world_scale.y, .m22 = (1.0 - 2.0 * (xx + yy)) * transform->world_scale.z, .m23 = 0.0,
		.m30 =                        transform->world_position.x, .m31 =                        transform->world_position.y, .m32 =                        transform->world_position.z, .m33 = 1.0,
	};

	TRACY_ZONE_END

	return r;
}
