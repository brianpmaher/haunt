#pragma once

#include "core/api.h"
#include "core/types.h"
#include "core/input/key.h"
#include "core/input/mouse_button.h"

void input_system_init(void);

void input_system_shutdown(void);

void input_system_update(void);

void input_system_update_key(Key key, b8 pressed);

void input_system_update_mouse_button(Mouse_Button button, b8 pressed);

void input_system_update_mouse_wheel(i32 y);

void input_system_update_mouse_position(i32 x, i32 y);

// Key pressed this frame
HAPI b8 input_is_key_pressed(Key key);

// Key released this frame
HAPI b8 input_is_key_released(Key key);

// Key is down
HAPI b8 input_is_key_down(Key key);

// Key is up
HAPI b8 input_is_key_up(Key key);

// Key pressed this frame
HAPI b8 input_is_mouse_button_pressed(Mouse_Button button);

// Key released this frame
HAPI b8 input_is_mouse_button_released(Mouse_Button button);

// Key is down
HAPI b8 input_is_mouse_button_down(Mouse_Button button);

// Key is up
HAPI b8 input_is_mouse_button_up(Mouse_Button button);

HAPI i32 input_get_mouse_wheel_delta(void);

HAPI void input_get_mouse_position(i32* x, i32* y);

HAPI void input_get_mouse_position_delta(i32* x, i32* y);
