#pragma once

#include "core/types.h"
#include "core/simd.h"

#include <math.h>
#include <float.h>

//
// Constants
//

#define e               2.71828182845904523536   // e
#define log2e           1.44269504088896340736   // log2(e)
#define log10e          0.434294481903251827651  // log10(e)
#define ln2             0.693147180559945309417  // ln(2)
#define ln10            2.30258509299404568402   // ln(10)
#define pi              3.14159265358979323846   // pi
#define pi_over_two     1.57079632679489661923   // pi/2
#define pi_over_four    0.785398163397448309616  // pi/4
#define one_over_pi     0.318309886183790671538  // 1/pi
#define two_over_pi     0.636619772367581343076  // 2/pi
#define two_over_sqrtpi 1.12837916709551257390   // 2/sqrt(pi)
#define sqrt2           1.41421356237309504880   // sqrt(2)
#define one_over_sqrt2  0.707106781186547524401  // 1/sqrt(2)

//
// Sizes
//

#define f32_max FLT_MAX
#define f64_max DBL_MAX

#define f32_epsilon FLT_EPSILON
#define f64_epsilon DBL_EPSILON

#define f32_min FLT_MIN
#define f64_min DBL_MIN

//
// Common utils
//

#define abs(a)    ((a) > 0 ? (a) : -(a))
#define mod(a, m) (((a) % (m)) >= 0 ? ((a) % (m)) : (((a) % (m)) + (m)))
#define square(x) ((x) * (x))

//
// Functions
//

inline f32 sqrt_f32(f32 value) {
	f32 result;

#ifdef SIMD_USE_SSE
	__m128 in = _mm_set_ss(value);
	__m128 out = _mm_sqrt_ss(in);
	result = _mm_cvtss_f32(out);
#elif defined(SIMD_USE_NEON)
	float32x4_t in = vdupq_n_f32(value);
	float32x4_t out = vsqrtq_f32(in);
	result = vgetq_lane_f32(out, 0);
#else
	result = sqrtf(value);
#endif

	return result;
}

static inline f32 invsqrt_f32(f32 value) {
	return 1.0f / sqrt_f32(value);
}

//
// Interpolation utilities
//

static inline f32 lerp_f32(f32 a, f32 t, f32 b) {
	return (1.0f - t) * a + t * b;
}

static inline f32 clamp(f32 min_val, f32 val, f32 max_val) {
	f32 result = val;

	if (result < min_val) {
		result = min_val;
	}

	if (result > max_val) {
		result = max_val;
	}

	return result;
}
