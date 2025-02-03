#pragma once

#include "core/api.h"

#include "entry/app.h"

HAPI b8 _engine_init(const App_Config* config);

HAPI b8 _engine_update(void);

HAPI b8 _engine_render(void);

HAPI void _engine_shutdown(void);

HAPI b8 _engine_is_running(void);
