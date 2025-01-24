#ifndef VEGA_CORE_MATH_VECTOR3_H
#define VEGA_CORE_MATH_VECTOR3_H

#include <math.h>

#ifdef VEGA_AVX_SUPPORT
	#include <immintrin.h>
#elif VEGA_SSE_SUPPORT
	#include <emmintrin.h>
	#include <pmmintrin.h>
#endif // VEGA_SIMD_SUPPORT

typedef union _vector3_t
{
	struct
	{
		double x;
		double y;
		double z;
	};
	double buffer[3];
} vector3_t;

///////////////////////////////////////////////////////////////
// Public API
///////////////////////////////////////////////////////////////

__forceinline vector3_t math_vector3_zero(void);
__forceinline vector3_t math_vector3_set(double x, double y, double z);
__forceinline vector3_t math_vector3_add(vector3_t a, vector3_t b);
__forceinline vector3_t math_vector3_sub(vector3_t a, vector3_t b);
__forceinline vector3_t math_vector3_mul(vector3_t a, vector3_t b);
__forceinline vector3_t math_vector3_div(vector3_t a, vector3_t b);
__forceinline vector3_t math_vector3_add_scalar(vector3_t a, double b);
__forceinline vector3_t math_vector3_sub_scalar(vector3_t a, double b);
__forceinline vector3_t math_vector3_mul_scalar(vector3_t a, double b);
__forceinline vector3_t math_vector3_div_scalar(vector3_t a, double b);
__forceinline vector3_t math_vector3_norm(vector3_t a);
__forceinline double math_vector3_dot(vector3_t a, vector3_t b);
__forceinline double math_vector3_length(vector3_t a);
__forceinline double math_vector3_length2(vector3_t a);
__forceinline vector3_t math_vector3_cross(vector3_t a, vector3_t b);
//__forceinline vector3_t math_vector3_rotate(vector3_t a, quaternion b); // TODO

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
__forceinline vector3_t math_vector3_set(double x, double y, double z)
{
	vector3_t r =
	{
		.x = x,
		.y = y,
		.z = z,
	};
	return r;
}
__forceinline vector3_t math_vector3_add(vector3_t a, vector3_t b)
{
#ifdef VEGA_AVX_SUPPORT
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
#elif VEGA_SSE_SUPPORT
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
#ifdef VEGA_AVX_SUPPORT
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
#elif VEGA_SSE_SUPPORT
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
#ifdef VEGA_AVX_SUPPORT
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
#elif VEGA_SSE_SUPPORT
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
#ifdef VEGA_AVX_SUPPORT
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
#elif VEGA_SSE_SUPPORT
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
#ifdef VEGA_AVX_SUPPORT
	__m256d va = _mm256_set_pd(0.0, a.z, a.y, a.x);
	__m256d vb = _mm256_set_pd(0.0, b, b, b);
	__m256d add = _mm256_add_pd(va, vb);
	vector3_t r =
	{
		.x = ((double*)&add)[0],
		.y = ((double*)&add)[1],
		.z = ((double*)&add)[2],
	};
	return r;
#elif VEGA_SSE_SUPPORT
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
#ifdef VEGA_AVX_SUPPORT
	__m256d va = _mm256_set_pd(0.0, a.z, a.y, a.x);
	__m256d vb = _mm256_set_pd(0.0, b, b, b);
	__m256d sub = _mm256_sub_pd(va, vb);
	vector3_t r =
	{
		.x = ((double*)&sub)[0],
		.y = ((double*)&sub)[1],
		.z = ((double*)&sub)[2],
	};
	return r;
#elif VEGA_SSE_SUPPORT
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
#ifdef VEGA_AVX_SUPPORT
	__m256d va = _mm256_set_pd(0.0, a.z, a.y, a.x);
	__m256d vb = _mm256_set_pd(0.0, b, b, b);
	__m256d mul = _mm256_mul_pd(va, vb);
	vector3_t r =
	{
		.x = ((double*)&mul)[0],
		.y = ((double*)&mul)[1],
		.z = ((double*)&mul)[2],
	};
	return r;
#elif VEGA_SSE_SUPPORT
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
#ifdef VEGA_AVX_SUPPORT
	__m256d va = _mm256_set_pd(0.0, a.z, a.y, a.x);
	__m256d vb = _mm256_set_pd(0.0, b, b, b);
	__m256d div = _mm256_div_pd(va, vb);
	vector3_t r =
	{
		.x = ((double*)&div)[0],
		.y = ((double*)&div)[1],
		.z = ((double*)&div)[2],
	};
	return r;
#elif VEGA_SSE_SUPPORT
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
#ifdef VEGA_AVX_SUPPORT
	// TODO
#elif VEGA_SSE_SUPPORT
	// TODO
#else
	// TODO
#endif // VEGA_SIMD_SUPPORT
}
__forceinline double math_vector3_dot(vector3_t a, vector3_t b)
{
#ifdef VEGA_AVX_SUPPORT
	__m256d va = _mm256_set_pd(0.0, a.z, a.y, a.x);
	__m256d vb = _mm256_set_pd(0.0, b.z, b.y, b.x);
	__m256d mul = _mm256_mul_pd(va, vb);
	__m256d hadd = _mm256_hadd_pd(mul, mul);
	__m128d low = _mm256_castpd256_pd128(hadd);
	__m128d high = _mm256_extractf128_pd(hadd, 1);
	__m128d dot = _mm_add_pd(low, high);
	return ((double*)&dot)[0];
#elif VEGA_SSE_SUPPORT
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
__forceinline vector3_t math_vector3_cross(vector3_t a, vector3_t b)
{
#ifdef VEGA_AVX_SUPPORT
	// TODO
#elif VEGA_SSE_SUPPORT
	// TODO
#else
	// TODO
#endif // VEGA_SIMD_SUPPORT
}

#endif // VEGA_CORE_MATH_VECTOR3_H
