#ifndef VEGA_CORE_MATH_FORWARD_H
#define VEGA_CORE_MATH_FORWARD_H

typedef struct _vector2_t
{
	double x;
	double y;
} vector2_t;

typedef struct _vector3_t
{
	double x;
	double y;
	double z;
} vector3_t;

typedef struct _vector4_t
{
	double x;
	double y;
	double z;
	double w;
} vector4_t;

typedef struct _quaternion_t
{
	double x;
	double y;
	double z;
	double w;
} quaternion_t;

typedef struct _matrix4_t
{
	double m00, m01, m02, m03;
	double m10, m11, m12, m13;
	double m20, m21, m22, m23;
	double m30, m31, m32, m33;
} matrix4_t;

///////////////////////////////////////////////////////////////
// Public API
///////////////////////////////////////////////////////////////

__forceinline double math_deg_to_rad(double a);
__forceinline double math_rad_to_deg(double a);
__forceinline double math_fast_inverse_sqrt(double a);

__forceinline vector2_t math_vector2_zero(void);
__forceinline vector2_t math_vector2_from_xy(double x, double y);
__forceinline vector2_t math_vector2_add(vector2_t a, vector2_t b);
__forceinline vector2_t math_vector2_sub(vector2_t a, vector2_t b);
__forceinline vector2_t math_vector2_mul(vector2_t a, vector2_t b);
__forceinline vector2_t math_vector2_div(vector2_t a, vector2_t b);
__forceinline vector2_t math_vector2_add_scalar(vector2_t a, double b);
__forceinline vector2_t math_vector2_sub_scalar(vector2_t a, double b);
__forceinline vector2_t math_vector2_mul_scalar(vector2_t a, double b);
__forceinline vector2_t math_vector2_div_scalar(vector2_t a, double b);
__forceinline vector2_t math_vector2_norm(vector2_t a);
__forceinline double math_vector2_dot(vector2_t a, vector2_t b);
__forceinline double math_vector2_length(vector2_t a);
__forceinline double math_vector2_length2(vector2_t a);

__forceinline vector3_t math_vector3_zero(void);
__forceinline vector3_t math_vector3_from_xyz(double x, double y, double z);
__forceinline vector3_t math_vector3_add(vector3_t a, vector3_t b);
__forceinline vector3_t math_vector3_sub(vector3_t a, vector3_t b);
__forceinline vector3_t math_vector3_mul(vector3_t a, vector3_t b);
__forceinline vector3_t math_vector3_div(vector3_t a, vector3_t b);
__forceinline vector3_t math_vector3_add_scalar(vector3_t a, double b);
__forceinline vector3_t math_vector3_sub_scalar(vector3_t a, double b);
__forceinline vector3_t math_vector3_mul_scalar(vector3_t a, double b);
__forceinline vector3_t math_vector3_div_scalar(vector3_t a, double b);
__forceinline vector3_t math_vector3_norm(vector3_t a);
__forceinline vector3_t math_vector3_cross(vector3_t a, vector3_t b);
__forceinline vector3_t math_vector3_rotate(vector3_t a, quaternion_t b);
__forceinline double math_vector3_dot(vector3_t a, vector3_t b);
__forceinline double math_vector3_length(vector3_t a);
__forceinline double math_vector3_length2(vector3_t a);

__forceinline vector4_t math_vector4_zero(void);
__forceinline vector4_t math_vector4_from_xyzw(double x, double y, double z, double w);
__forceinline vector4_t math_vector4_add(vector4_t a, vector4_t b);
__forceinline vector4_t math_vector4_sub(vector4_t a, vector4_t b);
__forceinline vector4_t math_vector4_mul(vector4_t a, vector4_t b);
__forceinline vector4_t math_vector4_div(vector4_t a, vector4_t b);
__forceinline vector4_t math_vector4_add_scalar(vector4_t a, double b);
__forceinline vector4_t math_vector4_sub_scalar(vector4_t a, double b);
__forceinline vector4_t math_vector4_mul_scalar(vector4_t a, double b);
__forceinline vector4_t math_vector4_div_scalar(vector4_t a, double b);
__forceinline vector4_t math_vector4_norm(vector4_t a);
__forceinline double math_vector4_dot(vector4_t a, vector4_t b);
__forceinline double math_vector4_length(vector4_t a);
__forceinline double math_vector4_length2(vector4_t a);

__forceinline quaternion_t math_quaternion_zero(void);
__forceinline quaternion_t math_quaternion_identity(void);
__forceinline quaternion_t math_quaternion_from_xyzw(double x, double y, double z, double w);
__forceinline quaternion_t math_quaternion_mul(quaternion_t a, quaternion_t b);
__forceinline quaternion_t math_quaternion_mul_scalar(quaternion_t a, double b);
__forceinline quaternion_t math_quaternion_conjugate(quaternion_t a);
__forceinline vector3_t math_quaternion_to_euler_angles(quaternion_t a);
__forceinline vector3_t math_quaternion_to_euler_angles_xyzw(double x, double y, double z, double w);
__forceinline quaternion_t math_quaternion_from_euler_angles(vector3_t a);
__forceinline quaternion_t math_quaternion_from_euler_angles_xyz(double x, double y, double z);
__forceinline quaternion_t math_quaternion_angle_axis(double a, vector3_t b);
__forceinline quaternion_t math_quaternion_norm(quaternion_t a); // TODO: norm using inversesqrt..
__forceinline double math_quaternion_dot(quaternion_t a, quaternion_t b);
__forceinline double math_quaternion_length(quaternion_t a);
__forceinline double math_quaternion_length2(quaternion_t a);

__forceinline matrix4_t math_matrix4_zero(void);
__forceinline matrix4_t math_matrix4_identity(void);
__forceinline vector3_t math_matrix4_position(matrix4_t a);
__forceinline quaternion_t math_matrix4_rotation(matrix4_t a);
__forceinline vector3_t math_matrix4_euler_angles(matrix4_t a);
__forceinline vector3_t math_matrix4_scale(matrix4_t a);

/* TODO
__forceinline void math_matrix4_SetPosition(Vector3* const Value);
__forceinline void math_matrix4_SetPositionSimple(double ValueX, double ValueY, double ValueZ);
__forceinline void math_matrix4_SetRotation(Quaternion* const Value);
__forceinline void math_matrix4_SetRotationSimple(double ValueX, double ValueY, double ValueZ, double ValueW);
__forceinline void math_matrix4_SetRotationEulerAngles(Vector3* const Value);
__forceinline void math_matrix4_SetRotationEulerAnglesSimple(double Pitch, double Yaw, double Roll);
__forceinline void math_matrix4_SetScale(Vector3* const Value);
__forceinline void math_matrix4_SetScaleSimple(double ValueX, double ValueY, double ValueZ);
*/

__forceinline void math_matrix4_decompose(matrix4_t a, vector3_t* p, quaternion_t* r, vector3_t* s);
__forceinline matrix4_t math_matrix4_mul(matrix4_t a, matrix4_t b);
__forceinline matrix4_t math_matrix4_ortho(double left, double right, double bottom, double top, double near_z, double far_z);
__forceinline matrix4_t math_matrix4_persp(double fov, double aspect_ratio, double near_z, double far_z);
__forceinline matrix4_t math_matrix4_look_at(vector3_t eye, vector3_t center, vector3_t up);

#endif // VEGA_CORE_MATH_FORWARD_H
