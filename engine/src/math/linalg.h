#pragma once

// TODO: Compile
// TODO: Visually inspect all functions, variable names, comments, etc
// TODO: Add handmade-math attribution
// TODO: Add quat_from_mat4_rh insomniac attribution
// TODO: Add rotate_vec3_quat molecular matters attribution

#include "math/base.h"
#include "math/angle.h"
#include "math/types.h"
#include "math/vec2.h"
#include "math/vec3.h"
#include "math/vec4.h"
#include "math/mat2.h"
#include "math/mat3.h"
#include "math/mat4.h"
#include "math/quat.h"
#include "math/transform.h"
#include "math/generics.h"

//
// Additional functions that depend on others
//

static inline Vec3 rotate_vec3_axis_angle_lh(Vec3 v, Vec3 axis, f32 angle) {
	return rotate_vec3_quat(v, quat_from_axis_angle_lh(axis, angle));
}

static inline Vec3 rotate_vec3_axis_angle_rh(Vec3 v, Vec3 axis, f32 angle) {
	return rotate_vec3_quat(v, quat_from_axis_angle_rh(axis, angle));
}

// implementation from https://blog.molecular-matters.com/2013/05/24/a-faster-quat-vec-multiplication/
static inline Vec3 rotate_vec3_quat(Vec3 v, Quat q) {
	Vec3 t = mul_vec3_f32(cross(q.xyz, v), 2);
	return add_vec3(v, add_vec3(mul_vec3_f32(t, q.w), cross(q.xyz, t)));
}
