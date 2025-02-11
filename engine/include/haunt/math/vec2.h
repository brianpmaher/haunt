#pragma once

#include "haunt/math/types.h"

static inline Vec2 vec2(f32 x, f32 y) {
	Vec2 vec;

	vec.x = x;
	vec.y = y;

	return vec;
}

static inline Vec2 add_vec2(Vec2 left, Vec2 right) {
	Vec2 result;

	result.x = left.x + right.x;
	result.y = left.y + right.y;

	return result;
}

static inline Vec2 sub_vec2(Vec2 left, Vec2 right) {
	Vec2 result;

	result.x = left.x - right.x;
	result.y = left.y - right.y;

	return result;
}

static inline Vec2 mul_vec2(Vec2 left, Vec2 right) {
	Vec2 result;

	result.x = left.x * right.x;
	result.y = left.y * right.y;

	return result;
}

static inline Vec2 mul_vec2_f32(Vec2 left, f32 right) {
	Vec2 result;

	result.x = left.x * right;
	result.y = left.y * right;

	return result;
}

static inline Vec2 div_vec2(Vec2 left, Vec2 right) {
	Vec2 result;

	result.x = left.x / right.x;
	result.y = left.y / right.y;

	return result;
}

static inline Vec2 div_vec2_f32(Vec2 left, f32 right) {
	Vec2 result;

	result.x = left.x / right;
	result.y = left.y / right;

	return result;
}

static inline b8 eq_vec2(Vec2 left, Vec2 right) {
	return left.x == right.x && left.y == right.y;
}

static inline f32 dot_vec2(Vec2 left, Vec2 right) {
	return (left.x * right.x) + (left.y * right.y);
}

static inline f32 len_sqr_vec2(Vec2 a) {
	return dot_vec2(a, a);
}

static inline f32 len_vec2(Vec2 a) {
	return sqrtf(len_sqr_vec2(a));
}

static inline Vec2 norm_vec2(Vec2 a) {
	return mul_vec2_f32(a, invsqrt_f32(dot_vec2(a, a)));
}

static inline Vec2 lerp_vec2(Vec2 a, f32 t, Vec2 b) {
	return add_vec2(mul_vec2_f32(a, 1.0f - t), mul_vec2_f32(b, t));
}

static inline Vec2 rotatet_vec2(Vec2 v, f32 angle) {
	f32 sin_angle = sinf(angle);
	f32 cos_angle = cosf(angle);

	return vec2(v.x * cos_angle - v.y * sin_angle, v.x * sin_angle + v.y * cos_angle);
}
