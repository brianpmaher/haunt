#ifndef HAUNT_CORE_GRAPHICS_H
#define HAUNT_CORE_GRAPHICS_H

#include "haunt/core/types.h"

#define GRAPHICS_DEBUG_ENABLED 1
#define GRAPHICS_SRGB_ENABLED  1

typedef struct Graphics_Settings {
	b8 vsync;
	b8 antialias;
} Graphics_Settings;

#endif // HAUNT_CORE_GRAPHICS_H
