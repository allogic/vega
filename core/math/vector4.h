#ifndef VEGA_CORE_MATH_VECTOR4_H
#define VEGA_CORE_MATH_VECTOR4_H

#include <math.h>

#ifdef VEGA_AVX_SUPPORT
	#include <immintrin.h>
#elif VEGA_SSE_SUPPORT
	#include <emmintrin.h>
	#include <pmmintrin.h>
#endif // VEGA_SIMD_SUPPORT

typedef union _vector4_t
{
	struct
	{
		double x;
		double y;
		double z;
		double w;
	};
	double buffer[4];
} vector4_t;

///////////////////////////////////////////////////////////////
// Public API
///////////////////////////////////////////////////////////////

__forceinline vector4_t math_vector4_zero(void);
__forceinline vector4_t math_vector4_set(double x, double y, double z, double w);
__forceinline vector4_t math_vector4_add(vector4_t a, vector4_t b);
__forceinline vector4_t math_vector4_sub(vector4_t a, vector4_t b);
__forceinline vector4_t math_vector4_mul(vector4_t a, vector4_t b);
__forceinline vector4_t math_vector4_div(vector4_t a, vector4_t b);
__forceinline vector4_t math_vector4_add_scalar(vector4_t a, double b);
__forceinline vector4_t math_vector4_sub_scalar(vector4_t a, double b);
__forceinline vector4_t math_vector4_mul_scalar(vector4_t a, double b);
__forceinline vector4_t math_vector4_div_scalar(vector4_t a, double b);

///////////////////////////////////////////////////////////////
// Inline Definition
///////////////////////////////////////////////////////////////

__forceinline vector4_t math_vector4_zero(void)
{
	vector4_t r =
	{
		.x = 0.0,
		.y = 0.0,
		.z = 0.0,
		.w = 0.0,
	};
	return r;
}
__forceinline vector4_t math_vector4_set(double x, double y, double z, double w)
{
	vector4_t r =
	{
		.x = x,
		.y = y,
		.z = z,
		.w = w,
	};
	return r;
}
__forceinline vector4_t math_vector4_add(vector4_t a, vector4_t b)
{
#ifdef VEGA_AVX_SUPPORT
	__m256d va = _mm256_set_pd(a.w, a.z, a.y, a.x);
	__m256d vb = _mm256_set_pd(b.w, b.z, b.y, b.x);
	__m256d add = _mm256_add_pd(va, vb);
	vector4_t r =
	{
		.x = ((double*)&add)[0],
		.y = ((double*)&add)[1],
		.z = ((double*)&add)[2],
		.w = ((double*)&add)[3],
	};
	return r;
#elif VEGA_SSE_SUPPORT
	__m128d va = _mm_set_pd(a.y, a.x);
	__m128d vb = _mm_set_pd(b.y, b.x);
	__m128d vc = _mm_set_pd(a.w, a.z);
	__m128d vd = _mm_set_pd(b.w, b.z);
	__m128d add0 = _mm_add_pd(va, vb);
	__m128d add1 = _mm_add_pd(vc, vd);
	vector4_t r =
	{
		.x = ((double*)&add0)[0],
		.y = ((double*)&add0)[1],
		.z = ((double*)&add1)[0],
		.w = ((double*)&add1)[1],
	};
	return r;
#else
	vector4_t r =
	{
		.x = a.x + b.x,
		.y = a.y + b.y,
		.z = a.z + b.z,
		.w = a.w + b.w,
	};
	return r;
#endif // VEGA_SIMD_SUPPORT
}
__forceinline vector4_t math_vector4_sub(vector4_t a, vector4_t b)
{
#ifdef VEGA_AVX_SUPPORT
	__m256d va = _mm256_set_pd(a.w, a.z, a.y, a.x);
	__m256d vb = _mm256_set_pd(b.w, b.z, b.y, b.x);
	__m256d sub = _mm256_sub_pd(va, vb);
	vector4_t r =
	{
		.x = ((double*)&sub)[0],
		.y = ((double*)&sub)[1],
		.z = ((double*)&sub)[2],
		.w = ((double*)&sub)[3],
	};
	return r;
#elif VEGA_SSE_SUPPORT
	__m128d va = _mm_set_pd(a.y, a.x);
	__m128d vb = _mm_set_pd(b.y, b.x);
	__m128d vc = _mm_set_pd(a.w, a.z);
	__m128d vd = _mm_set_pd(b.w, b.z);
	__m128d sub0 = _mm_sub_pd(va, vb);
	__m128d sub1 = _mm_sub_pd(vc, vd);
	vector4_t r =
	{
		.x = ((double*)&sub0)[0],
		.y = ((double*)&sub0)[1],
		.z = ((double*)&sub1)[0],
		.w = ((double*)&sub1)[1],
	};
	return r;
#else
	vector4_t r =
	{
		.x = a.x - b.x,
		.y = a.y - b.y,
		.z = a.z - b.z,
		.w = a.w - b.w,
	};
	return r;
#endif // VEGA_SIMD_SUPPORT
}
__forceinline vector4_t math_vector4_mul(vector4_t a, vector4_t b)
{
#ifdef VEGA_AVX_SUPPORT
	__m256d va = _mm256_set_pd(a.w, a.z, a.y, a.x);
	__m256d vb = _mm256_set_pd(b.w, b.z, b.y, b.x);
	__m256d mul = _mm256_mul_pd(va, vb);
	vector4_t r =
	{
		.x = ((double*)&mul)[0],
		.y = ((double*)&mul)[1],
		.z = ((double*)&mul)[2],
		.w = ((double*)&mul)[3],
	};
	return r;
#elif VEGA_SSE_SUPPORT
	__m128d va = _mm_set_pd(a.y, a.x);
	__m128d vb = _mm_set_pd(b.y, b.x);
	__m128d vc = _mm_set_pd(a.w, a.z);
	__m128d vd = _mm_set_pd(b.w, b.z);
	__m128d mul0 = _mm_mul_pd(va, vb);
	__m128d mul1 = _mm_mul_pd(vc, vd);
	vector4_t r =
	{
		.x = ((double*)&mul0)[0],
		.y = ((double*)&mul0)[1],
		.z = ((double*)&mul1)[0],
		.w = ((double*)&mul1)[1],
	};
	return r;
#else
	vector4_t r =
	{
		.x = a.x * b.x,
		.y = a.y * b.y,
		.z = a.z * b.z,
		.w = a.w * b.w,
	};
	return r;
#endif // VEGA_SIMD_SUPPORT
}
__forceinline vector4_t math_vector4_div(vector4_t a, vector4_t b)
{
#ifdef VEGA_AVX_SUPPORT
	__m256d va = _mm256_set_pd(a.w, a.z, a.y, a.x);
	__m256d vb = _mm256_set_pd(b.w, b.z, b.y, b.x);
	__m256d div = _mm256_div_pd(va, vb);
	vector4_t r =
	{
		.x = ((double*)&div)[0],
		.y = ((double*)&div)[1],
		.z = ((double*)&div)[2],
		.w = ((double*)&div)[3],
	};
	return r;
#elif VEGA_SSE_SUPPORT
	__m128d va = _mm_set_pd(a.y, a.x);
	__m128d vb = _mm_set_pd(b.y, b.x);
	__m128d vc = _mm_set_pd(a.w, a.z);
	__m128d vd = _mm_set_pd(b.w, b.z);
	__m128d div0 = _mm_div_pd(va, vb);
	__m128d div1 = _mm_div_pd(vc, vd);
	vector4_t r =
	{
		.x = ((double*)&div0)[0],
		.y = ((double*)&div0)[1],
		.z = ((double*)&div1)[0],
		.w = ((double*)&div1)[1],
	};
	return r;
#else
	vector4_t r =
	{
		.x = a.x / b.x,
		.y = a.y / b.y,
		.z = a.z / b.z,
		.w = a.w / b.w,
	};
	return r;
#endif // VEGA_SIMD_SUPPORT
}
__forceinline vector4_t math_vector4_add_scalar(vector4_t a, double b)
{
#ifdef VEGA_AVX_SUPPORT
	__m256d va = _mm256_set_pd(a.w, a.z, a.y, a.x);
	__m256d vb = _mm256_set_pd(b, b, b, b);
	__m256d add = _mm256_add_pd(va, vb);
	vector4_t r =
	{
		.x = ((double*)&add)[0],
		.y = ((double*)&add)[1],
		.z = ((double*)&add)[2],
		.w = ((double*)&add)[3],
	};
	return r;
#elif VEGA_SSE_SUPPORT
	__m128d va = _mm_set_pd(a.y, a.x);
	__m128d vb = _mm_set_pd(b, b);
	__m128d vc = _mm_set_pd(a.w, a.z);
	__m128d add0 = _mm_add_pd(va, vb);
	__m128d add1 = _mm_add_pd(vc, vb);
	vector4_t r =
	{
		.x = ((double*)&add0)[0],
		.y = ((double*)&add0)[1],
		.z = ((double*)&add1)[0],
		.w = ((double*)&add1)[1],
	};
	return r;
#else
	vector4_t r =
	{
		.x = a.x + b,
		.y = a.y + b,
		.z = a.z + b,
		.w = a.w + b,
	};
	return r;
#endif // VEGA_SIMD_SUPPORT
}
__forceinline vector4_t math_vector4_sub_scalar(vector4_t a, double b)
{
#ifdef VEGA_AVX_SUPPORT
	__m256d va = _mm256_set_pd(a.w, a.z, a.y, a.x);
	__m256d vb = _mm256_set_pd(b, b, b, b);
	__m256d sub = _mm256_sub_pd(va, vb);
	vector4_t r =
	{
		.x = ((double*)&sub)[0],
		.y = ((double*)&sub)[1],
		.z = ((double*)&sub)[2],
		.w = ((double*)&sub)[3],
	};
	return r;
#elif VEGA_SSE_SUPPORT
	__m128d va = _mm_set_pd(a.y, a.x);
	__m128d vb = _mm_set_pd(b, b);
	__m128d vc = _mm_set_pd(a.w, a.z);
	__m128d sub0 = _mm_sub_pd(va, vb);
	__m128d sub1 = _mm_sub_pd(vc, vb);
	vector4_t r =
	{
		.x = ((double*)&sub0)[0],
		.y = ((double*)&sub0)[1],
		.z = ((double*)&sub1)[0],
		.w = ((double*)&sub1)[1],
	};
	return r;
#else
	vector4_t r =
	{
		.x = a.x - b,
		.y = a.y - b,
		.z = a.z - b,
		.w = a.w - b,
	};
	return r;
#endif // VEGA_SIMD_SUPPORT
}
__forceinline vector4_t math_vector4_mul_scalar(vector4_t a, double b)
{
#ifdef VEGA_AVX_SUPPORT
	__m256d va = _mm256_set_pd(a.w, a.z, a.y, a.x);
	__m256d vb = _mm256_set_pd(b, b, b, b);
	__m256d mul = _mm256_mul_pd(va, vb);
	vector4_t r =
	{
		.x = ((double*)&mul)[0],
		.y = ((double*)&mul)[1],
		.z = ((double*)&mul)[2],
		.w = ((double*)&mul)[3],
	};
	return r;
#elif VEGA_SSE_SUPPORT
	__m128d va = _mm_set_pd(a.y, a.x);
	__m128d vb = _mm_set_pd(b, b);
	__m128d vc = _mm_set_pd(a.w, a.z);
	__m128d mul0 = _mm_mul_pd(va, vb);
	__m128d mul1 = _mm_mul_pd(vc, vb);
	vector4_t r =
	{
		.x = ((double*)&mul0)[0],
		.y = ((double*)&mul0)[1],
		.z = ((double*)&mul1)[0],
		.w = ((double*)&mul1)[1],
	};
	return r;
#else
	vector4_t r =
	{
		.x = a.x * b,
		.y = a.y * b,
		.z = a.z * b,
		.w = a.w * b,
	};
	return r;
#endif // VEGA_SIMD_SUPPORT
}
__forceinline vector4_t math_vector4_div_scalar(vector4_t a, double b)
{
#ifdef VEGA_AVX_SUPPORT
	__m256d va = _mm256_set_pd(a.w, a.z, a.y, a.x);
	__m256d vb = _mm256_set_pd(b, b, b, b);
	__m256d div = _mm256_div_pd(va, vb);
	vector4_t r =
	{
		.x = ((double*)&div)[0],
		.y = ((double*)&div)[1],
		.z = ((double*)&div)[2],
		.w = ((double*)&div)[3],
	};
	return r;
#elif VEGA_SSE_SUPPORT
	__m128d va = _mm_set_pd(a.y, a.x);
	__m128d vb = _mm_set_pd(b, b);
	__m128d vc = _mm_set_pd(a.w, a.z);
	__m128d div0 = _mm_div_pd(va, vb);
	__m128d div1 = _mm_div_pd(vc, vb);
	vector4_t r =
	{
		.x = ((double*)&div0)[0],
		.y = ((double*)&div0)[1],
		.z = ((double*)&div1)[0],
		.w = ((double*)&div1)[1],
	};
	return r;
#else
	vector4_t r =
	{
		.x = a.x / b,
		.y = a.y / b,
		.z = a.z / b,
		.w = a.w / b,
	};
	return r;
#endif // VEGA_SIMD_SUPPORT
}

#endif // VEGA_CORE_MATH_VECTOR4_H
