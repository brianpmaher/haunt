#pragma once

#include "core/api.h"
#include "core/types.h"
#include "core/input/key.h"

void input_system_init(void);

void input_system_shutdown(void);

HAPI b8 input_is_key_pressed(Key key);

HAPI b8 input_is_key_released(Key key);

HAPI b8 input_is_key_down(Key key);

HAPI b8 input_is_key_up(Key key);
