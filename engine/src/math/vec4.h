#pragma once

#include "math/types.h"

static inline Vec4 vec4(f32 x, f32 y, f32 z, f32 w) {
	Vec4 vec;
#ifdef SIMD_USE_SSE
	vec.sse = _mm_setr_ps(x, y, z, w);
#elif defined(SIMD_USE_NEON)
	vec.neon = (float32x4_t){x, y, z, w};
#else
	vec = (Vec4){x, y, z, w};
#endif
	return vec;
}

static inline Vec4 vec4_from_vec2(Vec2 v, f32 z, f32 w) {
	return vec4(v.x, v.y, z, w);
}

static inline Vec4 vec4_from_vec3(Vec3 v, f32 w) {
	return vec4(v.x, v.y, v.z, w);
}

static inline Vec4 add_vec4(Vec4 left, Vec4 right) {
	Vec4 result;

#ifdef SIMD_USE_SSE
	result.sse = _mm_add_ps(left.sse, right.sse);
#elif defined(SIMD_USE_NEON)
	result.neon = vaddq_f32(left.neon, right.neon);
#else
	result.x = left.x + right.x;
	result.y = left.y + right.y;
	result.z = left.z + right.z;
	result.w = left.w + right.w;
#endif

	return result;
}

static inline Vec4 sub_vec4(Vec4 left, Vec4 right) {
	Vec4 result;

#ifdef SIMD_USE_SSE
	result.sse = _mm_sub_ps(left.sse, right.sse);
#elif defined(SIMD_USE_NEON)
	result.neon = vsubq_f32(left.neon, right.neon);
#else
	result.x = left.x - right.x;
	result.y = left.y - right.y;
	result.z = left.z - right.z;
	result.w = left.w - right.w;
#endif

	return result;
}

static inline Vec4 mul_vec4(Vec4 left, Vec4 right) {
	Vec4 result;

#ifdef SIMD_USE_SSE
	result.sse = _mm_mul_ps(left.sse, right.sse);
#elif defined(SIMD_USE_NEON)
	result.neon = vmulq_f32(left.neon, right.neon);
#else
	result.x = left.x * right.x;
	result.y = left.y * right.y;
	result.z = left.z * right.z;
	result.w = left.w * right.w;
#endif

	return result;
}

static inline Vec4 mul_vec4_f32(Vec4 left, f32 right) {
	Vec4 result;

#ifdef SIMD_USE_SSE
	__m128 scalar = _mm_set1_ps(right);
	result.sse = _mm_mul_ps(left.sse, scalar);
#elif defined(SIMD_USE_NEON)
	result.neon = vmulq_n_f32(left.neon, right);
#else
	result.x = left.x * right;
	result.y = left.y * right;
	result.z = left.z * right;
	result.w = left.w * right;
#endif

	return result;
}

static inline Vec4 div_vec4(Vec4 left, Vec4 right) {
	Vec4 result;

#ifdef SIMD_USE_SSE
	result.sse = _mm_div_ps(left.sse, right.sse);
#elif defined(SIMD_USE_NEON)
	result.neon = vdivq_f32(left.neon, right.neon);
#else
	result.x = left.x / right.x;
	result.y = left.y / right.y;
	result.z = left.z / right.z;
	result.w = left.w / right.w;
#endif

	return result;
}

static inline Vec4 div_vec4_f32(Vec4 left, f32 right) {
	Vec4 result;

#ifdef SIMD_USE_SSE
	__m128 scalar = _mm_set1_ps(right);
	result.sse = _mm_div_ps(left.sse, scalar);
#elif defined(SIMD_USE_NEON)
	float32x4_t scalar = vdupq_n_f32(right);
	result.neon = vdivq_f32(left.neon, scalar);
#else
	result.x = left.x / right;
	result.y = left.y / right;
	result.z = left.z / right;
	result.w = left.w / right;
#endif

	return result;
}

static inline b8 eq_vec4(Vec4 left, Vec4 right) {
	return left.x == right.x && left.y == right.y && left.z == right.z && left.w == right.w;
}

static inline f32 dot_vec4(Vec4 left, Vec4 right) {
	f32 result;

#ifdef SIMD_USE_SSE
	__m128 see_res_one = _mm_mul_ps(left.sse, right.sse);
	__m128 sse_res_two = _mm_shuffle_ps(see_res_one, see_res_one, _MM_SHUFFLE(2, 3, 0, 1));
	see_res_one = _mm_add_ps(see_res_one, sse_res_two);
	sse_res_two = _mm_shuffle_ps(see_res_one, see_res_one, _MM_SHUFFLE(0, 1, 2, 3));
	see_res_one = _mm_add_ps(see_res_one, sse_res_two);
	_mm_store_ss(&result, see_res_one);
#elif defined(SIMD_USE_NEON)
	float32x4_t NEONMultiplyResult = vmulq_f32(left.neon, right.neon);
	float32x4_t NEONHalfAdd = vpaddq_f32(NEONMultiplyResult, NEONMultiplyResult);
	float32x4_t NEONFullAdd = vpaddq_f32(NEONHalfAdd, NEONHalfAdd);
	result = vgetq_lane_f32(NEONFullAdd, 0);
#else
	result = ((left.x * right.x) + (left.z * right.z)) + ((left.y * right.y) + (left.w * right.w));
#endif

	return result;
}

static inline Vec3 cross(Vec3 left, Vec3 right) {
	Vec3 result;
	result.x = (left.y * right.z) - (left.z * right.y);
	result.y = (left.z * right.x) - (left.x * right.z);
	result.z = (left.x * right.y) - (left.y * right.x);
	return result;
}

static inline f32 len_sqr_vec4(Vec4 a) {
	return dot_vec4(a, a);
}

static inline f32 len_vec4(Vec4 a) {
	return sqrtf(len_sqr_vec4(a));
}

static inline Vec4 norm_vec4(Vec4 a) {
	return mul_vec4_f32(a, invsqrtf(dot_vec4(a, a)));
}

static inline Vec4 lerp_vec4(Vec4 a, f32 t, Vec4 b) {
	return add_vec4(mul_vec4_f32(a, 1.0f - t), mul_vec4_f32(b, t));
}
