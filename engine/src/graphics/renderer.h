#pragma once

#include "core/types.h"
#include "graphics/color.h"

#define RENDER_VSYNC_ENABLED 1
#define RENDER_SRGB_ENABLED  1
#define RENDER_MSAA_ENABLED  1

void renderer_init(void);

void renderer_resize(i32 width, i32 height);

void set_clear_color(Color color);

void clear_screen(void);
