#pragma once

#include "core/types.h"

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
