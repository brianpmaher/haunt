#pragma once

#include "haunt/core/math.h"
#include "haunt/core/types.h"

#define GRAPHICS_DEBUG_ENABLED 1
#define GRAPHICS_SRGB_ENABLED  1

typedef Vec4 Color;

typedef struct Graphics_Settings {
	b8 vsync;
	b8 antialias;
} Graphics_Settings;
