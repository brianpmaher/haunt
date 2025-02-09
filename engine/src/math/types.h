#pragma once

#include "core/types.h"
#include "core/simd.h"

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
