#pragma once

#include "core/types.h"

#include "math/vector2.h"

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
