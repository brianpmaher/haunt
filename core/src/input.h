#ifndef HAUNT_CORE_INTERNAL_INPUT_H
#define HAUNT_CORE_INTERNAL_INPUT_H

#include "haunt/core/input.h"

void input_update(void);

void input_process_key(Input_Key key, b8 pressed);

void input_process_mouse_button(Mouse_Button button, b8 pressed);

void input_process_mouse_wheel(i32 y);

void input_process_mouse_position(i32 x, i32 y);

#endif // HAUNT_CORE_INTERNAL_INPUT_H
