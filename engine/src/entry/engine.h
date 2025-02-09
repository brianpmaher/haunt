#pragma once

#include "core/export.h"

#include "entry/app.h"

export b8 _engine_init(const App_Config* config);

export b8 _engine_update(void);

export b8 _engine_render(void);

export void _engine_shutdown(void);

export b8 _engine_is_running(void);
