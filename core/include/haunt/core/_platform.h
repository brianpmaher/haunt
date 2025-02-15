#pragma once

#include "haunt/core/assert.h"
#include "haunt/core/types.h"

export b8 _platform_init(const char* app_name);

export void _platform_shutdown(void);

export b8 _platform_pump_messages(void);

export b8 _platform_swap_buffers(void);
