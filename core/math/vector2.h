#ifndef VEGA_CORE_MATH_VECTOR2_H
#define VEGA_CORE_MATH_VECTOR2_H

#include <math.h>

#if defined(VEGA_AVX_SUPPORT) && defined(__AVX__) || defined(VEGA_AVX_FORCE)
	#include <immintrin.h>
#elif defined(VEGA_SSE_SUPPORT) && defined(__SSE__) || defined(VEGA_SSE_FORCE)
	#include <emmintrin.h>
	#include <pmmintrin.h>
#endif // VEGA_SIMD_SUPPORT

#include <vega/core/math/constants.h>
#include <vega/core/math/forward.h>

///////////////////////////////////////////////////////////////
// Inline Definition
///////////////////////////////////////////////////////////////

__forceinline vector2_t math_vector2_zero(void)
{
	vector2_t r =
	{
		.x = 0.0,
		.y = 0.0,
	};

	return r;
}
__forceinline vector2_t math_vector2_from_xy(double x, double y)
{
	vector2_t r =
	{
		.x = x,
		.y = y,
	};

	return r;
}
__forceinline vector2_t math_vector2_add(vector2_t a, vector2_t b)
{
#if defined(VEGA_AVX_SUPPORT) && defined(__AVX__) || defined(VEGA_AVX_FORCE)
	__m256d va = _mm256_set_pd(0.0, 0.0, a.y, a.x);
	__m256d vb = _mm256_set_pd(0.0, 0.0, b.y, b.x);

	__m256d add = _mm256_add_pd(va, vb);

	vector2_t r =
	{
		.x = ((double*)&add)[0],
		.y = ((double*)&add)[1],
	};

	return r;
#elif defined(VEGA_SSE_SUPPORT) && defined(__SSE__) || defined(VEGA_SSE_FORCE)
	__m128d va = _mm_set_pd(a.y, a.x);
	__m128d vb = _mm_set_pd(b.y, b.x);

	__m128d add = _mm_add_pd(va, vb);

	vector2_t r =
	{
		.x = ((double*)&add)[0],
		.y = ((double*)&add)[1],
	};

	return r;
#else
	vector2_t r =
	{
		.x = a.x + b.x,
		.y = a.y + b.y,
	};

	return r;
#endif // VEGA_SIMD_SUPPORT
}
__forceinline vector2_t math_vector2_sub(vector2_t a, vector2_t b)
{
#if defined(VEGA_AVX_SUPPORT) && defined(__AVX__) || defined(VEGA_AVX_FORCE)
	__m256d va = _mm256_set_pd(0.0, 0.0, a.y, a.x);
	__m256d vb = _mm256_set_pd(0.0, 0.0, b.y, b.x);

	__m256d sub = _mm256_sub_pd(va, vb);

	vector2_t r =
	{
		.x = ((double*)&sub)[0],
		.y = ((double*)&sub)[1],
	};

	return r;
#elif defined(VEGA_SSE_SUPPORT) && defined(__SSE__) || defined(VEGA_SSE_FORCE)
	__m128d va = _mm_set_pd(a.y, a.x);
	__m128d vb = _mm_set_pd(b.y, b.x);

	__m128d sub = _mm_sub_pd(va, vb);

	vector2_t r =
	{
		.x = ((double*)&sub)[0],
		.y = ((double*)&sub)[1],
	};

	return r;
#else
	vector2_t r =
	{
		.x = a.x - b.x,
		.y = a.y - b.y,
	};

	return r;
#endif // VEGA_SIMD_SUPPORT
}
__forceinline vector2_t math_vector2_mul(vector2_t a, vector2_t b)
{
#if defined(VEGA_AVX_SUPPORT) && defined(__AVX__) || defined(VEGA_AVX_FORCE)
	__m256d va = _mm256_set_pd(0.0, 0.0, a.y, a.x);
	__m256d vb = _mm256_set_pd(0.0, 0.0, b.y, b.x);

	__m256d mul = _mm256_mul_pd(va, vb);

	vector2_t r =
	{
		.x = ((double*)&mul)[0],
		.y = ((double*)&mul)[1],
	};

	return r;
#elif defined(VEGA_SSE_SUPPORT) && defined(__SSE__) || defined(VEGA_SSE_FORCE)
	__m128d va = _mm_set_pd(a.y, a.x);
	__m128d vb = _mm_set_pd(b.y, b.x);

	__m128d mul = _mm_mul_pd(va, vb);

	vector2_t r =
	{
		.x = ((double*)&mul)[0],
		.y = ((double*)&mul)[1],
	};

	return r;
#else
	vector2_t r =
	{
		.x = a.x * b.x,
		.y = a.y * b.y,
	};

	return r;
#endif // VEGA_SIMD_SUPPORT
}
__forceinline vector2_t math_vector2_div(vector2_t a, vector2_t b)
{
#if defined(VEGA_AVX_SUPPORT) && defined(__AVX__) || defined(VEGA_AVX_FORCE)
	__m256d va = _mm256_set_pd(0.0, 0.0, a.y, a.x);
	__m256d vb = _mm256_set_pd(0.0, 0.0, b.y, b.x);

	__m256d div = _mm256_div_pd(va, vb);

	vector2_t r =
	{
		.x = ((double*)&div)[0],
		.y = ((double*)&div)[1],
	};

	return r;
#elif defined(VEGA_SSE_SUPPORT) && defined(__SSE__) || defined(VEGA_SSE_FORCE)
	__m128d va = _mm_set_pd(a.y, a.x);
	__m128d vb = _mm_set_pd(b.y, b.x);

	__m128d div = _mm_div_pd(va, vb);

	vector2_t r =
	{
		.x = ((double*)&div)[0],
		.y = ((double*)&div)[1],
	};

	return r;
#else
	vector2_t r =
	{
		.x = a.x / b.x,
		.y = a.y / b.y,
	};

	return r;
#endif // VEGA_SIMD_SUPPORT
}
__forceinline vector2_t math_vector2_add_scalar(vector2_t a, double b)
{
#if defined(VEGA_AVX_SUPPORT) && defined(__AVX__) || defined(VEGA_AVX_FORCE)
	__m256d va = _mm256_set_pd(0.0, 0.0, a.y, a.x);
	__m256d vb = _mm256_set1_pd(b);

	__m256d add = _mm256_add_pd(va, vb);

	vector2_t r =
	{
		.x = ((double*)&add)[0],
		.y = ((double*)&add)[1],
	};

	return r;
#elif defined(VEGA_SSE_SUPPORT) && defined(__SSE__) || defined(VEGA_SSE_FORCE)
	__m128d va = _mm_set_pd(a.y, a.x);
	__m128d vb = _mm_set_pd(b, b);

	__m128d add = _mm_add_pd(va, vb);

	vector2_t r =
	{
		.x = ((double*)&add)[0],
		.y = ((double*)&add)[1],
	};

	return r;
#else
	vector2_t r =
	{
		.x = a.x + b,
		.y = a.y + b,
	};

	return r;
#endif // VEGA_SIMD_SUPPORT
}
__forceinline vector2_t math_vector2_sub_scalar(vector2_t a, double b)
{
#if defined(VEGA_AVX_SUPPORT) && defined(__AVX__) || defined(VEGA_AVX_FORCE)
	__m256d va = _mm256_set_pd(0.0, 0.0, a.y, a.x);
	__m256d vb = _mm256_set1_pd(b);

	__m256d sub = _mm256_sub_pd(va, vb);

	vector2_t r =
	{
		.x = ((double*)&sub)[0],
		.y = ((double*)&sub)[1],
	};

	return r;
#elif defined(VEGA_SSE_SUPPORT) && defined(__SSE__) || defined(VEGA_SSE_FORCE)
	__m128d va = _mm_set_pd(a.y, a.x);
	__m128d vb = _mm_set_pd(b, b);

	__m128d sub = _mm_sub_pd(va, vb);

	vector2_t r =
	{
		.x = ((double*)&sub)[0],
		.y = ((double*)&sub)[1],
	};

	return r;
#else
	vector2_t r =
	{
		.x = a.x - b,
		.y = a.y - b,
	};

	return r;
#endif // VEGA_SIMD_SUPPORT
}
__forceinline vector2_t math_vector2_mul_scalar(vector2_t a, double b)
{
#if defined(VEGA_AVX_SUPPORT) && defined(__AVX__) || defined(VEGA_AVX_FORCE)
	__m256d va = _mm256_set_pd(0.0, 0.0, a.y, a.x);
	__m256d vb = _mm256_set1_pd(b);

	__m256d mul = _mm256_mul_pd(va, vb);

	vector2_t r =
	{
		.x = ((double*)&mul)[0],
		.y = ((double*)&mul)[1],
	};

	return r;
#elif defined(VEGA_SSE_SUPPORT) && defined(__SSE__) || defined(VEGA_SSE_FORCE)
	__m128d va = _mm_set_pd(a.y, a.x);
	__m128d vb = _mm_set_pd(b, b);

	__m128d mul = _mm_mul_pd(va, vb);

	vector2_t r =
	{
		.x = ((double*)&mul)[0],
		.y = ((double*)&mul)[1],
	};

	return r;
#else
	vector2_t r =
	{
		.x = a.x * b,
		.y = a.y * b,
	};

	return r;
#endif // VEGA_SIMD_SUPPORT
}
__forceinline vector2_t math_vector2_div_scalar(vector2_t a, double b)
{
#if defined(VEGA_AVX_SUPPORT) && defined(__AVX__) || defined(VEGA_AVX_FORCE)
	__m256d va = _mm256_set_pd(0.0, 0.0, a.y, a.x);
	__m256d vb = _mm256_set1_pd(b);

	__m256d div = _mm256_div_pd(va, vb);

	vector2_t r =
	{
		.x = ((double*)&div)[0],
		.y = ((double*)&div)[1],
	};

	return r;
#elif defined(VEGA_SSE_SUPPORT) && defined(__SSE__) || defined(VEGA_SSE_FORCE)
	__m128d va = _mm_set_pd(a.y, a.x);
	__m128d vb = _mm_set_pd(b, b);

	__m128d div = _mm_div_pd(va, vb);

	vector2_t r =
	{
		.x = ((double*)&div)[0],
		.y = ((double*)&div)[1],
	};

	return r;
#else
	vector2_t r =
	{
		.x = a.x / b,
		.y = a.y / b,
	};

	return r;
#endif // VEGA_SIMD_SUPPORT
}
__forceinline vector2_t math_vector2_norm(vector2_t a)
{
	return math_vector2_mul_scalar(a, 1.0 / math_vector2_length(a));
}
__forceinline double math_vector2_dot(vector2_t a, vector2_t b)
{
#if defined(VEGA_AVX_SUPPORT) && defined(__AVX__) || defined(VEGA_AVX_FORCE)
	__m256d va = _mm256_set_pd(0.0, 0.0, a.y, a.x);
	__m256d vb = _mm256_set_pd(0.0, 0.0, b.y, b.x);

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

	return ((double*)&dot)[0];
#else
	return (a.x * b.x) + (a.y * b.y);
#endif // VEGA_SIMD_SUPPORT
}
__forceinline double math_vector2_length(vector2_t a)
{
	return sqrt(math_vector2_dot(a, a));
}
__forceinline double math_vector2_length2(vector2_t a)
{
	return math_vector2_dot(a, a);
}

#endif // VEGA_CORE_MATH_VECTOR2_H
