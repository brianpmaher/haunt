#pragma once

#include "math/types.h"

static inline Quat quat(f32 x, f32 y, f32 z, f32 w) {
	Quat result;

#ifdef SIMD_USE_SSE
	result.sse = _mm_setr_ps(x, y, z, w);
#elif defined(SIMD_USE_NEON)
	float32x4_t v = { x, y, z, w };
	result.neon = v;
#else
	result.x = x;
	result.y = y;
	result.z = z;
	result.w = w;
#endif

	return result;
}

static inline Quat quat_from_vec4(Vec4 v) {
	Quat result;

#ifdef SIMD_USE_SSE
	result.sse = v.sse;
#elif defined(SIMD_USE_NEON)
	result.neon = v.neon;
#else
	result.x = v.x;
	result.y = v.y;
	result.z = v.z;
	result.w = v.w;
#endif

	return result;
}

static inline Quat add_quat(Quat left, Quat right) {
	Quat result;

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

static inline Quat sub_quat(Quat left, Quat right) {
	Quat result;

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

static inline Quat mul_quat(Quat left, Quat right) {
	Quat result;

#ifdef SIMD_USE_SSE
	__m128 sse_res_one = _mm_xor_ps(_mm_shuffle_ps(left.sse, left.sse, _MM_SHUFFLE(0, 0, 0, 0)), _mm_setr_ps(0.f, -0.f, 0.f, -0.f));
	__m128 sse_res_two = _mm_shuffle_ps(right.sse, right.sse, _MM_SHUFFLE(0, 1, 2, 3));
	__m128 sse_res_three = _mm_mul_ps(sse_res_two, sse_res_one);

	sse_res_one = _mm_xor_ps(_mm_shuffle_ps(left.sse, left.sse, _MM_SHUFFLE(1, 1, 1, 1)) , _mm_setr_ps(0.f, 0.f, -0.f, -0.f));
	sse_res_two = _mm_shuffle_ps(right.sse, right.sse, _MM_SHUFFLE(1, 0, 3, 2));
	sse_res_three = _mm_add_ps(sse_res_three, _mm_mul_ps(sse_res_two, sse_res_one));

	sse_res_one = _mm_xor_ps(_mm_shuffle_ps(left.sse, left.sse, _MM_SHUFFLE(2, 2, 2, 2)), _mm_setr_ps(-0.f, 0.f, 0.f, -0.f));
	sse_res_two = _mm_shuffle_ps(right.sse, right.sse, _MM_SHUFFLE(2, 3, 0, 1));
	sse_res_three = _mm_add_ps(sse_res_three, _mm_mul_ps(sse_res_two, sse_res_one));

	sse_res_one = _mm_shuffle_ps(left.sse, left.sse, _MM_SHUFFLE(3, 3, 3, 3));
	sse_res_two = _mm_shuffle_ps(right.sse, right.sse, _MM_SHUFFLE(3, 2, 1, 0));
	result.sse = _mm_add_ps(sse_res_three, _mm_mul_ps(sse_res_two, sse_res_one));
#elif defined(SIMD_USE_NEON)
	float32x4_t right_1032 = vrev64q_f32(right.neon);
	float32x4_t right_3210 = vcombine_f32(vget_high_f32(right_1032), vget_low_f32(right_1032));
	float32x4_t right_2301 = vrev64q_f32(right_3210);

	float32x4_t first_sign = {1.0f, -1.0f, 1.0f, -1.0f};
	result.neon = vmulq_f32(right_3210, vmulq_f32(vdupq_laneq_f32(left.neon, 0), first_sign));
	float32x4_t second_sign = {1.0f, 1.0f, -1.0f, -1.0f};
	result.neon = vfmaq_f32(result.neon, right_2301, vmulq_f32(vdupq_laneq_f32(left.neon, 1), second_sign));
	float32x4_t third_sign = {-1.0f, 1.0f, 1.0f, -1.0f};
	result.neon = vfmaq_f32(result.neon, right_1032, vmulq_f32(vdupq_laneq_f32(left.neon, 2), third_sign));
	result.neon = vfmaq_laneq_f32(result.neon, right.neon, left.neon, 3);

#else
	result.x =  right.elements[3] * +left.elements[0];
	result.y =  right.elements[2] * -left.elements[0];
	result.z =  right.elements[1] * +left.elements[0];
	result.w =  right.elements[0] * -left.elements[0];

	result.x += right.elements[2] * +left.elements[1];
	result.y += right.elements[3] * +left.elements[1];
	result.z += right.elements[0] * -left.elements[1];
	result.w += right.elements[1] * -left.elements[1];

	result.x += right.elements[1] * -left.elements[2];
	result.y += right.elements[0] * +left.elements[2];
	result.z += right.elements[3] * +left.elements[2];
	result.w += right.elements[2] * -left.elements[2];

	result.x += right.elements[0] * +left.elements[3];
	result.y += right.elements[1] * +left.elements[3];
	result.z += right.elements[2] * +left.elements[3];
	result.w += right.elements[3] * +left.elements[3];
#endif

	return result;
}

static inline Quat mul_quat_f32(Quat left, f32 multiplicative) {
	Quat result;

#ifdef SIMD_USE_SSE
	__m128 scalar = _mm_set1_ps(multiplicative);
	result.sse = _mm_mul_ps(left.sse, scalar);
#elif defined(SIMD_USE_NEON)
	result.neon = vmulq_n_f32(left.neon, multiplicative);
#else
	result.x = left.x * multiplicative;
	result.y = left.y * multiplicative;
	result.z = left.z * multiplicative;
	result.w = left.w * multiplicative;
#endif

	return result;
}

static inline Quat div_quat_f32(Quat left, f32 dividend) {
	Quat result;

#ifdef SIMD_USE_SSE
	__m128 scalar = _mm_set1_ps(dividend);
	result.sse = _mm_div_ps(left.sse, scalar);
#elif defined(SIMD_USE_NEON)
	float32x4_t scalar = vdupq_n_f32(dividend);
	result.neon = vdivq_f32(left.neon, scalar);
#else
	result.x = left.x / dividend;
	result.y = left.y / dividend;
	result.z = left.z / dividend;
	result.w = left.w / dividend;
#endif

	return result;
}

static inline f32 dot_quat(Quat left, Quat right) {
	f32 result;

#ifdef SIMD_USE_SSE
	__m128 sse_res_one = _mm_mul_ps(left.sse, right.sse);
	__m128 sse_res_two = _mm_shuffle_ps(sse_res_one, sse_res_one, _MM_SHUFFLE(2, 3, 0, 1));
	sse_res_one = _mm_add_ps(sse_res_one, sse_res_two);
	sse_res_two = _mm_shuffle_ps(sse_res_one, sse_res_one, _MM_SHUFFLE(0, 1, 2, 3));
	sse_res_one = _mm_add_ps(sse_res_one, sse_res_two);
	_mm_store_ss(&result, sse_res_one);
#elif defined(SIMD_USE_NEON)
	float32x4_t neon_multiply_result = vmulq_f32(left.neon, right.neon);
	float32x4_t neon_half_add = vpaddq_f32(neon_multiply_result, neon_multiply_result);
	float32x4_t neon_full_add = vpaddq_f32(neon_half_add, neon_half_add);
	result = vgetq_lane_f32(neon_full_add, 0);
#else
	result = ((left.x * right.x) + (left.z * right.z)) + ((left.y * right.y) + (left.w * right.w));
#endif

	return result;
}

static inline Quat inv_quat(Quat left) {
	Quat result;

	result.x = -left.x;
	result.y = -left.y;
	result.z = -left.z;
	result.w = left.w;

	return div_quat_f32(result, (dot_quat(left, left)));
}

static inline Quat norm_quat(Quat q) {
	Vec4 vec = norm_vec4(vec4(q.x, q.y, q.z, q.w));
	Quat result = quat(vec.x, vec.y, vec.z, vec.w);
	return result;
}

static inline Quat _mix_quat(Quat left, f32 mix_left, Quat right, f32 mix_right) {
	Quat result;

#ifdef SIMD_USE_SSE
	__m128 scalar_left = _mm_set1_ps(mix_left);
	__m128 scalar_right = _mm_set1_ps(mix_right);
	__m128 sse_res_one = _mm_mul_ps(left.sse, scalar_left);
	__m128 sse_res_two = _mm_mul_ps(right.sse, scalar_right);
	result.sse = _mm_add_ps(sse_res_one, sse_res_two);
#elif defined(SIMD_USE_NEON)
	float32x4_t scaled_left = vmulq_n_f32(left.neon, mix_left);
	float32x4_t scaled_right = vmulq_n_f32(right.neon, mix_right);
	result.neon = vaddq_f32(scaled_left, scaled_right);
#else
	result.x = left.x*mix_left + right.x*mix_right;
	result.y = left.y*mix_left + right.y*mix_right;
	result.z = left.z*mix_left + right.z*mix_right;
	result.w = left.w*mix_left + right.w*mix_right;
#endif

	return result;
}

static inline Quat norm_lerp(Quat left, f32 t, Quat right) {
	Quat result = _mix_quat(left, 1.0f-t, right, t);
	result = norm_quat(result);
	return result;
}

static inline Quat slerp(Quat left, f32 t, Quat right) {
	Quat result;

	f32 cos_theta = dot_quat(left, right);

	if (cos_theta < 0.0f) { /* Take shortest path on Hyper-sphere */
		cos_theta = -cos_theta;
		right = quat(-right.x, -right.y, -right.z, -right.w);
	}

	/* Use Normalized Linear interpolation when vecs are roughly not L.I. */
	if (cos_theta > 0.9995f) {
		result = norm_lerp(left, t, right);
	} else {
		f32 angle = acosf(cos_theta);
		f32 mix_left = sinf((1.0f - t) * angle);
		f32 mix_right = sinf(t * angle);

		result = _mix_quat(left, mix_left, right, mix_right);
		result = norm_quat(result);
	}

	return result;
}

// This method taken from Mike Day at Insomniac Games.
// https://d3cw3dd2w32x2b.cloudfront.net/wp-content/uploads/2015/01/matrix-to-quat.pdf
//
// Note that as mentioned at the top of the paper, the paper assumes the matrix
// would be post-multiplied to a vector to rotate it, meaning the matrix is
// the transpose of what we're dealing with. But, because our matrices are
// stored in column-major order, the indices appear to match the paper.
//
// For example, m12 in the paper is row 1, column 2. We need to transpose it to
// row 2, column 1. But, because the column comes first when referencing
// elements, it looks like mat.elements[1][2].
static inline Quat quat_from_mat4_rh(Mat4 mat) {
	f32 t;
	Quat q;

	if (mat.elements[2][2] < 0.0f) {
		if (mat.elements[0][0] > mat.elements[1][1]) {
			t = 1 + mat.elements[0][0] - mat.elements[1][1] - mat.elements[2][2];
			q = quat(
				t,
				mat.elements[0][1] + mat.elements[1][0],
				mat.elements[2][0] + mat.elements[0][2],
				mat.elements[1][2] - mat.elements[2][1]
			);
		} else {
			t = 1 - mat.elements[0][0] + mat.elements[1][1] - mat.elements[2][2];
			q = quat(
				mat.elements[0][1] + mat.elements[1][0],
				t,
				mat.elements[1][2] + mat.elements[2][1],
				mat.elements[2][0] - mat.elements[0][2]
			);
		}
	} else {
		if (mat.elements[0][0] < -mat.elements[1][1]) {
			t = 1 - mat.elements[0][0] - mat.elements[1][1] + mat.elements[2][2];
			q = quat(
				mat.elements[2][0] + mat.elements[0][2],
				mat.elements[1][2] + mat.elements[2][1],
				t,
				mat.elements[0][1] - mat.elements[1][0]
			);
		} else {
			t = 1 + mat.elements[0][0] + mat.elements[1][1] + mat.elements[2][2];
			q = quat(
				mat.elements[1][2] - mat.elements[2][1],
				mat.elements[2][0] - mat.elements[0][2],
				mat.elements[0][1] - mat.elements[1][0],
				t
			);
		}
	}

	q = mul_quat_f32(q, 0.5f / sqrtf(t));

	return q;
}

static inline Quat quat_from_mat4_lh(Mat4 mat) {
	f32 t;
	Quat q;

	if (mat.elements[2][2] < 0.0f) {
		if (mat.elements[0][0] > mat.elements[1][1]) {
			t = 1 + mat.elements[0][0] - mat.elements[1][1] - mat.elements[2][2];
			q = quat(
				t,
				mat.elements[0][1] + mat.elements[1][0],
				mat.elements[2][0] + mat.elements[0][2],
				mat.elements[2][1] - mat.elements[1][2]
			);
		} else {
			t = 1 - mat.elements[0][0] + mat.elements[1][1] - mat.elements[2][2];
			q = quat(
				mat.elements[0][1] + mat.elements[1][0],
				t,
				mat.elements[1][2] + mat.elements[2][1],
				mat.elements[0][2] - mat.elements[2][0]
			);
		}
	} else {
		if (mat.elements[0][0] < -mat.elements[1][1]) {
			t = 1 - mat.elements[0][0] - mat.elements[1][1] + mat.elements[2][2];
			q = quat(
				mat.elements[2][0] + mat.elements[0][2],
				mat.elements[1][2] + mat.elements[2][1],
				t,
				mat.elements[1][0] - mat.elements[0][1]
			);
		} else {
			t = 1 + mat.elements[0][0] + mat.elements[1][1] + mat.elements[2][2];
			q = quat(
				mat.elements[2][1] - mat.elements[1][2],
				mat.elements[0][2] - mat.elements[2][0],
				mat.elements[1][0] - mat.elements[0][2],
				t
			);
		}
	}

	q = mul_quat_f32(q, 0.5f / sqrtf(t));

	return q;
}


static inline Quat quat_from_axis_angle_rh(Vec3 axis, f32 angle) {
	Quat result;

	Vec3 axis_normalized = norm_vec3(axis);
	f32 sine_of_rotation = sinf(angle / 2.0f);

	result.xyz = mul_vec3_f32(axis_normalized, sine_of_rotation);
	result.w = cosf(angle / 2.0f);

	return result;
}

static inline Quat quat_from_axis_angle_lh(Vec3 axis, f32 angle) {
	return quat_from_axis_angle_rh(axis, -angle);
}

static inline Quat quat_from_norm_pair(Vec3 left, Vec3 right) {
	Quat result;

	result.xyz = cross_vec3(left, right);
	result.w = 1.0f + dot_vec3(left, right);

	return norm_quat(result);
}

static inline Quat quat_from_vec_pair(Vec3 left, Vec3 right) {
	return quat_from_norm_pair(norm_vec3(left), norm_vec3(right));
}
