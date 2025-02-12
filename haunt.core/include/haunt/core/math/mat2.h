#pragma once

#include "haunt/core/math/types.h"

static inline Mat2 mat2(void) {
	Mat2 result = {0};
	return result;
}

static inline Mat2 mat2_diagonal(f32 diagonal) {
	Mat2 result = {0};

	result.elements[0][0] = diagonal;
	result.elements[1][1] = diagonal;

	return result;
}

static inline Mat2 transpose_mat2(Mat2 mat) {
	Mat2 result = mat;

	result.elements[0][1] = mat.elements[1][0];
	result.elements[1][0] = mat.elements[0][1];

	return result;
}

static inline Mat2 add_mat2(Mat2 left, Mat2 right) {
	Mat2 result;

	result.elements[0][0] = left.elements[0][0] + right.elements[0][0];
	result.elements[0][1] = left.elements[0][1] + right.elements[0][1];
	result.elements[1][0] = left.elements[1][0] + right.elements[1][0];
	result.elements[1][1] = left.elements[1][1] + right.elements[1][1];

	return result;
}

static inline Mat2 sub_mat2(Mat2 left, Mat2 right) {
	Mat2 result;

	result.elements[0][0] = left.elements[0][0] - right.elements[0][0];
	result.elements[0][1] = left.elements[0][1] - right.elements[0][1];
	result.elements[1][0] = left.elements[1][0] - right.elements[1][0];
	result.elements[1][1] = left.elements[1][1] - right.elements[1][1];

	return result;
}

static inline Vec2 mul_mat2_vec2(Mat2 mat, Vec2 v) {
	Vec2 result;

	result.x = v.elements[0] * mat.columns[0].x;
	result.y = v.elements[0] * mat.columns[0].y;

	result.x += v.elements[1] * mat.columns[1].x;
	result.y += v.elements[1] * mat.columns[1].y;

	return result;
}

static inline Mat2 mul_mat2(Mat2 left, Mat2 right) {
	Mat2 result;

	result.columns[0] = mul_mat2_vec2(left, right.columns[0]);
	result.columns[1] = mul_mat2_vec2(left, right.columns[1]);

	return result;
}

static inline Mat2 mul_mat2_f32(Mat2 mat, f32 scalar) {
	Mat2 result;

	result.elements[0][0] = mat.elements[0][0] * scalar;
	result.elements[0][1] = mat.elements[0][1] * scalar;
	result.elements[1][0] = mat.elements[1][0] * scalar;
	result.elements[1][1] = mat.elements[1][1] * scalar;

	return result;
}

static inline Mat2 div_mat2_f32(Mat2 mat, f32 scalar) {
	Mat2 result;

	result.elements[0][0] = mat.elements[0][0] / scalar;
	result.elements[0][1] = mat.elements[0][1] / scalar;
	result.elements[1][0] = mat.elements[1][0] / scalar;
	result.elements[1][1] = mat.elements[1][1] / scalar;

	return result;
}

static inline f32 determinant_mat2(Mat2 mat) {
	return mat.elements[0][0] * mat.elements[1][1] -
	       mat.elements[0][1] * mat.elements[1][0];
}

static inline Mat2 inv_general_mat2(Mat2 mat) {
	Mat2 result;

	f32 inv_det = 1.0f / determinant_mat2(mat);

	result.elements[0][0] = inv_det * +mat.elements[1][1];
	result.elements[1][1] = inv_det * +mat.elements[0][0];
	result.elements[0][1] = inv_det * -mat.elements[0][1];
	result.elements[1][0] = inv_det * -mat.elements[1][0];

	return result;
}
