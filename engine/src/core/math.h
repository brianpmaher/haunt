#pragma once

#include "core/types.h"
#include "core/simd.h"

#include <math.h>
#include <float.h>

//
// Constants
//

#define e               2.71828182845904523536   // e
#define log2e           1.44269504088896340736   // log2(e)
#define log10e          0.434294481903251827651  // log10(e)
#define ln2             0.693147180559945309417  // ln(2)
#define ln10            2.30258509299404568402   // ln(10)
#define pi              3.14159265358979323846   // pi
#define pi_over_two     1.57079632679489661923   // pi/2
#define pi_over_four    0.785398163397448309616  // pi/4
#define one_over_pi     0.318309886183790671538  // 1/pi
#define two_over_pi     0.636619772367581343076  // 2/pi
#define two_over_sqrtpi 1.12837916709551257390   // 2/sqrt(pi)
#define sqrt2           1.41421356237309504880   // sqrt(2)
#define one_over_sqrt2  0.707106781186547524401  // 1/sqrt(2)

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
// Angle units
//

#define rad_to_deg(x)  (180.0 / pi)
#define rad_to_turn(x) (0.5 / pi)
#define deg_to_rad(x)  (pi / 180.0)
#define deg_to_turn(x) (0.5 / 180.0)
#define turn_to_rad(x) (pi / 0.5)
#define turn_to_deg(x) (180.0 / 0.5)

//
// Utils
//

#define abs(a) ((a) > 0 ? (a) : -(a))
#define mod(a, m) (((a) % (m)) >= 0 ? ((a) % (m)) : (((a) % (m)) + (m)))
#define square(x) ((x) * (x))

//
// Types
//

typedef union Vector2 {
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
} Vector2;

typedef union Vector3 {
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
		Vector2 xy;
		f32 _ignored0;
	};
	struct {
		f32 _ignored1;
		Vector2 yz;
	};
	struct {
		Vector2 uv;
		f32 _ignored2;
	};
	struct {
		f32 _ignored3;
		Vector2 vw;
	};
	f32 elements[3];
} Vector3;

typedef union Vector4 {
	struct {
		union {
			Vector3 xyz;
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
			Vector3 rgb;
			struct {
				f32 r;
				f32 g;
				f32 b;
			};
		};
		f32 a;
	};
	struct {
		Vector2 xy;
		f32 _ignored0;
		f32 _ignored1;
	};
	struct {
		f32 _ignored2;
		Vector2 yz;
		f32 _ignored3;
	};
	struct {
		f32 _ignored4;
		f32 _ignored5;
		Vector2 zw;
	};
	f32 elements[4];

#ifdef SIMD_USE_SSE
	__m128 sse;
#endif

#ifdef SIMD_USE_NEON
	float32x4_t neon;
#endif
} Vector4;
