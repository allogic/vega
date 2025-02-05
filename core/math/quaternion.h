#ifndef VEGA_CORE_MATH_QUATERNION_H
#define VEGA_CORE_MATH_QUATERNION_H

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

VEGA_EXTERN_C_BEGIN

///////////////////////////////////////////////////////////////
// Inline Definition
///////////////////////////////////////////////////////////////

__forceinline quaternion_t math_quaternion_zero(void)
{
	quaternion_t r =
	{
		.x = 0.0,
		.y = 0.0,
		.z = 0.0,
		.w = 0.0,
	};

	return r;
}
__forceinline quaternion_t math_quaternion_identity(void)
{
	quaternion_t r =
	{
		.x = 0.0,
		.y = 0.0,
		.z = 0.0,
		.w = 1.0,
	};

	return r;
}
__forceinline quaternion_t math_quaternion_xyzw(double x, double y, double z, double w)
{
	quaternion_t r =
	{
		.x = x,
		.y = y,
		.z = z,
		.w = w,
	};

	return r;
}
__forceinline quaternion_t math_quaternion_mul(quaternion_t a, quaternion_t b)
{
#if defined(VEGA_AVX_SUPPORT) && defined(__AVX__) || defined(VEGA_AVX_FORCE)
	// TODO
#elif defined(VEGA_SSE_SUPPORT) && defined(__SSE__) || defined(VEGA_SSE_FORCE)
	// TODO
#else
	quaternion_t r =
	{
		.x = (a.w * b.x) + (a.x * b.w) + (a.y * b.z) - (a.z * b.y),
		.y = (a.w * b.y) - (a.x * b.z) + (a.y * b.w) + (a.z * b.x),
		.z = (a.w * b.z) + (a.x * b.y) - (a.y * b.x) + (a.z * b.w),
		.w = (a.w * b.w) - (a.x * b.x) - (a.y * b.y) - (a.z * b.z),
	};

	return r;
#endif // VEGA_SIMD_SUPPORT
}
__forceinline quaternion_t math_quaternion_mul_scalar(quaternion_t a, double b)
{
	quaternion_t r =
	{
		.x = a.x * b,
		.y = a.y * b,
		.z = a.z * b,
		.w = a.w * b,
	};

	return r;
}
__forceinline quaternion_t math_quaternion_conjugate(quaternion_t a)
{
	quaternion_t r =
	{
		.x = -a.x,
		.y = -a.y,
		.z = -a.z,
		.w = a.w,
	};

	return r;
}
__forceinline vector3_t math_quaternion_right(quaternion_t a)
{
	vector3_t r =
	{
		.x = 1.0 - 2.0 * ((a.y * a.y) + (a.z * a.z)),
		.y =       2.0 * ((a.x * a.y) + (a.w * a.z)),
		.z =       2.0 * ((a.x * a.z) - (a.w * a.y)),
	};

	return r;
}
__forceinline vector3_t math_quaternion_up(quaternion_t a)
{
	vector3_t r =
	{
		.x =       2.0 * ((a.x * a.y) - (a.w * a.z)),
		.y = 1.0 - 2.0 * ((a.x * a.x) + (a.z * a.z)),
		.z =       2.0 * ((a.y * a.z) + (a.w * a.x)),
	};

	return r;
}
__forceinline vector3_t math_quaternion_front(quaternion_t a)
{
	vector3_t r =
	{
		.x =       2.0 * ((a.x * a.z) + (a.w * a.y)),
		.y =       2.0 * ((a.y * a.z) - (a.w * a.x)),
		.z = 1.0 - 2.0 * ((a.x * a.x) + (a.y * a.y)),
	};

	return r;
}
__forceinline vector3_t math_quaternion_left(quaternion_t a)
{
	vector3_t r =
	{
		.x = -(1.0 - 2.0 * ((a.y * a.y) + (a.z * a.z))),
		.y = -(      2.0 * ((a.x * a.y) + (a.w * a.z))),
		.z = -(      2.0 * ((a.x * a.z) - (a.w * a.y))),
	};

	return r;
}
__forceinline vector3_t math_quaternion_down(quaternion_t a)
{
	vector3_t r =
	{
		.x = -(      2.0 * ((a.x * a.y) - (a.w * a.z))),
		.y = -(1.0 - 2.0 * ((a.x * a.x) + (a.z * a.z))),
		.z = -(      2.0 * ((a.y * a.z) + (a.w * a.x))),
	};

	return r;
}
__forceinline vector3_t math_quaternion_back(quaternion_t a)
{
	vector3_t r =
	{
		.x = -(      2.0 * ((a.x * a.z) + (a.w * a.y))),
		.y = -(      2.0 * ((a.y * a.z) - (a.w * a.x))),
		.z = -(1.0 - 2.0 * ((a.x * a.x) + (a.y * a.y))),
	};

	return r;
}
__forceinline vector3_t math_quaternion_to_euler_angles(quaternion_t a)
{
#if defined(VEGA_AVX_SUPPORT) && defined(__AVX__) || defined(VEGA_AVX_FORCE)
	// TODO
#elif defined(VEGA_SSE_SUPPORT) && defined(__SSE__) || defined(VEGA_SSE_FORCE)
	// TODO
#else
	double pitch = 0.0;
	double yaw = 0.0;
	double roll = 0.0;

	double test = (a.w * a.x) - (a.y * a.z);

	if (test > (0.5 - 1.0E-6))
	{
		pitch = MATH_PI_HALF;
		yaw = 2.0 * atan2(a.z, a.w);
		roll = 0.0;
	}
	else if (test < -(0.5 - 1.0E-6))
	{
		pitch = -MATH_PI_HALF;
		yaw = -2.0 * atan2(a.z, a.w);
		roll = 0.0;
	}
	else
	{
		pitch = asin(2.0 * test);
		yaw = atan2(2.0 * ((a.w * a.y) - (a.x * a.z)), 1.0 - 2.0 * ((a.x * a.x) + (a.y * a.y)));
		roll = atan2(2.0 * ((a.w * a.z) - (a.x * a.y)), 1.0 - 2.0 * ((a.y * a.y) + (a.z * a.z)));
	}

	vector3_t r =
	{
		.x = math_rad_to_deg(pitch),
		.y = math_rad_to_deg(yaw),
		.z = math_rad_to_deg(roll),
	};

	return r;
#endif // VEGA_SIMD_SUPPORT
}
__forceinline vector3_t math_quaternion_to_euler_angles_xyzw(double x, double y, double z, double w)
{
#if defined(VEGA_AVX_SUPPORT) && defined(__AVX__) || defined(VEGA_AVX_FORCE)
	// TODO
#elif defined(VEGA_SSE_SUPPORT) && defined(__SSE__) || defined(VEGA_SSE_FORCE)
	// TODO
#else
	double pitch = 0.0;
	double yaw = 0.0;
	double roll = 0.0;

	double test = (w * x) - (y * z);

	if (test > (0.5 - 1.0E-6))
	{
		pitch = MATH_PI_HALF;
		yaw = 2.0 * atan2(z, w);
		roll = 0.0;
	}
	else if (test < -(0.5 - 1.0E-6))
	{
		pitch = -MATH_PI_HALF;
		yaw = -2.0 * atan2(z, w);
		roll = 0.0;
	}
	else
	{
		pitch = asin(2.0 * test);
		yaw = atan2(2.0 * ((w * y) - (x * z)), 1.0 - 2.0 * ((x * x) + (y * y)));
		roll = atan2(2.0 * ((w * z) - (x * y)), 1.0 - 2.0 * ((y * y) + (z * z)));
	}

	vector3_t r =
	{
		.x = math_rad_to_deg(pitch),
		.y = math_rad_to_deg(yaw),
		.z = math_rad_to_deg(roll),
	};

	return r;
#endif // VEGA_SIMD_SUPPORT
}
__forceinline quaternion_t math_quaternion_from_euler_angles(vector3_t a)
{
#if defined(VEGA_AVX_SUPPORT) && defined(__AVX__) || defined(VEGA_AVX_FORCE)
	// TODO: properly vectorize this function..

	double pitch = math_deg_to_rad(a.x);
	double yaw = math_deg_to_rad(a.y);
	double roll = math_deg_to_rad(a.z);

	__m256d e = _mm256_set_pd(0.0, roll, yaw, pitch);
	__m256d mul = _mm256_mul_pd(e, _mm256_set1_pd(0.5));

	__m256d s = _mm256_sin_pd(mul);
	__m256d c = _mm256_cos_pd(mul);

	quaternion_t r =
	{
		.x = ((double*)&s)[2] * ((double*)&c)[0] * ((double*)&c)[1] - ((double*)&c)[2] * ((double*)&s)[0] * ((double*)&s)[1],
		.y = ((double*)&c)[2] * ((double*)&s)[0] * ((double*)&c)[1] + ((double*)&s)[2] * ((double*)&c)[0] * ((double*)&s)[1],
		.z = ((double*)&c)[2] * ((double*)&c)[0] * ((double*)&s)[1] - ((double*)&s)[2] * ((double*)&s)[0] * ((double*)&c)[1],
		.w = ((double*)&c)[2] * ((double*)&c)[0] * ((double*)&c)[1] + ((double*)&s)[2] * ((double*)&s)[0] * ((double*)&s)[1],
	};

	return r;
#elif defined(VEGA_SSE_SUPPORT) && defined(__SSE__) || defined(VEGA_SSE_FORCE)
	// TODO
#else
	double pitch = math_deg_to_rad(a.x);
	double yaw = math_deg_to_rad(a.y);
	double roll = math_deg_to_rad(a.z);

	double sp = sin(pitch * 0.5);
	double sy = sin(yaw * 0.5);
	double sr = sin(roll * 0.5);

	double cp = cos(pitch * 0.5);
	double cy = cos(yaw * 0.5);
	double cr = cos(roll * 0.5);

	quaternion_t r =
	{
		.x = sr * cp * cy - cr * sp * sy,
		.y = cr * sp * cy + sr * cp * sy,
		.z = cr * cp * sy - sr * sp * cy,
		.w = cr * cp * cy + sr * sp * sy,
	};

	return r;
#endif // VEGA_SIMD_SUPPORT
}
__forceinline quaternion_t math_quaternion_from_euler_angles_xyz(double x, double y, double z)
{
#if defined(VEGA_AVX_SUPPORT) && defined(__AVX__) || defined(VEGA_AVX_FORCE)
	// TODO: properly vectorize this function..

	double pitch = math_deg_to_rad(x);
	double yaw = math_deg_to_rad(y);
	double roll = math_deg_to_rad(z);

	__m256d e = _mm256_set_pd(0.0, roll, yaw, pitch);
	__m256d mul = _mm256_mul_pd(e, _mm256_set1_pd(0.5));

	__m256d s = _mm256_sin_pd(mul);
	__m256d c = _mm256_cos_pd(mul);

	quaternion_t r =
	{
		.x = ((double*)&s)[2] * ((double*)&c)[0] * ((double*)&c)[1] - ((double*)&c)[2] * ((double*)&s)[0] * ((double*)&s)[1],
		.y = ((double*)&c)[2] * ((double*)&s)[0] * ((double*)&c)[1] + ((double*)&s)[2] * ((double*)&c)[0] * ((double*)&s)[1],
		.z = ((double*)&c)[2] * ((double*)&c)[0] * ((double*)&s)[1] - ((double*)&s)[2] * ((double*)&s)[0] * ((double*)&c)[1],
		.w = ((double*)&c)[2] * ((double*)&c)[0] * ((double*)&c)[1] + ((double*)&s)[2] * ((double*)&s)[0] * ((double*)&s)[1],
	};

	return r;
#elif defined(VEGA_SSE_SUPPORT) && defined(__SSE__) || defined(VEGA_SSE_FORCE)
	// TODO
#else
	double pitch = math_deg_to_rad(x);
	double yaw = math_deg_to_rad(y);
	double roll = math_deg_to_rad(z);

	double sp = sin(pitch * 0.5);
	double sy = sin(yaw * 0.5);
	double sr = sin(roll * 0.5);

	double cp = cos(pitch * 0.5);
	double cy = cos(yaw * 0.5);
	double cr = cos(roll * 0.5);

	quaternion_t r =
	{
		.x = sr * cp * cy - cr * sp * sy,
		.y = cr * sp * cy + sr * cp * sy,
		.z = cr * cp * sy - sr * sp * cy,
		.w = cr * cp * cy + sr * sp * sy,
	};

	return r;
#endif // VEGA_SIMD_SUPPORT
}
__forceinline quaternion_t math_quaternion_angle_axis(double a, vector3_t b)
{
	vector3_t n = math_vector3_norm(b);

	double a_half = a * 0.5;
	double s = sin(a_half);

	quaternion_t r =
	{
		.x = n.x * s,
		.y = n.y * s,
		.z = n.z * s,
		.w = cos(a_half),
	};

	return r;
}
__forceinline quaternion_t math_quaternion_norm(quaternion_t a)
{
	double l = math_quaternion_length(a);

	if (l > 0.0)
	{
		return math_quaternion_mul_scalar(a, 1.0 / l);
	}
	else
	{
		return math_quaternion_identity();
	}
}
__forceinline double math_quaternion_dot(quaternion_t a, quaternion_t b)
{
#if defined(VEGA_AVX_SUPPORT) && defined(__AVX__) || defined(VEGA_AVX_FORCE)
	__m256d va = _mm256_set_pd(a.w, a.z, a.y, a.x);
	__m256d vb = _mm256_set_pd(b.w, b.z, b.y, b.x);

	__m256d mul = _mm256_mul_pd(va, vb);

	__m256d hadd = _mm256_hadd_pd(mul, mul);

	__m128d low = _mm256_castpd256_pd128(hadd);
	__m128d high = _mm256_extractf128_pd(hadd, 1);

	__m128d dot = _mm_add_pd(low, high);

	return ((double*)&dot)[0];
#elif defined(VEGA_SSE_SUPPORT) && defined(__SSE__) || defined(VEGA_SSE_FORCE)
	__m128d va = _mm_set_pd(a.y, a.x);
	__m128d vb = _mm_set_pd(b.y, b.x);
	__m128d vc = _mm_set_pd(a.w, a.z);
	__m128d vd = _mm_set_pd(b.w, b.z);

	__m128d mul0 = _mm_mul_pd(va, vb);
	__m128d mul1 = _mm_mul_pd(vc, vd);

	__m128d hadd0 = _mm_hadd_pd(mul0, mul0);
	__m128d hadd1 = _mm_hadd_pd(mul1, mul1);

	__m128d dot = _mm_hadd_pd(hadd0, hadd1);

	return ((double*)&dot)[0];
#else
	return (a.x * b.x) + (a.y * b.y) + (a.z * b.z) + (a.w * b.w);
#endif // VEGA_SIMD_SUPPORT
}
__forceinline double math_quaternion_length(quaternion_t a)
{
	return sqrt(math_quaternion_dot(a, a));
}
__forceinline double math_quaternion_length2(quaternion_t a)
{
	return math_quaternion_dot(a, a);
}
__forceinline void math_quaternion_print(quaternion_t a)
{
	printf("[%f, %f, %f, %f]\n", a.x, a.y, a.z, a.w);
}

VEGA_EXTERN_C_END

#endif // VEGA_CORE_MATH_QUATERNION_H
