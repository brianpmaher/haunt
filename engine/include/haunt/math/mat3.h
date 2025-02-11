#pragma once

#include "haunt/math/types.h"

static inline Mat3 mat3(void) {
	return (Mat3){0};
}

static inline Mat3 mat3_diagonal(f32 diagonal) {
	Mat3 result = {0};

	result.elements[0][0] = diagonal;
	result.elements[1][1] = diagonal;
	result.elements[2][2] = diagonal;

	return result;
}

static inline Mat3 transpose_mat3(Mat3 mat) {
	Mat3 result = mat;

	result.elements[0][1] = mat.elements[1][0];
	result.elements[0][2] = mat.elements[2][0];
	result.elements[1][0] = mat.elements[0][1];
	result.elements[1][2] = mat.elements[2][1];
	result.elements[2][1] = mat.elements[1][2];
	result.elements[2][0] = mat.elements[0][2];

	return result;
}

static inline Mat3 add_mat3(Mat3 left, Mat3 right) {
	Mat3 result;

	result.elements[0][0] = left.elements[0][0] + right.elements[0][0];
	result.elements[0][1] = left.elements[0][1] + right.elements[0][1];
	result.elements[0][2] = left.elements[0][2] + right.elements[0][2];
	result.elements[1][0] = left.elements[1][0] + right.elements[1][0];
	result.elements[1][1] = left.elements[1][1] + right.elements[1][1];
	result.elements[1][2] = left.elements[1][2] + right.elements[1][2];
	result.elements[2][0] = left.elements[2][0] + right.elements[2][0];
	result.elements[2][1] = left.elements[2][1] + right.elements[2][1];
	result.elements[2][2] = left.elements[2][2] + right.elements[2][2];

	return result;
}

static inline Mat3 sub_mat3(Mat3 left, Mat3 right) {
	Mat3 result;

	result.elements[0][0] = left.elements[0][0] - right.elements[0][0];
	result.elements[0][1] = left.elements[0][1] - right.elements[0][1];
	result.elements[0][2] = left.elements[0][2] - right.elements[0][2];
	result.elements[1][0] = left.elements[1][0] - right.elements[1][0];
	result.elements[1][1] = left.elements[1][1] - right.elements[1][1];
	result.elements[1][2] = left.elements[1][2] - right.elements[1][2];
	result.elements[2][0] = left.elements[2][0] - right.elements[2][0];
	result.elements[2][1] = left.elements[2][1] - right.elements[2][1];
	result.elements[2][2] = left.elements[2][2] - right.elements[2][2];

	return result;
}

static inline Vec3 mul_mat3_vec3(Mat3 mat, Vec3 v) {
	Vec3 result;

	result.x = v.elements[0] * mat.columns[0].x;
	result.y = v.elements[0] * mat.columns[0].y;
	result.z = v.elements[0] * mat.columns[0].z;

	result.x += v.elements[1] * mat.columns[1].x;
	result.y += v.elements[1] * mat.columns[1].y;
	result.z += v.elements[1] * mat.columns[1].z;

	result.x += v.elements[2] * mat.columns[2].x;
	result.y += v.elements[2] * mat.columns[2].y;
	result.z += v.elements[2] * mat.columns[2].z;

	return result;
}

static inline Mat3 mul_mat3(Mat3 left, Mat3 right) {
	Mat3 result;

	result.columns[0] = mul_mat3_vec3(left, right.columns[0]);
	result.columns[1] = mul_mat3_vec3(left, right.columns[1]);
	result.columns[2] = mul_mat3_vec3(left, right.columns[2]);

	return result;
}

static inline Mat3 mul_mat3_f32(Mat3 mat, f32 scalar) {
	Mat3 result;

	result.elements[0][0] = mat.elements[0][0] * scalar;
	result.elements[0][1] = mat.elements[0][1] * scalar;
	result.elements[0][2] = mat.elements[0][2] * scalar;
	result.elements[1][0] = mat.elements[1][0] * scalar;
	result.elements[1][1] = mat.elements[1][1] * scalar;
	result.elements[1][2] = mat.elements[1][2] * scalar;
	result.elements[2][0] = mat.elements[2][0] * scalar;
	result.elements[2][1] = mat.elements[2][1] * scalar;
	result.elements[2][2] = mat.elements[2][2] * scalar;

	return result;
}

static inline Mat3 div_mat3_f32(Mat3 mat, f32 scalar) {
	Mat3 result;

	result.elements[0][0] = mat.elements[0][0] / scalar;
	result.elements[0][1] = mat.elements[0][1] / scalar;
	result.elements[0][2] = mat.elements[0][2] / scalar;
	result.elements[1][0] = mat.elements[1][0] / scalar;
	result.elements[1][1] = mat.elements[1][1] / scalar;
	result.elements[1][2] = mat.elements[1][2] / scalar;
	result.elements[2][0] = mat.elements[2][0] / scalar;
	result.elements[2][1] = mat.elements[2][1] / scalar;
	result.elements[2][2] = mat.elements[2][2] / scalar;

	return result;
}

static inline f32 determinant_mat3(Mat3 mat) {
	Mat3 cross_val;

	cross_val.columns[0] = cross_vec3(mat.columns[1], mat.columns[2]);
	cross_val.columns[1] = cross_vec3(mat.columns[2], mat.columns[0]);
	cross_val.columns[2] = cross_vec3(mat.columns[0], mat.columns[1]);

	return dot_vec3(cross_val.columns[2], mat.columns[2]);
}

static inline Mat3 inv_general_mat3(Mat3 mat) {
	Mat3 cross_val;

	cross_val.columns[0] = cross_vec3(mat.columns[1], mat.columns[2]);
	cross_val.columns[1] = cross_vec3(mat.columns[2], mat.columns[0]);
	cross_val.columns[2] = cross_vec3(mat.columns[0], mat.columns[1]);

	f32 inv_det = 1.0f / dot_vec3(cross_val.columns[2], mat.columns[2]);

	Mat3 result;

	result.columns[0] = mul_vec3_f32(cross_val.columns[0], inv_det);
	result.columns[1] = mul_vec3_f32(cross_val.columns[1], inv_det);
	result.columns[2] = mul_vec3_f32(cross_val.columns[2], inv_det);

	return transpose_mat3(result);
}
