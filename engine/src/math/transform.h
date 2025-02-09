#pragma once

#include "core/types.h"
#include "math/types.h"

// Produces a right-handed orthographic projection mat with z ranging from -1 to 1 (the GL convention).
// left, right, bottom, and top specify the coordinates of their respective clipping planes.
// near_plane and far_plane specify the distances to the near and far clipping planes.
static inline Mat4 orthographic_rh_no(f32 left, f32 right, f32 bottom, f32 top, f32 near_plane, f32 far_plane) {
	Mat4 result = {0};

	result.elements[0][0] = 2.0f / (right - left);
	result.elements[1][1] = 2.0f / (top - bottom);
	result.elements[2][2] = 2.0f / (near_plane - far_plane);
	result.elements[3][3] = 1.0f;

	result.elements[3][0] = (left + right) / (left - right);
	result.elements[3][1] = (bottom + top) / (bottom - top);
	result.elements[3][2] = (near_plane + far_plane) / (near_plane - far_plane);

	return result;
}

// Produces a right-handed orthographic projection mat with z ranging from 0 to 1 (the DirectX convention).
// left, right, bottom, and top specify the coordinates of their respective clipping planes.
// near_plane and far_plane specify the distances to the near and far clipping planes.
static inline Mat4 orthographic_rh_zo(f32 left, f32 right, f32 bottom, f32 top, f32 near_plane, f32 far_plane) {
	Mat4 result = {0};

	result.elements[0][0] = 2.0f / (right - left);
	result.elements[1][1] = 2.0f / (top - bottom);
	result.elements[2][2] = 1.0f / (near_plane - far_plane);
	result.elements[3][3] = 1.0f;

	result.elements[3][0] = (left + right) / (left - right);
	result.elements[3][1] = (bottom + top) / (bottom - top);
	result.elements[3][2] = (near_plane) / (near_plane - far_plane);

	return result;
}

// Produces a left-handed orthographic projection mat with z ranging from -1 to 1 (the GL convention).
// left, right, bottom, and top specify the coordinates of their respective clipping planes.
// near_plane and far_plane specify the distances to the near and far clipping planes.
static inline Mat4 orthographic_lh_no(f32 left, f32 right, f32 bottom, f32 top, f32 near_plane, f32 far_plane) {
	Mat4 result = orthographic_rh_no(left, right, bottom, top, near_plane, far_plane);
	result.elements[2][2] = -result.elements[2][2];
	return result;
}

// Produces a left-handed orthographic projection mat with z ranging from 0 to 1 (the DirectX convention).
// left, right, bottom, and top specify the coordinates of their respective clipping planes.
// near_plane and far_plane specify the distances to the near and far clipping planes.
static inline Mat4 orthographic_lh_zo(f32 left, f32 right, f32 bottom, f32 top, f32 near_plane, f32 far_plane) {
	Mat4 result = orthographic_rh_zo(left, right, bottom, top, near_plane, far_plane);
	result.elements[2][2] = -result.elements[2][2];

	return result;
}

// Returns an inverse for the given orthographic projection mat. Works for all orthographic
// projection matrices, regardless of handedness or NDC convention.
static inline Mat4 inv_orthographic(Mat4 ortho_mat) {
	Mat4 result = {0};
	result.elements[0][0] = 1.0f / ortho_mat.elements[0][0];
	result.elements[1][1] = 1.0f / ortho_mat.elements[1][1];
	result.elements[2][2] = 1.0f / ortho_mat.elements[2][2];
	result.elements[3][3] = 1.0f;

	result.elements[3][0] = -ortho_mat.elements[3][0] * result.elements[0][0];
	result.elements[3][1] = -ortho_mat.elements[3][1] * result.elements[1][1];
	result.elements[3][2] = -ortho_mat.elements[3][2] * result.elements[2][2];

	return result;
}

static inline Mat4 perspective_rh_no(f32 fov, f32 aspect_ratio, f32 near_plane, f32 far_plane) {
	Mat4 result = {0};

	// See https://www.khronos.org/registry/OpenGL-Refpages/gl2.1/xhtml/gluPerspective.xml

	f32 Cotangent = 1.0f / tanf(fov / 2.0f);
	result.elements[0][0] = Cotangent / aspect_ratio;
	result.elements[1][1] = Cotangent;
	result.elements[2][3] = -1.0f;

	result.elements[2][2] = (near_plane + far_plane) / (near_plane - far_plane);
	result.elements[3][2] = (2.0f * near_plane * far_plane) / (near_plane - far_plane);

	return result;
}

static inline Mat4 oerspective_rh_zo(f32 fov, f32 aspect_ratio, f32 near_plane, f32 far_plane) {
	Mat4 result = {0};

	// See https://www.khronos.org/registry/OpenGL-Refpages/gl2.1/xhtml/gluPerspective.xml

	f32 Cotangent = 1.0f / tanf(fov / 2.0f);
	result.elements[0][0] = Cotangent / aspect_ratio;
	result.elements[1][1] = Cotangent;
	result.elements[2][3] = -1.0f;

	result.elements[2][2] = (far_plane) / (near_plane - far_plane);
	result.elements[3][2] = (near_plane * far_plane) / (near_plane - far_plane);

	return result;
}

static inline Mat4 perspective_lh_no(f32 fov, f32 aspect_ratio, f32 near_plane, f32 far_plane) {
	Mat4 result = perspective_rh_no(fov, aspect_ratio, near_plane, far_plane);
	result.elements[2][2] = -result.elements[2][2];
	result.elements[2][3] = -result.elements[2][3];

	return result;
}

static inline Mat4 perspective_lh_zo(f32 fov, f32 aspect_ratio, f32 near_plane, f32 far_plane) {
	Mat4 result = oerspective_rh_zo(fov, aspect_ratio, near_plane, far_plane);
	result.elements[2][2] = -result.elements[2][2];
	result.elements[2][3] = -result.elements[2][3];

	return result;
}

static inline Mat4 inv_perspective_rh(Mat4 perspective_mat) {
	Mat4 result = {0};
	result.elements[0][0] = 1.0f / perspective_mat.elements[0][0];
	result.elements[1][1] = 1.0f / perspective_mat.elements[1][1];
	result.elements[2][2] = 0.0f;

	result.elements[2][3] = 1.0f / perspective_mat.elements[3][2];
	result.elements[3][3] = perspective_mat.elements[2][2] * result.elements[2][3];
	result.elements[3][2] = perspective_mat.elements[2][3];

	return result;
}

static inline Mat4 inv_perspective_lh(Mat4 perspective_mat) {
	Mat4 result = {0};
	result.elements[0][0] = 1.0f / perspective_mat.elements[0][0];
	result.elements[1][1] = 1.0f / perspective_mat.elements[1][1];
	result.elements[2][2] = 0.0f;

	result.elements[2][3] = 1.0f / perspective_mat.elements[3][2];
	result.elements[3][3] = perspective_mat.elements[2][2] * -result.elements[2][3];
	result.elements[3][2] = perspective_mat.elements[2][3];

	return result;
}

static inline Mat4 translate(Vec3 translation_mat) {
	Mat4 result = mat4_diagonal(1.0f);
	result.elements[3][0] = translation_mat.x;
	result.elements[3][1] = translation_mat.y;
	result.elements[3][2] = translation_mat.z;

	return result;
}

static inline Mat4 inv_translate(Mat4 translation_mat) {
	Mat4 result = translation_mat;
	result.elements[3][0] = -result.elements[3][0];
	result.elements[3][1] = -result.elements[3][1];
	result.elements[3][2] = -result.elements[3][2];

	return result;
}

static inline Mat4 rotate_rh(f32 angle, Vec3 axis) {
	Mat4 result = mat4_diagonal(1.0f);

	axis = norm_vec3(axis);

	f32 SinTheta = sinf(angle);
	f32 CosTheta = cosf(angle);
	f32 CosValue = 1.0f - CosTheta;

	result.elements[0][0] = (axis.x * axis.x * CosValue) + CosTheta;
	result.elements[0][1] = (axis.x * axis.y * CosValue) + (axis.z * SinTheta);
	result.elements[0][2] = (axis.x * axis.z * CosValue) - (axis.y * SinTheta);

	result.elements[1][0] = (axis.y * axis.x * CosValue) - (axis.z * SinTheta);
	result.elements[1][1] = (axis.y * axis.y * CosValue) + CosTheta;
	result.elements[1][2] = (axis.y * axis.z * CosValue) + (axis.x * SinTheta);

	result.elements[2][0] = (axis.z * axis.x * CosValue) + (axis.y * SinTheta);
	result.elements[2][1] = (axis.z * axis.y * CosValue) - (axis.x * SinTheta);
	result.elements[2][2] = (axis.z * axis.z * CosValue) + CosTheta;

	return result;
}

static inline Mat4 rotate_lh(f32 angle, Vec3 axis) {
	/* mat will be inverse/transpose of RH. */
	return rotate_rh(-angle, axis);
}

static inline Mat4 inv_rotate(Mat4 rotation_mat) {
	return transpose_mat4(rotation_mat);
}

static inline Mat4 scale(Vec3 scale_vec) {
	Mat4 result = mat4_diagonal(1.0f);

	result.elements[0][0] = scale_vec.x;
	result.elements[1][1] = scale_vec.y;
	result.elements[2][2] = scale_vec.z;

	return result;
}

static inline Mat4 inv_scale(Mat4 scale_mat) {
	Mat4 result = scale_mat;

	result.elements[0][0] = 1.0f / result.elements[0][0];
	result.elements[1][1] = 1.0f / result.elements[1][1];
	result.elements[2][2] = 1.0f / result.elements[2][2];

	return result;
}

static inline Mat4 _look_at(Vec3 f,  Vec3 s, Vec3 u,  Vec3 eye) {
	Mat4 result;

	result.elements[0][0] = s.x;
	result.elements[0][1] = u.x;
	result.elements[0][2] = -f.x;
	result.elements[0][3] = 0.0f;

	result.elements[1][0] = s.y;
	result.elements[1][1] = u.y;
	result.elements[1][2] = -f.y;
	result.elements[1][3] = 0.0f;

	result.elements[2][0] = s.z;
	result.elements[2][1] = u.z;
	result.elements[2][2] = -f.z;
	result.elements[2][3] = 0.0f;

	result.elements[3][0] = -dot_vec3(s, eye);
	result.elements[3][1] = -dot_vec3(u, eye);
	result.elements[3][2] = dot_vec3(f, eye);
	result.elements[3][3] = 1.0f;

	return result;
}

static inline Mat4 look_at_rh(Vec3 eye, Vec3 center, Vec3 up) {
	Vec3 f = norm_vec3(sub_vec3(center, eye));
	Vec3 s = norm_vec3(cross(f, up));
	Vec3 u = cross(s, f);

	return _look_at(f, s, u, eye);
}

static inline Mat4 look_at_lh(Vec3 eye, Vec3 center, Vec3 up) {
	Vec3 f = norm_vec3(sub_vec3(eye, center));
	Vec3 s = norm_vec3(cross(f, up));
	Vec3 u = cross(s, f);

	return _look_at(f, s, u, eye);
}

static inline Mat4 inv_look_at(Mat4 mat) {
	Mat4 result;

	Mat3 rotation_mat = {0};
	rotation_mat.columns[0] = mat.columns[0].xyz;
	rotation_mat.columns[1] = mat.columns[1].xyz;
	rotation_mat.columns[2] = mat.columns[2].xyz;
	rotation_mat = transpose_mat3(rotation_mat);

	result.columns[0] = vec4_from_vec3(rotation_mat.columns[0], 0.0f);
	result.columns[1] = vec4_from_vec3(rotation_mat.columns[1], 0.0f);
	result.columns[2] = vec4_from_vec3(rotation_mat.columns[2], 0.0f);
	result.columns[3] = mul_vec4_f32(mat.columns[3], -1.0f);
	result.elements[3][0] = -1.0f * mat.elements[3][0] /
		(rotation_mat.elements[0][0] + rotation_mat.elements[0][1] + rotation_mat.elements[0][2]);
	result.elements[3][1] = -1.0f * mat.elements[3][1] /
		(rotation_mat.elements[1][0] + rotation_mat.elements[1][1] + rotation_mat.elements[1][2]);
	result.elements[3][2] = -1.0f * mat.elements[3][2] /
		(rotation_mat.elements[2][0] + rotation_mat.elements[2][1] + rotation_mat.elements[2][2]);
	result.elements[3][3] = 1.0f;

	return result;
}
