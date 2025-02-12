#pragma once

#include "haunt/core/math/types.h"

static inline Vec3 vec3(f32 x, f32 y, f32 z) {
	Vec3 vec;

	vec.x = x;
	vec.y = y;
	vec.z = z;

	return vec;
}

static Vec3 vec3_from_vec2(Vec2 v, f32 z) {
	return vec3(v.x, v.y, z);
}

static inline Vec3 add_vec3(Vec3 left, Vec3 right) {
	Vec3 result;

	result.x = left.x + right.x;
	result.y = left.y + right.y;
	result.z = left.z + right.z;

	return result;
}

static inline Vec3 sub_vec3(Vec3 left, Vec3 right) {
	Vec3 result;

	result.x = left.x - right.x;
	result.y = left.y - right.y;
	result.z = left.z - right.z;

	return result;
}

static inline Vec3 mul_vec3(Vec3 left, Vec3 right) {
	Vec3 result;

	result.x = left.x * right.x;
	result.y = left.y * right.y;
	result.z = left.z * right.z;

	return result;
}

static inline Vec3 mul_vec3_f32(Vec3 left, f32 right) {
	Vec3 result;

	result.x = left.x * right;
	result.y = left.y * right;
	result.z = left.z * right;

	return result;
}

static inline Vec3 div_vec3(Vec3 left, Vec3 right) {
	Vec3 result;

	result.x = left.x / right.x;
	result.y = left.y / right.y;
	result.z = left.z / right.z;

	return result;
}

static inline Vec3 div_vec3_f32(Vec3 left, f32 right) {
	Vec3 result;

	result.x = left.x / right;
	result.y = left.y / right;
	result.z = left.z / right;

	return result;
}

static inline b8 eq_vec3(Vec3 left, Vec3 right) {
	return left.x == right.x && left.y == right.y && left.z == right.z;
}

static inline f32 dot_vec3(Vec3 left, Vec3 right) {
	return (left.x * right.x) + (left.y * right.y) + (left.z * right.z);
}

static inline Vec3 cross_vec3(Vec3 left, Vec3 right) {
	Vec3 result;

	result.x = (left.y * right.z) - (left.z * right.y);
	result.y = (left.z * right.x) - (left.x * right.z);
	result.z = (left.x * right.y) - (left.y * right.x);

	return result;
}

static inline f32 len_sqr_vec3(Vec3 a) {
	return dot_vec3(a, a);
}

static inline f32 len_vec3(Vec3 a) {
	return sqrtf(len_sqr_vec3(a));
}

static inline Vec3 norm_vec3(Vec3 a) {
	return mul_vec3_f32(a, invsqrt_f32(dot_vec3(a, a)));
}

static inline Vec3 lerp_vec3(Vec3 a, f32 t, Vec3 b) {
	return add_vec3(mul_vec3_f32(a, 1.0f - t), mul_vec3_f32(b, t));
}
