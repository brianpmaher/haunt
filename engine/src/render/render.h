#pragma once

#include "core/types.h"
#include "render/color.h"

#define RENDER_VSYNC_ENABLED 1
#define RENDER_SRGB_ENABLED  1
#define RENDER_MSAA_ENABLED  1

void render_system_init(void);

void render_system_resize(i32 width, i32 height);

void render_set_clear_color(Color color);

void render_clear(void);
