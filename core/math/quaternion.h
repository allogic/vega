#ifndef VEGA_CORE_MATH_QUATERNION_H
#define VEGA_CORE_MATH_QUATERNION_H

#include <math.h>

#ifdef VEGA_AVX_SUPPORT
	#include <immintrin.h>
#elif VEGA_SSE_SUPPORT
	#include <emmintrin.h>
	#include <pmmintrin.h>
#endif // VEGA_SIMD_SUPPORT

#include <vega/core/math/constants.h>
#include <vega/core/math/forward.h>

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
__forceinline quaternion_t math_quaternion_from_xyzw(double x, double y, double z, double w)
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
#ifdef VEGA_AVX_SUPPORT
	// TODO
#elif VEGA_SSE_SUPPORT
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
__forceinline vector3_t math_quaternion_to_euler_angles(quaternion_t a)
{
#ifdef VEGA_AVX_SUPPORT
	// TODO
#elif VEGA_SSE_SUPPORT
	// TODO
#else
	double srcp = 2.0 * ((a.w * a.x) + (a.y * a.z));
	double crcp = 1.0 - 2.0 * (a.x * a.x + a.y * a.y);
	double roll = atan2(srcp, crcp);

	double sp = 2.0 * ((a.w * a.y) - (a.z * a.x));
	double pitch = 0.0;

	if (fabs(sp) >= 1.0)
	{
		pitch = copysign(MATH_PI / 2.0, sp);
	}
	else
	{
		pitch = asin(sp);
	}

	double sycp = 2.0 * ((a.w * a.z) + (a.x * a.y));
	double cycp = 1.0 - 2.0 * (a.y * a.y + a.z * a.z);
	double yaw = atan2(sycp, cycp);

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
#ifdef VEGA_AVX_SUPPORT
	// TODO
#elif VEGA_SSE_SUPPORT
	// TODO
#else
	double srcp = 2.0 * ((w * x) + (y * z));
	double crcp = 1.0 - 2.0 * (x * x + y * y);
	double roll = atan2(srcp, crcp);

	double sp = 2.0 * ((w * y) - (z * x));
	double pitch = 0.0;

	if (fabs(sp) >= 1.0)
	{
		pitch = copysign(MATH_PI / 2.0, sp);
	}
	else
	{
		pitch = asin(sp);
	}

	double sycp = 2.0 * ((w * z) + (x * y));
	double cycp = 1.0 - 2.0 * (y * y + z * z);
	double yaw = atan2(sycp, cycp);

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
#ifdef VEGA_AVX_SUPPORT
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
#elif VEGA_SSE_SUPPORT
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
#ifdef VEGA_AVX_SUPPORT
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
#elif VEGA_SSE_SUPPORT
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
#ifdef VEGA_AVX_SUPPORT
	// TODO
#elif VEGA_SSE_SUPPORT
	// TODO
#else
	vector3_t n = math_vector3_norm(b);

	double a_two = a / 2.0;
	double s = sin(a_two);

	quaternion_t r =
	{
		.x = n.x * s,
		.y = n.y * s,
		.z = n.z * s,
		.w = cos(a_two),
	};

	return r;
#endif // VEGA_SIMD_SUPPORT
}
__forceinline quaternion_t math_quaternion_norm(quaternion_t a)
{
	double l = math_quaternion_length(a);

	if (l == 0.0)
	{
		return math_quaternion_zero();
	}
	else
	{
		return math_quaternion_mul_scalar(a, 1.0 / l);
	}
}
__forceinline double math_quaternion_dot(quaternion_t a, quaternion_t b)
{
#ifdef VEGA_AVX_SUPPORT
	__m256d va = _mm256_set_pd(a.w, a.z, a.y, a.x);
	__m256d vb = _mm256_set_pd(b.w, b.z, b.y, b.x);

	__m256d mul = _mm256_mul_pd(va, vb);

	__m256d hadd = _mm256_hadd_pd(mul, mul);

	__m128d low = _mm256_castpd256_pd128(hadd);
	__m128d high = _mm256_extractf128_pd(hadd, 1);

	__m128d dot = _mm_add_pd(low, high);

	return ((double*)&dot)[0];
#elif VEGA_SSE_SUPPORT
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

#endif // VEGA_CORE_MATH_QUATERNION_H
