#pragma once

#include "math/types.h"
#include "math/vec2.h"
#include "math/vec3.h"
#include "math/vec4.h"
#include "math/mat2.h"
#include "math/mat3.h"
#include "math/mat4.h"
#include "math/quat.h"

#define add(a, b) _Generic((a), \
	Vec2: add_vec2, \
	Vec3: add_vec3, \
	Vec4: add_vec4, \
	Mat2: add_mat2, \
	Mat3: add_mat3, \
	Mat4: add_mat4, \
	Quat: add_quat  \
)(a, b)

#define sub(a, b) _Generic((a), \
	Vec2: sub_vec2, \
	Vec3: sub_vec3, \
	Vec4: sub_vec4, \
	Mat2: sub_mat2, \
	Mat3: sub_mat3, \
	Mat4: sub_mat4, \
	Quat: sub_quat  \
)(a, b)

#define mul(a, b) _Generic((b), \
	 f32: _Generic((a), \
		Vec2: mul_vec2_f32, \
		Vec3: mul_vec3_f32, \
		Vec4: mul_vec4_f32, \
		Mat2: mul_mat2_f32, \
		Mat3: mul_mat3_f32, \
		Mat4: mul_mat4_f32, \
		Quat: mul_quat_f32  \
	 ), \
	 Mat2: mul_mat2, \
	 Mat3: mul_mat3, \
	 Mat4: mul_mat4, \
	 Quat: mul_quat, \
	 default: _Generic((a), \
		Vec2: mul_vec2, \
		Vec3: mul_vec3, \
		Vec4: mul_vec4, \
		Mat2: mul_mat2_vec2, \
		Mat3: mul_mat3_vec3, \
		Mat4: mul_mat4_vec4  \
	) \
)(a, b)

#define div(a, b) _Generic((b), \
	 f32: _Generic((a), \
		Mat2: div_mat2_f32, \
		Mat3: div_mat3_f32, \
		Mat4: div_mat4_f32, \
		Vec2: div_vec2_f32, \
		Vec3: div_vec3_f32, \
		Vec4: div_vec4_f32, \
		Quat: div_quat_f32  \
	 ), \
	 Mat2: div_mat2, \
	 Mat3: div_mat3, \
	 Mat4: div_mat4, \
	 Quat: div_quat, \
	 default: _Generic((a), \
		Vec2: div_vec2, \
		Vec3: div_vec3, \
		Vec4: div_vec4  \
	) \
)(a, b)

#define len(a) _Generic((a), \
	Vec2: len_vec2, \
	Vec3: len_vec3, \
	Vec4: len_vec4  \
)(a)

#define len_sqr(a) _Generic((a), \
	Vec2: len_sqr_vec2, \
	Vec3: len_sqr_vec3, \
	Vec4: len_sqr_vec4  \
)(a)

#define norm(a) _Generic((a), \
	Vec2: norm_vec2, \
	Vec3: norm_vec3, \
	Vec4: norm_vec4  \
)(a)

#define dot(a, b) _Generic((a), \
	Vec2: dot_vec2, \
	Vec3: dot_vec3, \
	Vec4: dot_vec4  \
)(a, b)

#define cross(a, b) _Generic((a), \
	Vec3: cross_vec3 \
)(a, b)

#define lerp(a, t, b) _Generic((a), \
	f32: lerp_f32,   \
	Vec2: lerp_vec2, \
	Vec3: lerp_vec3, \
	Vec4: lerp_vec4  \
)(a, t, b)

#define eq(a, b) _Generic((a), \
	Vec2: eq_vec2, \
	Vec3: eq_vec3, \
	Vec4: eq_vec4  \
)(a, b)

#define transpose(m) _Generic((m), \
	Mat2: transpose_mat2, \
	Mat3: transpose_mat3, \
	Mat4: transpose_mat4  \
)(m)

#define determinant(m) _Generic((m), \
	Mat2: determinant_mat2, \
	Mat3: determinant_mat3, \
	Mat4: determinant_mat4  \
)(m)

#define inv_general(m) _Generic((m), \
	Mat2: inv_general_mat2, \
	Mat3: inv_general_mat3, \
	Mat4: inv_general_mat4  \
)(m)
