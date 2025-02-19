#ifndef VEGA_CORE_MATH_VECTOR3_H
#define VEGA_CORE_MATH_VECTOR3_H

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

extern vector3_t const g_world_right;
extern vector3_t const g_world_up;
extern vector3_t const g_world_front;

extern vector3_t const g_world_left;
extern vector3_t const g_world_down;
extern vector3_t const g_world_back;

///////////////////////////////////////////////////////////////
// Inline Definition
///////////////////////////////////////////////////////////////

__forceinline vector3_t math_vector3_zero(void)
{
	vector3_t r =
	{
		.x = 0.0,
		.y = 0.0,
		.z = 0.0,
	};

	return r;
}
__forceinline vector3_t math_vector3_one(void)
{
	vector3_t r =
	{
		.x = 1.0,
		.y = 1.0,
		.z = 1.0,
	};

	return r;
}
__forceinline vector3_t math_vector3_xyz(double x, double y, double z)
{
	vector3_t r =
	{
		.x = x,
		.y = y,
		.z = z,
	};

	return r;
}
__forceinline vector3_t math_vector3_invert(vector3_t a)
{
	vector3_t r =
	{
		.x = -a.x,
		.y = -a.y,
		.z = -a.z,
	};

	return r;
}
__forceinline vector3_t math_vector3_add(vector3_t a, vector3_t b)
{
#if defined(VEGA_AVX_SUPPORT) && defined(__AVX__) || defined(VEGA_AVX_FORCE)
	__m256d va = _mm256_set_pd(0.0, a.z, a.y, a.x);
	__m256d vb = _mm256_set_pd(0.0, b.z, b.y, b.x);

	__m256d add = _mm256_add_pd(va, vb);

	vector3_t r =
	{
		.x = ((double*)&add)[0],
		.y = ((double*)&add)[1],
		.z = ((double*)&add)[2],
	};

	return r;
#elif defined(VEGA_SSE_SUPPORT) && defined(__SSE__) || defined(VEGA_SSE_FORCE)
	__m128d va = _mm_set_pd(a.y, a.x);
	__m128d vb = _mm_set_pd(b.y, b.x);

	__m128d add = _mm_add_pd(va, vb);

	vector3_t r =
	{
		.x = ((double*)&add)[0],
		.y = ((double*)&add)[1],
		.z = a.z + b.z,
	};

	return r;
#else
	vector3_t r =
	{
		.x = a.x + b.x,
		.y = a.y + b.y,
		.z = a.z + b.z,
	};

	return r;
#endif // VEGA_SIMD_SUPPORT
}
__forceinline vector3_t math_vector3_sub(vector3_t a, vector3_t b)
{
#if defined(VEGA_AVX_SUPPORT) && defined(__AVX__) || defined(VEGA_AVX_FORCE)
	__m256d va = _mm256_set_pd(0.0, a.z, a.y, a.x);
	__m256d vb = _mm256_set_pd(0.0, b.z, b.y, b.x);

	__m256d sub = _mm256_sub_pd(va, vb);

	vector3_t r =
	{
		.x = ((double*)&sub)[0],
		.y = ((double*)&sub)[1],
		.z = ((double*)&sub)[2],
	};

	return r;
#elif defined(VEGA_SSE_SUPPORT) && defined(__SSE__) || defined(VEGA_SSE_FORCE)
	__m128d va = _mm_set_pd(a.y, a.x);
	__m128d vb = _mm_set_pd(b.y, b.x);

	__m128d sub = _mm_sub_pd(va, vb);

	vector3_t r =
	{
		.x = ((double*)&sub)[0],
		.y = ((double*)&sub)[1],
		.z = a.z - b.z,
	};

	return r;
#else
	vector3_t r =
	{
		.x = a.x - b.x,
		.y = a.y - b.y,
		.z = a.z - b.z,
	};

	return r;
#endif // VEGA_SIMD_SUPPORT
}
__forceinline vector3_t math_vector3_mul(vector3_t a, vector3_t b)
{
#if defined(VEGA_AVX_SUPPORT) && defined(__AVX__) || defined(VEGA_AVX_FORCE)
	__m256d va = _mm256_set_pd(0.0, a.z, a.y, a.x);
	__m256d vb = _mm256_set_pd(0.0, b.z, b.y, b.x);

	__m256d mul = _mm256_mul_pd(va, vb);

	vector3_t r =
	{
		.x = ((double*)&mul)[0],
		.y = ((double*)&mul)[1],
		.z = ((double*)&mul)[2],
	};

	return r;
#elif defined(VEGA_SSE_SUPPORT) && defined(__SSE__) || defined(VEGA_SSE_FORCE)
	__m128d va = _mm_set_pd(a.y, a.x);
	__m128d vb = _mm_set_pd(b.y, b.x);

	__m128d mul = _mm_mul_pd(va, vb);

	vector3_t r =
	{
		.x = ((double*)&mul)[0],
		.y = ((double*)&mul)[1],
		.z = a.z * b.z,
	};

	return r;
#else
	vector3_t r =
	{
		.x = a.x * b.x,
		.y = a.y * b.y,
		.z = a.z * b.z,
	};

	return r;
#endif // VEGA_SIMD_SUPPORT
}
__forceinline vector3_t math_vector3_div(vector3_t a, vector3_t b)
{
#if defined(VEGA_AVX_SUPPORT) && defined(__AVX__) || defined(VEGA_AVX_FORCE)
	__m256d va = _mm256_set_pd(0.0, a.z, a.y, a.x);
	__m256d vb = _mm256_set_pd(0.0, b.z, b.y, b.x);

	__m256d div = _mm256_div_pd(va, vb);

	vector3_t r =
	{
		.x = ((double*)&div)[0],
		.y = ((double*)&div)[1],
		.z = ((double*)&div)[2],
	};

	return r;
#elif defined(VEGA_SSE_SUPPORT) && defined(__SSE__) || defined(VEGA_SSE_FORCE)
	__m128d va = _mm_set_pd(a.y, a.x);
	__m128d vb = _mm_set_pd(b.y, b.x);

	__m128d div = _mm_div_pd(va, vb);

	vector3_t r =
	{
		.x = ((double*)&div)[0],
		.y = ((double*)&div)[1],
		.z = a.z / b.z,
	};

	return r;
#else
	vector3_t r =
	{
		.x = a.x / b.x,
		.y = a.y / b.y,
		.z = a.z / b.z,
	};

	return r;
#endif // VEGA_SIMD_SUPPORT
}
__forceinline vector3_t math_vector3_add_scalar(vector3_t a, double b)
{
#if defined(VEGA_AVX_SUPPORT) && defined(__AVX__) || defined(VEGA_AVX_FORCE)
	__m256d va = _mm256_set_pd(0.0, a.z, a.y, a.x);
	__m256d vb = _mm256_set1_pd(b);

	__m256d add = _mm256_add_pd(va, vb);

	vector3_t r =
	{
		.x = ((double*)&add)[0],
		.y = ((double*)&add)[1],
		.z = ((double*)&add)[2],
	};

	return r;
#elif defined(VEGA_SSE_SUPPORT) && defined(__SSE__) || defined(VEGA_SSE_FORCE)
	__m128d va = _mm_set_pd(a.y, a.x);
	__m128d vb = _mm_set_pd(b, b);

	__m128d add = _mm_add_pd(va, vb);

	vector3_t r =
	{
		.x = ((double*)&add)[0],
		.y = ((double*)&add)[1],
		.z = a.z + b,
	};

	return r;
#else
	vector3_t r =
	{
		.x = a.x + b,
		.y = a.y + b,
		.z = a.z + b,
	};

	return r;
#endif // VEGA_SIMD_SUPPORT
}
__forceinline vector3_t math_vector3_sub_scalar(vector3_t a, double b)
{
#if defined(VEGA_AVX_SUPPORT) && defined(__AVX__) || defined(VEGA_AVX_FORCE)
	__m256d va = _mm256_set_pd(0.0, a.z, a.y, a.x);
	__m256d vb = _mm256_set1_pd(b);

	__m256d sub = _mm256_sub_pd(va, vb);

	vector3_t r =
	{
		.x = ((double*)&sub)[0],
		.y = ((double*)&sub)[1],
		.z = ((double*)&sub)[2],
	};

	return r;
#elif defined(VEGA_SSE_SUPPORT) && defined(__SSE__) || defined(VEGA_SSE_FORCE)
	__m128d va = _mm_set_pd(a.y, a.x);
	__m128d vb = _mm_set_pd(b, b);

	__m128d sub = _mm_sub_pd(va, vb);

	vector3_t r =
	{
		.x = ((double*)&sub)[0],
		.y = ((double*)&sub)[1],
		.z = a.z - b,
	};

	return r;
#else
	vector3_t r =
	{
		.x = a.x - b,
		.y = a.y - b,
		.z = a.z - b,
	};

	return r;
#endif // VEGA_SIMD_SUPPORT
}
__forceinline vector3_t math_vector3_mul_scalar(vector3_t a, double b)
{
#if defined(VEGA_AVX_SUPPORT) && defined(__AVX__) || defined(VEGA_AVX_FORCE)
	__m256d va = _mm256_set_pd(0.0, a.z, a.y, a.x);
	__m256d vb = _mm256_set1_pd(b);

	__m256d mul = _mm256_mul_pd(va, vb);

	vector3_t r =
	{
		.x = ((double*)&mul)[0],
		.y = ((double*)&mul)[1],
		.z = ((double*)&mul)[2],
	};

	return r;
#elif defined(VEGA_SSE_SUPPORT) && defined(__SSE__) || defined(VEGA_SSE_FORCE)
	__m128d va = _mm_set_pd(a.y, a.x);
	__m128d vb = _mm_set_pd(b, b);

	__m128d mul = _mm_mul_pd(va, vb);

	vector3_t r =
	{
		.x = ((double*)&mul)[0],
		.y = ((double*)&mul)[1],
		.z = a.z * b,
	};

	return r;
#else
	vector3_t r =
	{
		.x = a.x * b,
		.y = a.y * b,
		.z = a.z * b,
	};

	return r;
#endif // VEGA_SIMD_SUPPORT
}
__forceinline vector3_t math_vector3_div_scalar(vector3_t a, double b)
{
#if defined(VEGA_AVX_SUPPORT) && defined(__AVX__) || defined(VEGA_AVX_FORCE)
	__m256d va = _mm256_set_pd(0.0, a.z, a.y, a.x);
	__m256d vb = _mm256_set1_pd(b);

	__m256d div = _mm256_div_pd(va, vb);

	vector3_t r =
	{
		.x = ((double*)&div)[0],
		.y = ((double*)&div)[1],
		.z = ((double*)&div)[2],
	};

	return r;
#elif defined(VEGA_SSE_SUPPORT) && defined(__SSE__) || defined(VEGA_SSE_FORCE)
	__m128d va = _mm_set_pd(a.y, a.x);
	__m128d vb = _mm_set_pd(b, b);

	__m128d div = _mm_div_pd(va, vb);

	vector3_t r =
	{
		.x = ((double*)&div)[0],
		.y = ((double*)&div)[1],
		.z = a.z / b,
	};

	return r;
#else
	vector3_t r =
	{
		.x = a.x / b,
		.y = a.y / b,
		.z = a.z / b,
	};

	return r;
#endif // VEGA_SIMD_SUPPORT
}
__forceinline vector3_t math_vector3_norm(vector3_t a)
{
	double l = math_vector3_length(a);

	if (l > 0.0)
	{
		return math_vector3_mul_scalar(a, 1.0 / l);
	}
	else
	{
		return math_vector3_zero();
	}
}
__forceinline vector3_t math_vector3_cross(vector3_t a, vector3_t b)
{
#if defined(VEGA_AVX_SUPPORT) && defined(__AVX__) || defined(VEGA_AVX_FORCE)
	__m256d va = _mm256_set_pd(0.0, a.z, a.y, a.x);
	__m256d vb = _mm256_set_pd(0.0, b.z, b.y, b.x);

	__m256d vap0 = _mm256_permute4x64_pd(va, _MM_SHUFFLE(3, 0, 2, 1));
	__m256d vbp0 = _mm256_permute4x64_pd(vb, _MM_SHUFFLE(3, 1, 0, 2));
	__m256d vap1 = _mm256_permute4x64_pd(va, _MM_SHUFFLE(3, 1, 0, 2));
	__m256d vbp1 = _mm256_permute4x64_pd(vb, _MM_SHUFFLE(3, 0, 2, 1));

	__m256d mul0 = _mm256_mul_pd(vap0, vbp0);
	__m256d mul1 = _mm256_mul_pd(vap1, vbp1);

	__m256d cross = _mm256_sub_pd(mul0, mul1);

	vector3_t r =
	{
		.x = ((double*)&cross)[0],
		.y = ((double*)&cross)[1],
		.z = ((double*)&cross)[2],
	};

	return r;
#elif defined(VEGA_SSE_SUPPORT) && defined(__SSE__) || defined(VEGA_SSE_FORCE)
	__m128d azy = _mm_set_pd(a.z, a.y);
	__m128d axz = _mm_set_pd(a.x, a.z);
	__m128d bzy = _mm_set_pd(b.z, b.y);
	__m128d bxz = _mm_set_pd(b.x, b.z);

	__m128d mul0 = _mm_mul_pd(azy, bxz);
	__m128d mul1 = _mm_mul_pd(axz, bzy);

	__m128d cross = _mm_sub_pd(mul0, mul1);

	vector3_t r =
	{
		.x = ((double*)&cross)[1],
		.y = ((double*)&cross)[0],
		.z = (a.x * b.y) - (a.y * b.x),
	};

	return r;
#else
	vector3_t r =
	{
		.x = (a.y * b.z) - (a.z * b.y),
		.y = (a.z * b.x) - (a.x * b.z),
		.z = (a.x * b.y) - (a.y * b.x),
	};

	return r;
#endif // VEGA_SIMD_SUPPORT
}
__forceinline vector3_t math_vector3_rotate(vector3_t a, quaternion_t b)
{
	double xx = b.x * b.x;
	double yy = b.y * b.y;
	double zz = b.z * b.z;

	double xy = b.x * b.y;
	double xz = b.x * b.z;
	double yz = b.y * b.z;

	double wx = b.w * b.x;
	double wy = b.w * b.y;
	double wz = b.w * b.z;

	vector3_t r =
	{
		.x = (1.0 - 2.0 * (yy + zz)) * a.x +        2.0 * (xy - wz)  * a.y +        2.0 * (xz + wy)  * a.z,
		.y =        2.0 * (xy + wz)  * a.x + (1.0 - 2.0 * (xx + zz)) * a.y +        2.0 * (yz - wx)  * a.z,
		.z =        2.0 * (xz - wy)  * a.x +        2.0 * (yz + wx)  * a.y + (1.0 - 2.0 * (xx + yy)) * a.z,
	};

	return r;
}
__forceinline double math_vector3_dot(vector3_t a, vector3_t b)
{
#if defined(VEGA_AVX_SUPPORT) && defined(__AVX__) || defined(VEGA_AVX_FORCE)
	__m256d va = _mm256_set_pd(0.0, a.z, a.y, a.x);
	__m256d vb = _mm256_set_pd(0.0, b.z, b.y, b.x);

	__m256d mul = _mm256_mul_pd(va, vb);

	__m256d hadd = _mm256_hadd_pd(mul, mul);

	__m128d low = _mm256_castpd256_pd128(hadd);
	__m128d high = _mm256_extractf128_pd(hadd, 1);

	__m128d dot = _mm_add_pd(low, high);

	return ((double*)&dot)[0];
#elif defined(VEGA_SSE_SUPPORT) && defined(__SSE__) || defined(VEGA_SSE_FORCE)
	__m128d va = _mm_set_pd(a.y, a.x);
	__m128d vb = _mm_set_pd(b.y, b.x);

	__m128d mul = _mm_mul_pd(va, vb);

	__m128d dot = _mm_hadd_pd(mul, mul);

	return ((double*)&dot)[0] + (a.z * b.z);
#else
	return (a.x * b.x) + (a.y * b.y) + (a.z * b.z);
#endif // VEGA_SIMD_SUPPORT
}
__forceinline double math_vector3_length(vector3_t a)
{
	return sqrt(math_vector3_dot(a, a));
}
__forceinline double math_vector3_length2(vector3_t a)
{
	return math_vector3_dot(a, a);
}
__forceinline void math_vector3_print(vector3_t a)
{
	printf("[%f, %f, %f]\n", a.x, a.y, a.z);
}

VEGA_EXTERN_C_END

#endif // VEGA_CORE_MATH_VECTOR3_H
