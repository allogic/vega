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

///////////////////////////////////////////////////////////////
// Public API
///////////////////////////////////////////////////////////////

__forceinline double math_deg_to_rad(double a);
__forceinline double math_rad_to_deg(double a);

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
__forceinline quaternion_t math_quaternion_norm(quaternion_t a);
__forceinline double math_quaternion_dot(quaternion_t a, quaternion_t b);
__forceinline double math_quaternion_length(quaternion_t a);
__forceinline double math_quaternion_length2(quaternion_t a);

#endif // VEGA_CORE_MATH_FORWARD_H
