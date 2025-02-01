#include <string.h>

#include <vega/engine/component/transform.h>

#ifndef TRACY_ZONE_BEGIN
	#define TRACY_ZONE_BEGIN TracyCZoneC(ctx, TRACY_COLOR_GREEN, 1U);
#endif // TRACY_ZONE_BEGIN

#ifndef TRACY_ZONE_END
	#define TRACY_ZONE_END TracyCZoneEnd(ctx);
#endif // TRACY_ZONE_END

transform_t transform_identity(void)
{
	TRACY_ZONE_BEGIN

	transform_t transform = { 0 };
	transform.parent = 0;
	transform.local_right = g_world_right;
	transform.local_up = g_world_up;
	transform.local_front = g_world_front;
	transform.local_left = g_world_left;
	transform.local_down = g_world_down;
	transform.local_back = g_world_back;
	transform.local_position = math_vector3_zero();
	transform.local_rotation = math_quaternion_identity();
	transform.local_scale = math_vector3_one();
	transform.world_position = math_vector3_zero();
	transform.world_rotation = math_quaternion_identity();
	transform.world_scale = math_vector3_one();

	TRACY_ZONE_END

	return transform;
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

	// TODO

	TRACY_ZONE_END
}
void transform_set_rotation_xyzw(transform_t* transform, double x, double y, double z, double w)
{
	TRACY_ZONE_BEGIN

	// TODO

	TRACY_ZONE_END
}
void transform_set_relative_rotation(transform_t* transform, quaternion_t rotation)
{
	TRACY_ZONE_BEGIN

	// TODO

	TRACY_ZONE_END
}
void transform_set_relative_rotation_xyzw(transform_t* transform, double x, double y, double z, double w)
{
	TRACY_ZONE_BEGIN

	// TODO

	TRACY_ZONE_END
}
void transform_set_euler_angles(transform_t* transform, vector3_t rotation)
{
	TRACY_ZONE_BEGIN

	double pitch = math_deg_to_rad(rotation.x);
	double yaw = math_deg_to_rad(rotation.y);
	double roll = math_deg_to_rad(rotation.z);

	if (transform->parent)
	{
		quaternion_t qx = math_quaternion_angle_axis(pitch, transform->parent->local_right);
		quaternion_t qy = math_quaternion_angle_axis(yaw, transform->parent->local_up);
		quaternion_t qz = math_quaternion_angle_axis(roll, transform->parent->local_front);

		quaternion_t qxz = math_quaternion_mul(qx, qz);
		quaternion_t qyxz = math_quaternion_mul(qy, qxz);

		transform->local_rotation = math_quaternion_norm(qyxz);

		vector3_t lr = math_vector3_rotate(transform->parent->local_right, transform->local_rotation);
		vector3_t lu = math_vector3_rotate(transform->parent->local_up, transform->local_rotation);
		vector3_t lf = math_vector3_rotate(transform->parent->local_front, transform->local_rotation);

		transform->local_right = math_vector3_norm(lr);
		transform->local_up = math_vector3_norm(lu);
		transform->local_front = math_vector3_norm(lf);

		transform->local_left = math_vector3_from_xyz(-transform->local_right.x, -transform->local_right.y, -transform->local_right.z);
		transform->local_down = math_vector3_from_xyz(-transform->local_up.x, -transform->local_up.y, -transform->local_up.z);
		transform->local_back = math_vector3_from_xyz(-transform->local_front.x, -transform->local_front.y, -transform->local_front.z);
	}
	else
	{
		quaternion_t qx = math_quaternion_angle_axis(pitch, g_world_right);
		quaternion_t qy = math_quaternion_angle_axis(yaw, g_world_up);
		quaternion_t qz = math_quaternion_angle_axis(roll, g_world_front);

		quaternion_t qxz = math_quaternion_mul(qx, qz);
		quaternion_t qyxz = math_quaternion_mul(qy, qxz);

		transform->local_rotation = math_quaternion_norm(qyxz);

		vector3_t lr = math_vector3_rotate(g_world_right, transform->local_rotation);
		vector3_t lu = math_vector3_rotate(g_world_up, transform->local_rotation);
		vector3_t lf = math_vector3_rotate(g_world_front, transform->local_rotation);

		transform->local_right = math_vector3_norm(lr);
		transform->local_up = math_vector3_norm(lu);
		transform->local_front = math_vector3_norm(lf);

		transform->local_left = math_vector3_from_xyz(-transform->local_right.x, -transform->local_right.y, -transform->local_right.z);
		transform->local_down = math_vector3_from_xyz(-transform->local_up.x, -transform->local_up.y, -transform->local_up.z);
		transform->local_back = math_vector3_from_xyz(-transform->local_front.x, -transform->local_front.y, -transform->local_front.z);
	}

	transform_compute_world_rotation(transform);
	transform_compute_world_position(transform);

	TRACY_ZONE_END
}
void transform_set_euler_angles_pyr(transform_t* transform, double p, double y, double r)
{
	TRACY_ZONE_BEGIN

	double pitch = math_deg_to_rad(p);
	double yaw = math_deg_to_rad(y);
	double roll = math_deg_to_rad(r);

	if (transform->parent)
	{
		quaternion_t qx = math_quaternion_angle_axis(pitch, transform->parent->local_right);
		quaternion_t qy = math_quaternion_angle_axis(yaw, transform->parent->local_up);
		quaternion_t qz = math_quaternion_angle_axis(roll, transform->parent->local_front);

		quaternion_t qxz = math_quaternion_mul(qx, qz);
		quaternion_t qyxz = math_quaternion_mul(qy, qxz);

		transform->local_rotation = math_quaternion_norm(qyxz);

		vector3_t lr = math_vector3_rotate(transform->parent->local_right, transform->local_rotation);
		vector3_t lu = math_vector3_rotate(transform->parent->local_up, transform->local_rotation);
		vector3_t lf = math_vector3_rotate(transform->parent->local_front, transform->local_rotation);

		transform->local_right = math_vector3_norm(lr);
		transform->local_up = math_vector3_norm(lu);
		transform->local_front = math_vector3_norm(lf);

		transform->local_left = math_vector3_from_xyz(-transform->local_right.x, -transform->local_right.y, -transform->local_right.z);
		transform->local_down = math_vector3_from_xyz(-transform->local_up.x, -transform->local_up.y, -transform->local_up.z);
		transform->local_back = math_vector3_from_xyz(-transform->local_front.x, -transform->local_front.y, -transform->local_front.z);
	}
	else
	{
		quaternion_t qx = math_quaternion_angle_axis(pitch, g_world_right);
		quaternion_t qy = math_quaternion_angle_axis(yaw, g_world_up);
		quaternion_t qz = math_quaternion_angle_axis(roll, g_world_front);

		quaternion_t qxz = math_quaternion_mul(qx, qz);
		quaternion_t qyxz = math_quaternion_mul(qy, qxz);

		transform->local_rotation = math_quaternion_norm(qyxz);

		vector3_t lr = math_vector3_rotate(g_world_right, transform->local_rotation);
		vector3_t lu = math_vector3_rotate(g_world_up, transform->local_rotation);
		vector3_t lf = math_vector3_rotate(g_world_front, transform->local_rotation);

		transform->local_right = math_vector3_norm(lr);
		transform->local_up = math_vector3_norm(lu);
		transform->local_front = math_vector3_norm(lf);

		transform->local_left = math_vector3_from_xyz(-transform->local_right.x, -transform->local_right.y, -transform->local_right.z);
		transform->local_down = math_vector3_from_xyz(-transform->local_up.x, -transform->local_up.y, -transform->local_up.z);
		transform->local_back = math_vector3_from_xyz(-transform->local_front.x, -transform->local_front.y, -transform->local_front.z);
	}

	transform_compute_world_rotation(transform);
	transform_compute_world_position(transform);

	TRACY_ZONE_END
}
void transform_set_relative_euler_angles(transform_t* transform, vector3_t rotation)
{
	TRACY_ZONE_BEGIN

	double pitch = math_deg_to_rad(rotation.x);
	double yaw = math_deg_to_rad(rotation.y);
	double roll = math_deg_to_rad(rotation.z);

	if (transform->parent)
	{
		quaternion_t qx = math_quaternion_angle_axis(pitch, transform->parent->local_right);
		quaternion_t qy = math_quaternion_angle_axis(yaw, transform->parent->local_up);
		quaternion_t qz = math_quaternion_angle_axis(roll, transform->parent->local_front);

		quaternion_t qxz = math_quaternion_mul(qx, qz);
		quaternion_t qyxz = math_quaternion_mul(qy, qxz);
		quaternion_t qyxzr = math_quaternion_mul(transform->local_rotation, qyxz);

		transform->local_rotation = math_quaternion_norm(qyxzr);

		vector3_t lr = math_vector3_rotate(transform->parent->local_right, transform->local_rotation);
		vector3_t lu = math_vector3_rotate(transform->parent->local_up, transform->local_rotation);
		vector3_t lf = math_vector3_rotate(transform->parent->local_front, transform->local_rotation);

		transform->local_right = math_vector3_norm(lr);
		transform->local_up = math_vector3_norm(lu);
		transform->local_front = math_vector3_norm(lf);

		transform->local_left = math_vector3_from_xyz(-transform->local_right.x, -transform->local_right.y, -transform->local_right.z);
		transform->local_down = math_vector3_from_xyz(-transform->local_up.x, -transform->local_up.y, -transform->local_up.z);
		transform->local_back = math_vector3_from_xyz(-transform->local_front.x, -transform->local_front.y, -transform->local_front.z);
	}
	else
	{
		quaternion_t qx = math_quaternion_angle_axis(pitch, g_world_right);
		quaternion_t qy = math_quaternion_angle_axis(yaw, g_world_up);
		quaternion_t qz = math_quaternion_angle_axis(roll, g_world_front);

		quaternion_t qxz = math_quaternion_mul(qx, qz);
		quaternion_t qyxz = math_quaternion_mul(qy, qxz);
		quaternion_t qyxzr = math_quaternion_mul(transform->local_rotation, qyxz);

		transform->local_rotation = math_quaternion_norm(qyxzr);

		vector3_t lr = math_vector3_rotate(g_world_right, transform->local_rotation);
		vector3_t lu = math_vector3_rotate(g_world_up, transform->local_rotation);
		vector3_t lf = math_vector3_rotate(g_world_front, transform->local_rotation);

		transform->local_right = math_vector3_norm(lr);
		transform->local_up = math_vector3_norm(lu);
		transform->local_front = math_vector3_norm(lf);

		transform->local_left = math_vector3_from_xyz(-transform->local_right.x, -transform->local_right.y, -transform->local_right.z);
		transform->local_down = math_vector3_from_xyz(-transform->local_up.x, -transform->local_up.y, -transform->local_up.z);
		transform->local_back = math_vector3_from_xyz(-transform->local_front.x, -transform->local_front.y, -transform->local_front.z);
	}

	transform_compute_world_rotation(transform);
	transform_compute_world_position(transform);

	TRACY_ZONE_END
}
void transform_set_relative_euler_angles_pyr(transform_t* transform, double p, double y, double r)
{
	TRACY_ZONE_BEGIN

	double pitch = math_deg_to_rad(p);
	double yaw = math_deg_to_rad(y);
	double roll = math_deg_to_rad(r);

	if (transform->parent)
	{
		quaternion_t qx = math_quaternion_angle_axis(pitch, transform->parent->local_right);
		quaternion_t qy = math_quaternion_angle_axis(yaw, transform->parent->local_up);
		quaternion_t qz = math_quaternion_angle_axis(roll, transform->parent->local_front);

		quaternion_t qxz = math_quaternion_mul(qx, qz);
		quaternion_t qyxz = math_quaternion_mul(qy, qxz);
		quaternion_t qyxzr = math_quaternion_mul(transform->local_rotation, qyxz);

		transform->local_rotation = math_quaternion_norm(qyxzr);

		vector3_t lr = math_vector3_rotate(transform->parent->local_right, transform->local_rotation);
		vector3_t lu = math_vector3_rotate(transform->parent->local_up, transform->local_rotation);
		vector3_t lf = math_vector3_rotate(transform->parent->local_front, transform->local_rotation);

		transform->local_right = math_vector3_norm(lr);
		transform->local_up = math_vector3_norm(lu);
		transform->local_front = math_vector3_norm(lf);

		transform->local_left = math_vector3_from_xyz(-transform->local_right.x, -transform->local_right.y, -transform->local_right.z);
		transform->local_down = math_vector3_from_xyz(-transform->local_up.x, -transform->local_up.y, -transform->local_up.z);
		transform->local_back = math_vector3_from_xyz(-transform->local_front.x, -transform->local_front.y, -transform->local_front.z);
	}
	else
	{
		quaternion_t qx = math_quaternion_angle_axis(pitch, g_world_right);
		quaternion_t qy = math_quaternion_angle_axis(yaw, g_world_up);
		quaternion_t qz = math_quaternion_angle_axis(roll, g_world_front);

		quaternion_t qxz = math_quaternion_mul(qx, qz);
		quaternion_t qyxz = math_quaternion_mul(qy, qxz);
		quaternion_t qyxzr = math_quaternion_mul(transform->local_rotation, qyxz);

		transform->local_rotation = math_quaternion_norm(qyxzr);

		vector3_t lr = math_vector3_rotate(g_world_right, transform->local_rotation);
		vector3_t lu = math_vector3_rotate(g_world_up, transform->local_rotation);
		vector3_t lf = math_vector3_rotate(g_world_front, transform->local_rotation);

		transform->local_right = math_vector3_norm(lr);
		transform->local_up = math_vector3_norm(lu);
		transform->local_front = math_vector3_norm(lf);

		transform->local_left = math_vector3_from_xyz(-transform->local_right.x, -transform->local_right.y, -transform->local_right.z);
		transform->local_down = math_vector3_from_xyz(-transform->local_up.x, -transform->local_up.y, -transform->local_up.z);
		transform->local_back = math_vector3_from_xyz(-transform->local_front.x, -transform->local_front.y, -transform->local_front.z);
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

	double x2 = transform->world_rotation.x + transform->world_rotation.x;
	double y2 = transform->world_rotation.y + transform->world_rotation.y;
	double z2 = transform->world_rotation.z + transform->world_rotation.z;

	double xx = transform->world_rotation.x * x2;
	double yy = transform->world_rotation.y * y2;
	double zz = transform->world_rotation.z * z2;
	double xy = transform->world_rotation.x * y2;
	double xz = transform->world_rotation.x * z2;
	double yz = transform->world_rotation.y * z2;
	double wx = transform->world_rotation.w * x2;
	double wy = transform->world_rotation.w * y2;
	double wz = transform->world_rotation.w * z2;

	matrix4_t r =
	{
		.m00 = (1.0 - (yy + zz)) * transform->world_scale.x, .m01 =        (xy - wz)  * transform->world_scale.x, .m02 =        (xz + wy)  * transform->world_scale.x, .m03 = 0.0,
		.m10 =        (xy + wz)  * transform->world_scale.y, .m11 = (1.0 - (xx + zz)) * transform->world_scale.y, .m12 =        (yz - wx)  * transform->world_scale.y, .m13 = 0.0,
		.m20 =        (xz - wy)  * transform->world_scale.z, .m21 =        (yz + wx)  * transform->world_scale.z, .m22 = (1.0 - (xx + yy)) * transform->world_scale.z, .m23 = 0.0,
		.m30 =                  transform->world_position.x, .m31 =                  transform->world_position.y, .m32 =                  transform->world_position.z, .m33 = 1.0,
	};

	TRACY_ZONE_END

	return r;
}
