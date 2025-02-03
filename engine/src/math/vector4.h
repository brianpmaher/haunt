#pragma once

#include "core/types.h"
#include "core/simd.h"

#include "math/vector2.h"
#include "math/vector3.h"

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
