#pragma once

#define C11_GENERICS_ENABLED 1

#include "haunt/core/simd.h"
#include "haunt/core/types.h"

#include <float.h>
#include <math.h>

//
// Constants
//

#define e               2.71828182845904523536  // e
#define log2e           1.44269504088896340736  // log2(e)
#define log10e          0.434294481903251827651 // log10(e)
#define ln2             0.693147180559945309417 // ln(2)
#define ln10            2.30258509299404568402  // ln(10)
#define pi              3.14159265358979323846  // pi
#define pi_over_two     1.57079632679489661923  // pi / 2
#define pi_over_four    0.785398163397448309616 // pi / 4
#define one_over_pi     0.318309886183790671538 // 1 / pi
#define two_over_pi     0.636619772367581343076 // 2 / pi
#define two_over_sqrtpi 1.12837916709551257390  // 2 / sqrt(pi)
#define sqrt2           1.41421356237309504880  // sqrt(2)
#define one_over_sqrt2  0.707106781186547524401 // 1 / sqrt(2)

//
// Sizes
//

#define f32_max FLT_MAX
#define f64_max DBL_MAX

#define f32_epsilon FLT_EPSILON
#define f64_epsilon DBL_EPSILON

#define f32_min FLT_MIN
#define f64_min DBL_MIN

//
// Common utils
//

#define abs(a)    ((a) > 0 ? (a) : -(a))
#define mod(a, m) (((a) % (m)) >= 0 ? ((a) % (m)) : (((a) % (m)) + (m)))
#define square(x) ((x) * (x))

//
// Types
//

typedef union Vec2 {
	struct {
		f32 x;
		f32 y;
	};
	struct {
		f32 u;
		f32 v;
	};
	struct {
		f32 width;
		f32 height;
	};
	f32 elements[2];
} Vec2;

typedef union Vec2i {
	struct {
		i32 x;
		i32 y;
	};
	struct {
		i32 u;
		i32 v;
	};
	struct {
		i32 width;
		i32 height;
	};
	i32 elements[2];
} Vec2i;

typedef union Vec3 {
	struct {
		f32 x;
		f32 y;
		f32 z;
	};
	struct {
		f32 u;
		f32 v;
		f32 w;
	};
	struct {
		f32 r;
		f32 g;
		f32 b;
	};
	struct {
		Vec2 xy;
		f32 _ignored0;
	};
	struct {
		f32 _ignored1;
		Vec2 yz;
	};
	struct {
		Vec2 uv;
		f32 _ignored2;
	};
	struct {
		f32 _ignored3;
		Vec2 vw;
	};
	f32 elements[3];
} Vec3;

typedef union Vec4 {
	struct {
		union {
			Vec3 xyz;
			struct {
				f32 x;
				f32 y;
				f32 z;
			};
		};
		f32 w;
	};
	struct {
		union {
			Vec3 rgb;
			struct {
				f32 r;
				f32 g;
				f32 b;
			};
		};
		f32 a;
	};
	struct {
		Vec2 xy;
		f32 _ignored0;
		f32 _ignored1;
	};
	struct {
		f32 _ignored2;
		Vec2 yz;
		f32 _ignored3;
	};
	struct {
		f32 _ignored4;
		f32 _ignored5;
		Vec2 zw;
	};
	f32 elements[4];

#ifdef SIMD_USE_SSE
	__m128 sse;
#endif

#ifdef SIMD_USE_NEON
	float32x4_t neon;
#endif
} Vec4;

typedef union Mat2 {
	f32 elements[2][2];
	Vec2 columns[2];
} Mat2;

typedef union Mat3 {
	f32 elements[3][3];
	Vec3 columns[3];
} Mat3;

typedef union Mat4 {
	f32 elements[4][4];
	Vec4 columns[4];
} Mat4;

typedef union Quat {
	struct {
		union {
			Vec3 xyz;
			struct {
				f32 x;
				f32 y;
				f32 z;
			};
		};
		f32 w;
	};
	f32 elements[4];

#ifdef SIMD_USE_SSE
	__m128 sse;
#endif

#ifdef SIMD_USE_NEON
	float32x4_t neon;
#endif
} Quat;

//
// Better math functions
//

static inline f32 sqrt_f32(f32 value) {
	f32 result;

#ifdef SIMD_USE_SSE
	__m128 in = _mm_set_ss(value);
	__m128 out = _mm_sqrt_ss(in);
	result = _mm_cvtss_f32(out);
#elif defined(SIMD_USE_NEON)
	float32x4_t in = vdupq_n_f32(value);
	float32x4_t out = vsqrtq_f32(in);
	result = vgetq_lane_f32(out, 0);
#else
	result = sqrtf(value);
#endif

	return result;
}

static inline f32 invsqrt_f32(f32 value) {
	return 1.0f / sqrt_f32(value);
}

//
// Interpolation utilities
//

static inline f32 lerp_f32(f32 a, f32 t, f32 b) {
	return (1.0f - t) * a + t * b;
}

static inline f32 clamp(f32 min_val, f32 val, f32 max_val) {
	f32 result = val;

	if (result < min_val) {
		result = min_val;
	}

	if (result > max_val) {
		result = max_val;
	}

	return result;
}

//
// Angle unit conversions
//

#define rad_to_deg(x)  (180.0 / pi)
#define rad_to_turn(x) (0.5 / pi)
#define deg_to_rad(x)  (pi / 180.0)
#define deg_to_turn(x) (0.5 / 180.0)
#define turn_to_rad(x) (pi / 0.5)
#define turn_to_deg(x) (180.0 / 0.5)

//
// Vec2 functions
//

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
	return sqrt_f32(len_sqr_vec2(a));
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

//
// Vec2i functions
//

static inline Vec2i vec2i(i32 x, i32 y) {
	Vec2i vec;

	vec.x = x;
	vec.y = y;

	return vec;
}

static inline Vec2i add_vec2i(Vec2i left, Vec2i right) {
	Vec2i result;

	result.x = left.x + right.x;
	result.y = left.y + right.y;

	return result;
}

static inline Vec2i sub_vec2i(Vec2i left, Vec2i right) {
	Vec2i result;

	result.x = left.x - right.x;
	result.y = left.y - right.y;

	return result;
}

//
// Vec3 functions
//

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
	return sqrt_f32(len_sqr_vec3(a));
}

static inline Vec3 norm_vec3(Vec3 a) {
	return mul_vec3_f32(a, invsqrt_f32(dot_vec3(a, a)));
}

static inline Vec3 lerp_vec3(Vec3 a, f32 t, Vec3 b) {
	return add_vec3(mul_vec3_f32(a, 1.0f - t), mul_vec3_f32(b, t));
}

//
// Vec4 functions
//

static inline Vec4 vec4(f32 x, f32 y, f32 z, f32 w) {
	Vec4 vec;

#ifdef SIMD_USE_SSE
	vec.sse = _mm_setr_ps(x, y, z, w);
#elif defined(SIMD_USE_NEON)
	vec.neon = (float32x4_t){x, y, z, w};
#else
	vec.x = x;
	vec.y = y;
	vec.z = z;
	vec.w = w;
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
	float32x4_t neon_multiply_result = vmulq_f32(left.neon, right.neon);
	float32x4_t neon_half_add = vpaddq_f32(neon_multiply_result, neon_multiply_result);
	float32x4_t neon_full_add = vpaddq_f32(neon_half_add, neon_half_add);
	result = vgetq_lane_f32(neon_full_add, 0);
#else
	result = ((left.x * right.x) + (left.z * right.z)) + ((left.y * right.y) + (left.w * right.w));
#endif

	return result;
}

static inline f32 len_sqr_vec4(Vec4 a) {
	return dot_vec4(a, a);
}

static inline f32 len_vec4(Vec4 a) {
	return sqrt_f32(len_sqr_vec4(a));
}

static inline Vec4 norm_vec4(Vec4 a) {
	return mul_vec4_f32(a, invsqrt_f32(dot_vec4(a, a)));
}

static inline Vec4 lerp_vec4(Vec4 a, f32 t, Vec4 b) {
	return add_vec4(mul_vec4_f32(a, 1.0f - t), mul_vec4_f32(b, t));
}

//
// Mat2 functions
//

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
	return mat.elements[0][0] * mat.elements[1][1] - mat.elements[0][1] * mat.elements[1][0];
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

//
// Mat3 functions
//

static inline Mat3 mat3(void) {
	Mat3 result = {0};
	return result;
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

//
// Mat4 functions
//

static inline Mat4 mat4(void) {
	Mat4 result = {0};
	return result;
}

static inline Mat4 mat4_diagonal(f32 diagonal) {
	Mat4 result = {0};

	result.elements[0][0] = diagonal;
	result.elements[1][1] = diagonal;
	result.elements[2][2] = diagonal;
	result.elements[3][3] = diagonal;

	return result;
}

static inline Mat4 transpose_mat4(Mat4 mat) {
	Mat4 result;

#ifdef SIMD_USE_SSE
	result = mat;
	_MM_TRANSPOSE4_PS(result.columns[0].sse, result.columns[1].sse, result.columns[2].sse,
	                  result.columns[3].sse);
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
	result.sse = _mm_add_ps(
		result.sse, _mm_mul_ps(_mm_shuffle_ps(left.sse, left.sse, 0x55), right.columns[1].sse));
	result.sse = _mm_add_ps(
		result.sse, _mm_mul_ps(_mm_shuffle_ps(left.sse, left.sse, 0xaa), right.columns[2].sse));
	result.sse = _mm_add_ps(
		result.sse, _mm_mul_ps(_mm_shuffle_ps(left.sse, left.sse, 0xff), right.columns[3].sse));
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
	Vec3 b10 = sub_vec3(mul_vec3_f32(mat.columns[0].xyz, mat.columns[1].w),
	                    mul_vec3_f32(mat.columns[1].xyz, mat.columns[0].w));
	Vec3 b32 = sub_vec3(mul_vec3_f32(mat.columns[2].xyz, mat.columns[3].w),
	                    mul_vec3_f32(mat.columns[3].xyz, mat.columns[2].w));

	return dot_vec3(c01, b32) + dot_vec3(c23, b10);
}

// General-purpose inverse of a Mat4.
// Special-purpose inverses of many transformations are available in transform.h
// and will be more efficient.
static inline Mat4 inv_general_mat4(Mat4 mat) {
	Vec3 c01 = cross_vec3(mat.columns[0].xyz, mat.columns[1].xyz);
	Vec3 c23 = cross_vec3(mat.columns[2].xyz, mat.columns[3].xyz);
	Vec3 b10 = sub_vec3(mul_vec3_f32(mat.columns[0].xyz, mat.columns[1].w),
	                    mul_vec3_f32(mat.columns[1].xyz, mat.columns[0].w));
	Vec3 b32 = sub_vec3(mul_vec3_f32(mat.columns[2].xyz, mat.columns[3].w),
	                    mul_vec3_f32(mat.columns[3].xyz, mat.columns[2].w));

	f32 inv_det = 1.0f / (dot_vec3(c01, b32) + dot_vec3(c23, b10));
	c01 = mul_vec3_f32(c01, inv_det);
	c23 = mul_vec3_f32(c23, inv_det);
	b10 = mul_vec3_f32(b10, inv_det);
	b32 = mul_vec3_f32(b32, inv_det);

	Mat4 result;
	result.columns[0] = vec4_from_vec3(
		add_vec3(cross_vec3(mat.columns[1].xyz, b32), mul_vec3_f32(c23, mat.columns[1].w)),
		-dot_vec3(mat.columns[1].xyz, c23));
	result.columns[1] = vec4_from_vec3(
		sub_vec3(cross_vec3(b32, mat.columns[0].xyz), mul_vec3_f32(c23, mat.columns[0].w)),
		+dot_vec3(mat.columns[0].xyz, c23));
	result.columns[2] = vec4_from_vec3(
		add_vec3(cross_vec3(mat.columns[3].xyz, b10), mul_vec3_f32(c01, mat.columns[3].w)),
		-dot_vec3(mat.columns[3].xyz, c01));
	result.columns[3] = vec4_from_vec3(
		sub_vec3(cross_vec3(b10, mat.columns[2].xyz), mul_vec3_f32(c01, mat.columns[2].w)),
		+dot_vec3(mat.columns[2].xyz, c01));

	return transpose_mat4(result);
}

//
// Quat functions
//

static inline Quat quat(f32 x, f32 y, f32 z, f32 w) {
	Quat result;

#ifdef SIMD_USE_SSE
	result.sse = _mm_setr_ps(x, y, z, w);
#elif defined(SIMD_USE_NEON)
	float32x4_t v = {x, y, z, w};
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
	__m128 sse_res_one = _mm_xor_ps(_mm_shuffle_ps(left.sse, left.sse, _MM_SHUFFLE(0, 0, 0, 0)),
	                                _mm_setr_ps(0.f, -0.f, 0.f, -0.f));
	__m128 sse_res_two = _mm_shuffle_ps(right.sse, right.sse, _MM_SHUFFLE(0, 1, 2, 3));
	__m128 sse_res_three = _mm_mul_ps(sse_res_two, sse_res_one);

	sse_res_one = _mm_xor_ps(_mm_shuffle_ps(left.sse, left.sse, _MM_SHUFFLE(1, 1, 1, 1)),
	                         _mm_setr_ps(0.f, 0.f, -0.f, -0.f));
	sse_res_two = _mm_shuffle_ps(right.sse, right.sse, _MM_SHUFFLE(1, 0, 3, 2));
	sse_res_three = _mm_add_ps(sse_res_three, _mm_mul_ps(sse_res_two, sse_res_one));

	sse_res_one = _mm_xor_ps(_mm_shuffle_ps(left.sse, left.sse, _MM_SHUFFLE(2, 2, 2, 2)),
	                         _mm_setr_ps(-0.f, 0.f, 0.f, -0.f));
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
	result.neon =
		vfmaq_f32(result.neon, right_2301, vmulq_f32(vdupq_laneq_f32(left.neon, 1), second_sign));
	float32x4_t third_sign = {-1.0f, 1.0f, 1.0f, -1.0f};
	result.neon =
		vfmaq_f32(result.neon, right_1032, vmulq_f32(vdupq_laneq_f32(left.neon, 2), third_sign));
	result.neon = vfmaq_laneq_f32(result.neon, right.neon, left.neon, 3);

#else
	result.x = right.elements[3] * +left.elements[0];
	result.y = right.elements[2] * -left.elements[0];
	result.z = right.elements[1] * +left.elements[0];
	result.w = right.elements[0] * -left.elements[0];

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
	result.x = left.x * mix_left + right.x * mix_right;
	result.y = left.y * mix_left + right.y * mix_right;
	result.z = left.z * mix_left + right.z * mix_right;
	result.w = left.w * mix_left + right.w * mix_right;
#endif

	return result;
}

static inline Quat norm_lerp(Quat left, f32 t, Quat right) {
	Quat result = _mix_quat(left, 1.0f - t, right, t);
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

//
// Mat4 and Quat functions
//

static inline Mat4 mat4_from_quat(Quat left) {
	Mat4 result;

	Quat normalized_quat = norm_quat(left);

	f32 xx, yy, zz, xy, xz, yz, wx, wy, wz;

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
			q = quat(t, mat.elements[0][1] + mat.elements[1][0],
			         mat.elements[2][0] + mat.elements[0][2],
			         mat.elements[1][2] - mat.elements[2][1]);
		} else {
			t = 1 - mat.elements[0][0] + mat.elements[1][1] - mat.elements[2][2];
			q = quat(mat.elements[0][1] + mat.elements[1][0], t,
			         mat.elements[1][2] + mat.elements[2][1],
			         mat.elements[2][0] - mat.elements[0][2]);
		}
	} else {
		if (mat.elements[0][0] < -mat.elements[1][1]) {
			t = 1 - mat.elements[0][0] - mat.elements[1][1] + mat.elements[2][2];
			q = quat(mat.elements[2][0] + mat.elements[0][2],
			         mat.elements[1][2] + mat.elements[2][1], t,
			         mat.elements[0][1] - mat.elements[1][0]);
		} else {
			t = 1 + mat.elements[0][0] + mat.elements[1][1] + mat.elements[2][2];
			q = quat(mat.elements[1][2] - mat.elements[2][1],
			         mat.elements[2][0] - mat.elements[0][2],
			         mat.elements[0][1] - mat.elements[1][0], t);
		}
	}

	q = mul_quat_f32(q, 0.5f / sqrt_f32(t));

	return q;
}

static inline Quat quat_from_mat4_lh(Mat4 mat) {
	f32 t;
	Quat q;

	if (mat.elements[2][2] < 0.0f) {
		if (mat.elements[0][0] > mat.elements[1][1]) {
			t = 1 + mat.elements[0][0] - mat.elements[1][1] - mat.elements[2][2];
			q = quat(t, mat.elements[0][1] + mat.elements[1][0],
			         mat.elements[2][0] + mat.elements[0][2],
			         mat.elements[2][1] - mat.elements[1][2]);
		} else {
			t = 1 - mat.elements[0][0] + mat.elements[1][1] - mat.elements[2][2];
			q = quat(mat.elements[0][1] + mat.elements[1][0], t,
			         mat.elements[1][2] + mat.elements[2][1],
			         mat.elements[0][2] - mat.elements[2][0]);
		}
	} else {
		if (mat.elements[0][0] < -mat.elements[1][1]) {
			t = 1 - mat.elements[0][0] - mat.elements[1][1] + mat.elements[2][2];
			q = quat(mat.elements[2][0] + mat.elements[0][2],
			         mat.elements[1][2] + mat.elements[2][1], t,
			         mat.elements[1][0] - mat.elements[0][1]);
		} else {
			t = 1 + mat.elements[0][0] + mat.elements[1][1] + mat.elements[2][2];
			q = quat(mat.elements[2][1] - mat.elements[1][2],
			         mat.elements[0][2] - mat.elements[2][0],
			         mat.elements[1][0] - mat.elements[0][2], t);
		}
	}

	q = mul_quat_f32(q, 0.5f / sqrt_f32(t));

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

//
// Transform functions
//

// Produces a right-handed orthographic projection mat with z ranging from -1 to
// 1 (the GL convention). left, right, bottom, and top specify the coordinates
// of their respective clipping planes. near_plane and far_plane specify the
// distances to the near and far clipping planes.
static inline Mat4 orthographic_rh_no(f32 left, f32 right, f32 bottom, f32 top, f32 near_plane,
                                      f32 far_plane) {
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

// Produces a right-handed orthographic projection mat with z ranging from 0 to
// 1 (the DirectX convention). left, right, bottom, and top specify the
// coordinates of their respective clipping planes. near_plane and far_plane
// specify the distances to the near and far clipping planes.
static inline Mat4 orthographic_rh_zo(f32 left, f32 right, f32 bottom, f32 top, f32 near_plane,
                                      f32 far_plane) {
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

// Produces a left-handed orthographic projection mat with z ranging from -1 to
// 1 (the GL convention). left, right, bottom, and top specify the coordinates
// of their respective clipping planes. near_plane and far_plane specify the
// distances to the near and far clipping planes.
static inline Mat4 orthographic_lh_no(f32 left, f32 right, f32 bottom, f32 top, f32 near_plane,
                                      f32 far_plane) {
	Mat4 result = orthographic_rh_no(left, right, bottom, top, near_plane, far_plane);
	result.elements[2][2] = -result.elements[2][2];
	return result;
}

// Produces a left-handed orthographic projection mat with z ranging from 0 to 1
// (the DirectX convention). left, right, bottom, and top specify the
// coordinates of their respective clipping planes. near_plane and far_plane
// specify the distances to the near and far clipping planes.
static inline Mat4 orthographic_lh_zo(f32 left, f32 right, f32 bottom, f32 top, f32 near_plane,
                                      f32 far_plane) {
	Mat4 result = orthographic_rh_zo(left, right, bottom, top, near_plane, far_plane);
	result.elements[2][2] = -result.elements[2][2];
	return result;
}

// Returns an inverse for the given orthographic projection mat. Works for all
// orthographic projection matrices, regardless of handedness or NDC convention.
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

	// See
	// https://www.khronos.org/registry/OpenGL-Refpages/gl2.1/xhtml/gluPerspective.xml
	f32 cotangent = 1.0f / tanf(fov / 2.0f);
	result.elements[0][0] = cotangent / aspect_ratio;
	result.elements[1][1] = cotangent;
	result.elements[2][3] = -1.0f;

	result.elements[2][2] = (near_plane + far_plane) / (near_plane - far_plane);
	result.elements[3][2] = (2.0f * near_plane * far_plane) / (near_plane - far_plane);

	return result;
}

static inline Mat4 perspective_rh_zo(f32 fov, f32 aspect_ratio, f32 near_plane, f32 far_plane) {
	Mat4 result = {0};

	// See
	// https://www.khronos.org/registry/OpenGL-Refpages/gl2.1/xhtml/gluPerspective.xml
	f32 cotangent = 1.0f / tanf(fov / 2.0f);
	result.elements[0][0] = cotangent / aspect_ratio;
	result.elements[1][1] = cotangent;
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
	Mat4 result = perspective_rh_zo(fov, aspect_ratio, near_plane, far_plane);

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

	f32 sin_theta = sinf(angle);
	f32 cos_theta = cosf(angle);
	f32 cos_value = 1.0f - cos_theta;

	result.elements[0][0] = (axis.x * axis.x * cos_value) + cos_theta;
	result.elements[0][1] = (axis.x * axis.y * cos_value) + (axis.z * sin_theta);
	result.elements[0][2] = (axis.x * axis.z * cos_value) - (axis.y * sin_theta);

	result.elements[1][0] = (axis.y * axis.x * cos_value) - (axis.z * sin_theta);
	result.elements[1][1] = (axis.y * axis.y * cos_value) + cos_theta;
	result.elements[1][2] = (axis.y * axis.z * cos_value) + (axis.x * sin_theta);

	result.elements[2][0] = (axis.z * axis.x * cos_value) + (axis.y * sin_theta);
	result.elements[2][1] = (axis.z * axis.y * cos_value) - (axis.x * sin_theta);
	result.elements[2][2] = (axis.z * axis.z * cos_value) + cos_theta;

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

static inline Mat4 _look_at(Vec3 f, Vec3 s, Vec3 u, Vec3 eye) {
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
	Vec3 s = norm_vec3(cross_vec3(f, up));
	Vec3 u = cross_vec3(s, f);

	return _look_at(f, s, u, eye);
}

static inline Mat4 look_at_lh(Vec3 eye, Vec3 center, Vec3 up) {
	Vec3 f = norm_vec3(sub_vec3(eye, center));
	Vec3 s = norm_vec3(cross_vec3(f, up));
	Vec3 u = cross_vec3(s, f);

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
	result.elements[3][0] =
		-1.0f * mat.elements[3][0] /
		(rotation_mat.elements[0][0] + rotation_mat.elements[0][1] + rotation_mat.elements[0][2]);
	result.elements[3][1] =
		-1.0f * mat.elements[3][1] /
		(rotation_mat.elements[1][0] + rotation_mat.elements[1][1] + rotation_mat.elements[1][2]);
	result.elements[3][2] =
		-1.0f * mat.elements[3][2] /
		(rotation_mat.elements[2][0] + rotation_mat.elements[2][1] + rotation_mat.elements[2][2]);
	result.elements[3][3] = 1.0f;

	return result;
}

// Implementation from
// https://blog.molecular-matters.com/2013/05/24/a-faster-quaternion-vector-multiplication/
static inline Vec3 rotate_vec3_quat(Vec3 v, Quat q) {
	Vec3 t = mul_vec3_f32(cross_vec3(q.xyz, v), 2);
	return add_vec3(v, add_vec3(mul_vec3_f32(t, q.w), cross_vec3(q.xyz, t)));
}

static inline Vec3 rotate_vec3_axis_angle_lh(Vec3 v, Vec3 axis, f32 angle) {
	return rotate_vec3_quat(v, quat_from_axis_angle_lh(axis, angle));
}

static inline Vec3 rotate_vec3_axis_angle_rh(Vec3 v, Vec3 axis, f32 angle) {
	return rotate_vec3_quat(v, quat_from_axis_angle_rh(axis, angle));
}

#if C11_GENERICS_ENABLED

#define add(a, b)                                                                                  \
	_Generic((a),                                                                                  \
	    Vec2: add_vec2,                                                                            \
	    Vec2i: add_vec2i,                                                                          \
	    Vec3: add_vec3,                                                                            \
	    Vec4: add_vec4,                                                                            \
	    Mat2: add_mat2,                                                                            \
	    Mat3: add_mat3,                                                                            \
	    Mat4: add_mat4,                                                                            \
	    Quat: add_quat)(a, b)

#define sub(a, b)                                                                                  \
	_Generic((a),                                                                                  \
	    Vec2: sub_vec2,                                                                            \
	    Vec2i: sub_vec2i,                                                                          \
	    Vec3: sub_vec3,                                                                            \
	    Vec4: sub_vec4,                                                                            \
	    Mat2: sub_mat2,                                                                            \
	    Mat3: sub_mat3,                                                                            \
	    Mat4: sub_mat4,                                                                            \
	    Quat: sub_quat)(a, b)

#define mul(a, b)                                                                                  \
	_Generic((b),                                                                                  \
	    f32: _Generic((a),                                                                         \
	        Vec2: mul_vec2_f32,                                                                    \
	        Vec3: mul_vec3_f32,                                                                    \
	        Vec4: mul_vec4_f32,                                                                    \
	        Mat2: mul_mat2_f32,                                                                    \
	        Mat3: mul_mat3_f32,                                                                    \
	        Mat4: mul_mat4_f32,                                                                    \
	        Quat: mul_quat_f32),                                                                   \
	    Mat2: mul_mat2,                                                                            \
	    Mat3: mul_mat3,                                                                            \
	    Mat4: mul_mat4,                                                                            \
	    Quat: mul_quat,                                                                            \
	    default: _Generic((a),                                                                     \
	        Vec2: mul_vec2,                                                                        \
	        Vec3: mul_vec3,                                                                        \
	        Vec4: mul_vec4,                                                                        \
	        Mat2: mul_mat2_vec2,                                                                   \
	        Mat3: mul_mat3_vec3,                                                                   \
	        Mat4: mul_mat4_vec4))(a, b)

#define div(a, b)                                                                                  \
	_Generic((b),                                                                                  \
	    f32: _Generic((a),                                                                         \
	        Mat2: div_mat2_f32,                                                                    \
	        Mat3: div_mat3_f32,                                                                    \
	        Mat4: div_mat4_f32,                                                                    \
	        Vec2: div_vec2_f32,                                                                    \
	        Vec3: div_vec3_f32,                                                                    \
	        Vec4: div_vec4_f32,                                                                    \
	        Quat: div_quat_f32),                                                                   \
	    Mat2: div_mat2,                                                                            \
	    Mat3: div_mat3,                                                                            \
	    Mat4: div_mat4,                                                                            \
	    Quat: div_quat,                                                                            \
	    default: _Generic((a), Vec2: div_vec2, Vec3: div_vec3, Vec4: div_vec4))(a, b)

#define len(a) _Generic((a), Vec2: len_vec2, Vec3: len_vec3, Vec4: len_vec4)(a)

#define len_sqr(a) _Generic((a), Vec2: len_sqr_vec2, Vec3: len_sqr_vec3, Vec4: len_sqr_vec4)(a)

#define norm(a) _Generic((a), Vec2: norm_vec2, Vec3: norm_vec3, Vec4: norm_vec4)(a)

#define dot(a, b) _Generic((a), Vec2: dot_vec2, Vec3: dot_vec3, Vec4: dot_vec4)(a, b)

#define cross(a, b) _Generic((a), Vec3: cross_vec3)(a, b)

#define lerp(a, t, b)                                                                              \
	_Generic((a), f32: lerp_f32, Vec2: lerp_vec2, Vec3: lerp_vec3, Vec4: lerp_vec4)(a, t, b)

#define eq(a, b) _Generic((a), Vec2: eq_vec2, Vec3: eq_vec3, Vec4: eq_vec4)(a, b)

#define transpose(m)                                                                               \
	_Generic((m), Mat2: transpose_mat2, Mat3: transpose_mat3, Mat4: transpose_mat4)(m)

#define determinant(m)                                                                             \
	_Generic((m), Mat2: determinant_mat2, Mat3: determinant_mat3, Mat4: determinant_mat4)(m)

#define inv_general(m)                                                                             \
	_Generic((m), Mat2: inv_general_mat2, Mat3: inv_general_mat3, Mat4: inv_general_mat4)(m)

#endif // C11_GENERICS_ENABLED
