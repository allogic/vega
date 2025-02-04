#ifndef VEGA_CORE_MATH_MATRIX4_H
#define VEGA_CORE_MATH_MATRIX4_H

#include <math.h>
#include <stdio.h>

#if defined(VEGA_AVX_SUPPORT) && defined(__AVX__) || defined(VEGA_AVX_FORCE)
	#include <immintrin.h>
#elif defined(VEGA_SSE_SUPPORT) && defined(__SSE__) || defined(VEGA_SSE_FORCE)
	#include <emmintrin.h>
	#include <pmmintrin.h>
#endif // VEGA_SIMD_SUPPORT

#include <vega/core/macros.h>

#include <vega/core/math/constants.h>
#include <vega/core/math/forward.h>

// TODO: properly vectorize matrix4..

VEGA_EXTERN_C_BEGIN

///////////////////////////////////////////////////////////////
// Inline Definition
///////////////////////////////////////////////////////////////

__forceinline matrix4_t math_matrix4_zero(void)
{
	matrix4_t r =
	{
		.m00 = 0.0, .m01 = 0.0, .m02 = 0.0, .m03 = 0.0,
		.m10 = 0.0, .m11 = 0.0, .m12 = 0.0, .m13 = 0.0,
		.m20 = 0.0, .m21 = 0.0, .m22 = 0.0, .m23 = 0.0,
		.m30 = 0.0, .m31 = 0.0, .m32 = 0.0, .m33 = 0.0,
	};

	return r;
}
__forceinline matrix4_t math_matrix4_identity(void)
{
	matrix4_t r =
	{
		.m00 = 1.0, .m01 = 0.0, .m02 = 0.0, .m03 = 0.0,
		.m10 = 0.0, .m11 = 1.0, .m12 = 0.0, .m13 = 0.0,
		.m20 = 0.0, .m21 = 0.0, .m22 = 1.0, .m23 = 0.0,
		.m30 = 0.0, .m31 = 0.0, .m32 = 0.0, .m33 = 1.0,
	};

	return r;
}
__forceinline vector3_t math_matrix4_position(matrix4_t a)
{
	vector3_t r =
	{
		.x = a.m30,
		.y = a.m31,
		.z = a.m32,
	};

	return r;
}
__forceinline quaternion_t math_matrix4_rotation(matrix4_t a)
{
	quaternion_t r;

	vector3_t c0 = { .x = a.m00, .y = a.m10, .z = a.m20 };
	vector3_t c1 = { .x = a.m01, .y = a.m11, .z = a.m21 };
	vector3_t c2 = { .x = a.m02, .y = a.m12, .z = a.m22 };

	vector3_t cn0 = math_vector3_norm(c0);
	vector3_t cn1 = math_vector3_norm(c1);
	vector3_t cn2 = math_vector3_norm(c2);

	double trace = cn0.x + cn1.y + cn2.z;
	if (trace > 0.0)
	{
		double s = 0.5 / sqrt(trace + 1.0);

		r.x = (cn2.y - cn1.z) * s;
		r.y = (cn0.z - cn2.x) * s;
		r.z = (cn1.x - cn0.y) * s;
		r.w = 0.25 / s;
	}
	else
	{
		if ((cn0.x > cn1.y) && (cn0.x > cn2.z))
		{
			double s = 2.0 * sqrt(1.0 + cn0.x - cn1.y - cn2.z);

			r.x = 0.25 * s;
			r.y = (cn0.y + cn1.x) / s;
			r.z = (cn0.z + cn2.y) / s;
			r.w = (cn2.y - cn1.z) / s;
		}
		else if (cn1.y > cn2.z)
		{
			double s = 2.0 * sqrt(1.0 + cn1.y - cn0.x - cn2.z);

			r.x = (cn0.y + cn1.x) / s;
			r.y = 0.25 * s;
			r.z = (cn1.z + cn2.y) / s;
			r.w = (cn0.z - cn2.x) / s;
		}
		else
		{
			double s = 2.0 * sqrt(1.0 + cn2.z - cn0.x - cn1.y);

			r.x = (cn0.z + cn2.x) / s;
			r.y = (cn1.z + cn2.y) / s;
			r.z = 0.25 * s;
			r.w = (cn1.x - cn0.y) / s;
		}
	}

	return r;
}
__forceinline vector3_t math_matrix4_euler_angles(matrix4_t a)
{
	vector3_t r;

	if (fabs(a.m02) < (1.0 - MATH_EPSILON_15))
	{
		r.x = atan2(-a.m12, a.m22);
		r.y = asin(a.m02);
		r.z = atan2(-a.m01, a.m00);
	}
	else
	{
		r.x = atan2(a.m10, a.m11);
		r.y = (a.m02 > 0.0) ? (MATH_PI / 2.0) : (-MATH_PI / 2.0);
		r.z = 0.0;
	}

	return r;
}
__forceinline vector3_t math_matrix4_scale(matrix4_t a)
{
	vector3_t r =
	{
		.x = a.m00,
		.y = a.m11,
		.z = a.m22,
	};

	return r;
}
__forceinline void math_matrix4_decompose(matrix4_t a, vector3_t* p, quaternion_t* r, vector3_t* s)
{
	p->x = a.m30;
	p->y = a.m31;
	p->z = a.m32;

	vector3_t c0 = { .x = a.m00, .y = a.m10, .z = a.m20 };
	vector3_t c1 = { .x = a.m01, .y = a.m11, .z = a.m21 };
	vector3_t c2 = { .x = a.m02, .y = a.m12, .z = a.m22 };

	s->x = math_vector3_length(c0);
	s->y = math_vector3_length(c1);
	s->z = math_vector3_length(c2);

	vector3_t cn0 = math_vector3_norm(c0);
	vector3_t cn1 = math_vector3_norm(c1);
	vector3_t cn2 = math_vector3_norm(c2);

	double trace = cn0.x + cn1.y + cn2.z;
	if (trace > 0.0)
	{
		double s = 0.5 / sqrt(trace + 1.0);

		r->x = (cn2.y - cn1.z) * s;
		r->y = (cn0.z - cn2.x) * s;
		r->z = (cn1.x - cn0.y) * s;
		r->w = 0.25 / s;
	}
	else
	{
		if ((cn0.x > cn1.y) && (cn0.x > cn2.z))
		{
			double s = 2.0 * sqrt(1.0 + cn0.x - cn1.y - cn2.z);

			r->x = 0.25 * s;
			r->y = (cn0.y + cn1.x) / s;
			r->z = (cn0.z + cn2.y) / s;
			r->w = (cn2.y - cn1.z) / s;
		}
		else if (cn1.y > cn2.z)
		{
			double s = 2.0 * sqrt(1.0 + cn1.y - cn0.x - cn2.z);

			r->x = (cn0.y + cn1.x) / s;
			r->y = 0.25 * s;
			r->z = (cn1.z + cn2.y) / s;
			r->w = (cn0.z - cn2.x) / s;
		}
		else
		{
			double s = 2.0 * sqrt(1.0 + cn2.z - cn0.x - cn1.y);

			r->x = (cn0.z + cn2.x) / s;
			r->y = (cn1.z + cn2.y) / s;
			r->z = 0.25 * s;
			r->w = (cn1.x - cn0.y) / s;
		}
	}
}
__forceinline matrix4_t math_matrix4_mul(matrix4_t a, matrix4_t b)
{
#if defined(VEGA_AVX_SUPPORT) && defined(__AVX__) || defined(VEGA_AVX_FORCE)
	// TODO
#elif defined(VEGA_SSE_SUPPORT) && defined(__SSE__) || defined(VEGA_SSE_FORCE)
	// TODO
#else
	matrix4_t r =
	{
		.m00 = (a.m00 * a.m00) + (a.m10 * b.m01) + (a.m20 * b.m02) + (a.m30 * b.m03),
		.m01 = (a.m01 * a.m00) + (a.m11 * b.m01) + (a.m21 * b.m02) + (a.m31 * b.m03),
		.m02 = (a.m02 * a.m00) + (a.m12 * b.m01) + (a.m22 * b.m02) + (a.m32 * b.m03),
		.m03 = (a.m03 * a.m00) + (a.m13 * b.m01) + (a.m23 * b.m02) + (a.m33 * b.m03),
		.m10 = (a.m00 * a.m10) + (a.m10 * b.m11) + (a.m20 * b.m12) + (a.m30 * b.m13),
		.m11 = (a.m01 * a.m10) + (a.m11 * b.m11) + (a.m21 * b.m12) + (a.m31 * b.m13),
		.m12 = (a.m02 * a.m10) + (a.m12 * b.m11) + (a.m22 * b.m12) + (a.m32 * b.m13),
		.m13 = (a.m03 * a.m10) + (a.m13 * b.m11) + (a.m23 * b.m12) + (a.m33 * b.m13),
		.m20 = (a.m00 * a.m20) + (a.m10 * b.m21) + (a.m20 * b.m22) + (a.m30 * b.m23),
		.m21 = (a.m01 * a.m20) + (a.m11 * b.m21) + (a.m21 * b.m22) + (a.m31 * b.m23),
		.m22 = (a.m02 * a.m20) + (a.m12 * b.m21) + (a.m22 * b.m22) + (a.m32 * b.m23),
		.m23 = (a.m03 * a.m20) + (a.m13 * b.m21) + (a.m23 * b.m22) + (a.m33 * b.m23),
		.m30 = (a.m00 * a.m30) + (a.m10 * b.m31) + (a.m20 * b.m32) + (a.m30 * b.m33),
		.m31 = (a.m01 * a.m30) + (a.m11 * b.m31) + (a.m21 * b.m32) + (a.m31 * b.m33),
		.m32 = (a.m02 * a.m30) + (a.m12 * b.m31) + (a.m22 * b.m32) + (a.m32 * b.m33),
		.m33 = (a.m03 * a.m30) + (a.m13 * b.m31) + (a.m23 * b.m32) + (a.m33 * b.m33),
	};

	return r;
#endif // VEGA_SIMD_SUPPORT
}
__forceinline matrix4_t math_matrix4_ortho(double left, double right, double bottom, double top, double near_z, double far_z)
{
	matrix4_t r =
	{
		.m00 =             2.0 / (right - left), .m01 =                              0.0, .m02 =                        0.0, .m03 = 0.0,
		.m10 =                              0.0, .m11 =             2.0 / (top - bottom), .m12 =                        0.0, .m13 = 0.0,
		.m20 =                              0.0, .m21 =                              0.0, .m22 =     1.0 / (far_z - near_z), .m23 = 1.0,
		.m30 = -(right + left) / (right - left), .m31 = -(top + bottom) / (top - bottom), .m32 = -near_z / (far_z - near_z), .m33 = 0.0,
	};

	return r;
}
__forceinline matrix4_t math_matrix4_persp(double fov, double aspect_ratio, double near_z, double far_z)
{
	double tan_half_fov = tan(fov * 0.5);

	matrix4_t r =
	{
		.m00 = 1.0 / (aspect_ratio * tan_half_fov), .m01 =                0.0, .m02 =                                  0.0, .m03 = 0.0,
		.m10 =                                 0.0, .m11 = 1.0 / tan_half_fov, .m12 =                                  0.0, .m13 = 0.0,
		.m20 =                                 0.0, .m21 =                0.0, .m22 =             far_z / (far_z - near_z), .m23 = 1.0,
		.m30 =                                 0.0, .m31 =                0.0, .m32 = -(far_z * near_z) / (far_z - near_z), .m33 = 0.0,
	};

	return r;
}
__forceinline matrix4_t math_matrix4_look_at(vector3_t eye, vector3_t center, vector3_t up)
{
	vector3_t f = math_vector3_sub(center, eye);
	vector3_t fn = math_vector3_norm(f);

	vector3_t s = math_vector3_cross(up, fn);
	vector3_t sn = math_vector3_norm(s);

	vector3_t u = math_vector3_cross(fn, sn);

	matrix4_t r =
	{
		.m00 =                       s.x, .m01 =                       u.x, .m02 =                       fn.x, .m03 = 0.0,
		.m10 =                       s.y, .m11 =                       u.y, .m12 =                       fn.y, .m13 = 0.0,
		.m20 =                       s.z, .m21 =                       u.z, .m22 =                       fn.z, .m23 = 0.0,
		.m30 = -math_vector3_dot(s, eye), .m31 = -math_vector3_dot(u, eye), .m32 = -math_vector3_dot(fn, eye), .m33 = 1.0,
	};

	return r;
}
__forceinline void math_matrix4_print(matrix4_t a)
{
	printf("[%f, %f, %f, %f]\n", a.m00, a.m01, a.m02, a.m03);
	printf("[%f, %f, %f, %f]\n", a.m10, a.m11, a.m12, a.m13);
	printf("[%f, %f, %f, %f]\n", a.m20, a.m21, a.m22, a.m23);
	printf("[%f, %f, %f, %f]\n", a.m30, a.m31, a.m32, a.m33);
}

VEGA_EXTERN_C_END

#endif // VEGA_CORE_MATH_MATRIX4_H
