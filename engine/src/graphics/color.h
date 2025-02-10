#pragma once

#include "math/types.h"
#include "math/vec4.h"

typedef Vec4 Color;

static inline Color color(f32 r, f32 g, f32 b, f32 a) {
	return vec4(r, g, b, a);
}

static inline Color color_rgb(f32 r, f32 g, f32 b) {
	return color(r, g, b, 1);
}

static inline Color color_rgba(f32 r, f32 g, f32 b, f32 a) {
	return color(r, g, b, a);
}
