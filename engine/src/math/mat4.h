#pragma once

#include "math/types.h"
#include "math/quat.h"

static inline Mat4 mat4(void) {
	return (Mat4){0};
}

static inline Mat4 mat4_diagonal(f32 diagonal) {
	Mat4 result = {0};

	result.elements[0][0] = diagonal;
	result.elements[1][1] = diagonal;
	result.elements[2][2] = diagonal;
	result.elements[3][3] = diagonal;

	return result;
}

static inline Mat4 mat4_from_quat(Quat left) {
	Mat4 result;

	Quat normalized_quat = norm_quat(left);

	f32 xx, yy, zz,
		xy, xz, yz,
		wx, wy, wz;

	xx = normalized_quat.x * normalized_quat.x;
	yy = normalized_quat.y * normalized_quat.y;
	zz = normalized_quat.z * normalized_quat.z;
	xy = normalized_quat.x * normalized_quat.y;
	xz = normalized_quat.x * normalized_quat.z;
	yz = normalized_quat.y * normalized_quat.z;
	wx = normalized_quat.w * normalized_quat.x;
	wy = normalized_quat.w * normalized_quat.y;
	wz = normalized_quat.w * normalized_quat.z;

	result.elements[0][0] = 1.0f - 2.0f * (yy + zz);
	result.elements[0][1] = 2.0f * (xy + wz);
	result.elements[0][2] = 2.0f * (xz - wy);
	result.elements[0][3] = 0.0f;

	result.elements[1][0] = 2.0f * (xy - wz);
	result.elements[1][1] = 1.0f - 2.0f * (xx + zz);
	result.elements[1][2] = 2.0f * (yz + wx);
	result.elements[1][3] = 0.0f;

	result.elements[2][0] = 2.0f * (xz + wy);
	result.elements[2][1] = 2.0f * (yz - wx);
	result.elements[2][2] = 1.0f - 2.0f * (xx + yy);
	result.elements[2][3] = 0.0f;

	result.elements[3][0] = 0.0f;
	result.elements[3][1] = 0.0f;
	result.elements[3][2] = 0.0f;
	result.elements[3][3] = 1.0f;

	return result;
}

static inline Mat4 transpose_mat4(Mat4 mat) {
	Mat4 result;

#ifdef SIMD_USE_SSE
	result = mat;
	_MM_TRANSPOSE4_PS(result.columns[0].sse, result.columns[1].sse, result.columns[2].sse, result.columns[3].sse);
#elif defined(SIMD_USE_NEON)
	float32x4x4_t transposed = vld4q_f32((f32*)mat.columns);
	result.columns[0].neon = transposed.val[0];
	result.columns[1].neon = transposed.val[1];
	result.columns[2].neon = transposed.val[2];
	result.columns[3].neon = transposed.val[3];
#else
	result.elements[0][0] = mat.elements[0][0];
	result.elements[0][1] = mat.elements[1][0];
	result.elements[0][2] = mat.elements[2][0];
	result.elements[0][3] = mat.elements[3][0];
	result.elements[1][0] = mat.elements[0][1];
	result.elements[1][1] = mat.elements[1][1];
	result.elements[1][2] = mat.elements[2][1];
	result.elements[1][3] = mat.elements[3][1];
	result.elements[2][0] = mat.elements[0][2];
	result.elements[2][1] = mat.elements[1][2];
	result.elements[2][2] = mat.elements[2][2];
	result.elements[2][3] = mat.elements[3][2];
	result.elements[3][0] = mat.elements[0][3];
	result.elements[3][1] = mat.elements[1][3];
	result.elements[3][2] = mat.elements[2][3];
	result.elements[3][3] = mat.elements[3][3];
#endif

	return result;
}

static inline Mat4 add_mat4(Mat4 left, Mat4 right) {
	Mat4 result;

	result.columns[0] = add_vec4(left.columns[0], right.columns[0]);
	result.columns[1] = add_vec4(left.columns[1], right.columns[1]);
	result.columns[2] = add_vec4(left.columns[2], right.columns[2]);
	result.columns[3] = add_vec4(left.columns[3], right.columns[3]);

	return result;
}

static inline Mat4 sub_mat4(Mat4 left, Mat4 right) {
	Mat4 result;

	result.columns[0] = sub_vec4(left.columns[0], right.columns[0]);
	result.columns[1] = sub_vec4(left.columns[1], right.columns[1]);
	result.columns[2] = sub_vec4(left.columns[2], right.columns[2]);
	result.columns[3] = sub_vec4(left.columns[3], right.columns[3]);

	return result;
}

static inline Vec4 linear_combine_vec4_mat4(Vec4 left, Mat4 right) {
	Vec4 result;

#ifdef SIMD_USE_SSE
	result.sse = _mm_mul_ps(_mm_shuffle_ps(left.sse, left.sse, 0x00), right.columns[0].sse);
	result.sse = _mm_add_ps(result.sse, _mm_mul_ps(_mm_shuffle_ps(left.sse, left.sse, 0x55), right.columns[1].sse));
	result.sse = _mm_add_ps(result.sse, _mm_mul_ps(_mm_shuffle_ps(left.sse, left.sse, 0xaa), right.columns[2].sse));
	result.sse = _mm_add_ps(result.sse, _mm_mul_ps(_mm_shuffle_ps(left.sse, left.sse, 0xff), right.columns[3].sse));
#elif defined(SIMD_USE_NEON)
	result.neon = vmulq_laneq_f32(right.columns[0].neon, left.neon, 0);
	result.neon = vfmaq_laneq_f32(result.neon, right.columns[1].neon, left.neon, 1);
	result.neon = vfmaq_laneq_f32(result.neon, right.columns[2].neon, left.neon, 2);
	result.neon = vfmaq_laneq_f32(result.neon, right.columns[3].neon, left.neon, 3);
#else
	result.x = left.elements[0] * right.columns[0].x;
	result.y = left.elements[0] * right.columns[0].y;
	result.z = left.elements[0] * right.columns[0].z;
	result.w = left.elements[0] * right.columns[0].w;

	result.x += left.elements[1] * right.columns[1].x;
	result.y += left.elements[1] * right.columns[1].y;
	result.z += left.elements[1] * right.columns[1].z;
	result.w += left.elements[1] * right.columns[1].w;

	result.x += left.elements[2] * right.columns[2].x;
	result.y += left.elements[2] * right.columns[2].y;
	result.z += left.elements[2] * right.columns[2].z;
	result.w += left.elements[2] * right.columns[2].w;

	result.x += left.elements[3] * right.columns[3].x;
	result.y += left.elements[3] * right.columns[3].y;
	result.z += left.elements[3] * right.columns[3].z;
	result.w += left.elements[3] * right.columns[3].w;
#endif

	return result;
}

static inline Mat4 mul_mat4(Mat4 left, Mat4 right) {
	Mat4 result;

	result.columns[0] = linear_combine_vec4_mat4(right.columns[0], left);
	result.columns[1] = linear_combine_vec4_mat4(right.columns[1], left);
	result.columns[2] = linear_combine_vec4_mat4(right.columns[2], left);
	result.columns[3] = linear_combine_vec4_mat4(right.columns[3], left);

	return result;
}

static inline Mat4 mul_mat4_f32(Mat4 mat, f32 scalar) {
	Mat4 result;

#ifdef SIMD_USE_SSE
	__m128 sse_scalar = _mm_set1_ps(scalar);
	result.columns[0].sse = _mm_mul_ps(mat.columns[0].sse, sse_scalar);
	result.columns[1].sse = _mm_mul_ps(mat.columns[1].sse, sse_scalar);
	result.columns[2].sse = _mm_mul_ps(mat.columns[2].sse, sse_scalar);
	result.columns[3].sse = _mm_mul_ps(mat.columns[3].sse, sse_scalar);
#elif defined(SIMD_USE_NEON)
	result.columns[0].neon = vmulq_n_f32(mat.columns[0].neon, scalar);
	result.columns[1].neon = vmulq_n_f32(mat.columns[1].neon, scalar);
	result.columns[2].neon = vmulq_n_f32(mat.columns[2].neon, scalar);
	result.columns[3].neon = vmulq_n_f32(mat.columns[3].neon, scalar);
#else
	result.elements[0][0] = mat.elements[0][0] * scalar;
	result.elements[0][1] = mat.elements[0][1] * scalar;
	result.elements[0][2] = mat.elements[0][2] * scalar;
	result.elements[0][3] = mat.elements[0][3] * scalar;
	result.elements[1][0] = mat.elements[1][0] * scalar;
	result.elements[1][1] = mat.elements[1][1] * scalar;
	result.elements[1][2] = mat.elements[1][2] * scalar;
	result.elements[1][3] = mat.elements[1][3] * scalar;
	result.elements[2][0] = mat.elements[2][0] * scalar;
	result.elements[2][1] = mat.elements[2][1] * scalar;
	result.elements[2][2] = mat.elements[2][2] * scalar;
	result.elements[2][3] = mat.elements[2][3] * scalar;
	result.elements[3][0] = mat.elements[3][0] * scalar;
	result.elements[3][1] = mat.elements[3][1] * scalar;
	result.elements[3][2] = mat.elements[3][2] * scalar;
	result.elements[3][3] = mat.elements[3][3] * scalar;
#endif

	return result;
}

static inline Vec4 mul_mat4_vec4(Mat4 mat, Vec4 v) {
	return linear_combine_vec4_mat4(v, mat);
}

static inline Mat4 div_mat4_f32(Mat4 mat, f32 scalar) {
	Mat4 result;

#ifdef SIMD_USE_SSE
	__m128 sse_scalar = _mm_set1_ps(scalar);
	result.columns[0].sse = _mm_div_ps(mat.columns[0].sse, sse_scalar);
	result.columns[1].sse = _mm_div_ps(mat.columns[1].sse, sse_scalar);
	result.columns[2].sse = _mm_div_ps(mat.columns[2].sse, sse_scalar);
	result.columns[3].sse = _mm_div_ps(mat.columns[3].sse, sse_scalar);
#elif defined(SIMD_USE_NEON)
	float32x4_t neon_scalar = vdupq_n_f32(scalar);
	result.columns[0].neon = vdivq_f32(mat.columns[0].neon, neon_scalar);
	result.columns[1].neon = vdivq_f32(mat.columns[1].neon, neon_scalar);
	result.columns[2].neon = vdivq_f32(mat.columns[2].neon, neon_scalar);
	result.columns[3].neon = vdivq_f32(mat.columns[3].neon, neon_scalar);
#else
	result.elements[0][0] = mat.elements[0][0] / scalar;
	result.elements[0][1] = mat.elements[0][1] / scalar;
	result.elements[0][2] = mat.elements[0][2] / scalar;
	result.elements[0][3] = mat.elements[0][3] / scalar;
	result.elements[1][0] = mat.elements[1][0] / scalar;
	result.elements[1][1] = mat.elements[1][1] / scalar;
	result.elements[1][2] = mat.elements[1][2] / scalar;
	result.elements[1][3] = mat.elements[1][3] / scalar;
	result.elements[2][0] = mat.elements[2][0] / scalar;
	result.elements[2][1] = mat.elements[2][1] / scalar;
	result.elements[2][2] = mat.elements[2][2] / scalar;
	result.elements[2][3] = mat.elements[2][3] / scalar;
	result.elements[3][0] = mat.elements[3][0] / scalar;
	result.elements[3][1] = mat.elements[3][1] / scalar;
	result.elements[3][2] = mat.elements[3][2] / scalar;
	result.elements[3][3] = mat.elements[3][3] / scalar;
#endif

	return result;
}

static inline f32 determinant_mat4(Mat4 mat) {
	Vec3 c01 = cross_vec3(mat.columns[0].xyz, mat.columns[1].xyz);
	Vec3 c23 = cross_vec3(mat.columns[2].xyz, mat.columns[3].xyz);
	Vec3 b10 = sub_vec3(mul_vec3_f32(mat.columns[0].xyz, mat.columns[1].w), mul_vec3_f32(mat.columns[1].xyz, mat.columns[0].w));
	Vec3 b32 = sub_vec3(mul_vec3_f32(mat.columns[2].xyz, mat.columns[3].w), mul_vec3_f32(mat.columns[3].xyz, mat.columns[2].w));

	return dot_vec3(c01, b32) + dot_vec3(c23, b10);
}

// General-purpose inverse of a Mat4.
// Special-purpose inverses of many transformations are available in transform.h and will be more efficient.
static inline Mat4 inv_general_mat4(Mat4 mat) {
	Vec3 c01 = cross_vec3(mat.columns[0].xyz, mat.columns[1].xyz);
	Vec3 c23 = cross_vec3(mat.columns[2].xyz, mat.columns[3].xyz);
	Vec3 b10 = sub_vec3(mul_vec3_f32(mat.columns[0].xyz, mat.columns[1].w), mul_vec3_f32(mat.columns[1].xyz, mat.columns[0].w));
	Vec3 b32 = sub_vec3(mul_vec3_f32(mat.columns[2].xyz, mat.columns[3].w), mul_vec3_f32(mat.columns[3].xyz, mat.columns[2].w));

	f32 inv_det = 1.0f / (dot_vec3(c01, b32) + dot_vec3(c23, b10));
	c01 = mul_vec3_f32(c01, inv_det);
	c23 = mul_vec3_f32(c23, inv_det);
	b10 = mul_vec3_f32(b10, inv_det);
	b32 = mul_vec3_f32(b32, inv_det);

	Mat4 result;
	result.columns[0] = vec4_from_vec3(add_vec3(cross_vec3(mat.columns[1].xyz, b32), mul_vec3_f32(c23, mat.columns[1].w)), -dot_vec3(mat.columns[1].xyz, c23));
	result.columns[1] = vec4_from_vec3(sub_vec3(cross_vec3(b32, mat.columns[0].xyz), mul_vec3_f32(c23, mat.columns[0].w)), +dot_vec3(mat.columns[0].xyz, c23));
	result.columns[2] = vec4_from_vec3(add_vec3(cross_vec3(mat.columns[3].xyz, b10), mul_vec3_f32(c01, mat.columns[3].w)), -dot_vec3(mat.columns[3].xyz, c01));
	result.columns[3] = vec4_from_vec3(sub_vec3(cross_vec3(b10, mat.columns[2].xyz), mul_vec3_f32(c01, mat.columns[2].w)), +dot_vec3(mat.columns[2].xyz, c01));

	return transpose_mat4(result);
}

