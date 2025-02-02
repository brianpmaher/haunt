#include "core/input/input.h"

#include "core/memory.h"

typedef struct Key_State {
	b8 pressed[KEY_COUNT];
	b8 released[KEY_COUNT];
	b8 down[KEY_COUNT];
} Key_State;

typedef struct Mouse_Position {
	i32 x;
	i32 y;
	i32 prev_x;
	i32 prev_y;
} Mouse_Position;

typedef struct Mouse_State {
	b8 pressed[MOUSE_BUTTON_COUNT];
	b8 released[MOUSE_BUTTON_COUNT];
	b8 down[MOUSE_BUTTON_COUNT];
	i32 wheel;
	Mouse_Position position;
} Mouse_State;

typedef struct Input_System {
	Key_State key;
	Mouse_State mouse;
} Input_System;

static Input_System input_system = {0};

void input_system_init(void) {
}

void input_system_shutdown(void) {
}

void input_system_update(void) {
	memory_zero(input_system.key.pressed, sizeof(input_system.key.pressed));
	memory_zero(input_system.key.released, sizeof(input_system.key.released));
	memory_zero(input_system.mouse.pressed, sizeof(input_system.mouse.pressed));
	memory_zero(input_system.mouse.released, sizeof(input_system.mouse.released));

	input_system.mouse.position.x = input_system.mouse.position.prev_x;
	input_system.mouse.position.y = input_system.mouse.position.prev_y;
}

void input_system_update_key(Key key, b8 pressed) {
	input_system.key.pressed[key] = pressed;
	input_system.key.released[key] = !pressed;
	input_system.key.down[key] = pressed;
}

void input_system_update_mouse_button(Mouse_Button button, b8 pressed) {
	input_system.mouse.pressed[button] = pressed;
	input_system.mouse.released[button] = !pressed;
	input_system.mouse.down[button] = pressed;
}

void input_system_update_mouse_wheel(i32 y) {
	input_system.mouse.wheel = y;
}

void input_system_update_mouse_position(i32 x, i32 y) {
	input_system.mouse.position.x = x;
	input_system.mouse.position.y = y;
}

b8 input_is_key_pressed(Key key) {
	return input_system.key.pressed[key];
}

b8 input_is_key_released(Key key) {
	return input_system.key.released[key];
}

b8 input_is_key_down(Key key) {
	return input_system.key.down[key];
}

b8 input_is_key_up(Key key) {
	return !input_system.key.down[key];
}

b8 input_is_mouse_button_pressed(Mouse_Button button) {
	return input_system.mouse.pressed[button];
}

b8 input_is_mouse_button_released(Mouse_Button button) {
	return input_system.mouse.released[button];
}

b8 input_is_mouse_button_down(Mouse_Button button) {
	return input_system.mouse.down[button];
}

b8 input_is_mouse_button_up(Mouse_Button button) {
	return !input_system.mouse.down[button];
}

i32 input_get_mouse_wheel(void) {
	return input_system.mouse.wheel;
}

void input_get_mouse_position(i32* x, i32* y) {
	*x = input_system.mouse.position.x;
	*y = input_system.mouse.position.y;
}

void input_get_mouse_position_delta(i32* x, i32* y) {
	*x = input_system.mouse.position.x - input_system.mouse.position.prev_x;
	*y = input_system.mouse.position.y - input_system.mouse.position.prev_y;
}
