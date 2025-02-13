#include "input.h"

// TODO: Use bit fields instead of array of bytes for these boolean states

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

typedef struct Input_State {
	Key_State key;
	Mouse_State mouse;
} Input_State;

static Input_State input = {0};

void input_update(void) {
	// Reset pressed and released
	memory_zero(input.key.pressed, sizeof(input.key.pressed));
	memory_zero(input.key.released, sizeof(input.key.released));
	memory_zero(input.mouse.pressed, sizeof(input.mouse.pressed));
	memory_zero(input.mouse.released, sizeof(input.mouse.released));

	// Update prev positions
	input.mouse.position.prev_x = input.mouse.position.x;
	input.mouse.position.prev_y = input.mouse.position.y;

	// Reset wheel
	input.mouse.wheel = 0;
}

void input_process_key(Key key, b8 pressed) {
	if (!input.key.down[key]) {
		input.key.pressed[key] = pressed;
	}
	input.key.released[key] = !pressed;
	input.key.down[key] = pressed;
}

void input_process_mouse_button(Mouse_Button button, b8 pressed) {
	input.mouse.pressed[button] = pressed;
	input.mouse.released[button] = !pressed;
	input.mouse.down[button] = pressed;
}

void input_process_mouse_wheel(i32 y) {
	input.mouse.wheel = y;
}

void input_process_mouse_position(i32 x, i32 y) {
	input.mouse.position.x = x;
	input.mouse.position.y = y;
}

b8 is_key_pressed(Key key) {
	return input.key.pressed[key];
}

b8 is_key_released(Key key) {
	return input.key.released[key];
}

b8 is_key_down(Key key) {
	return input.key.down[key];
}

b8 is_key_up(Key key) {
	return !input.key.down[key];
}

b8 is_mouse_button_pressed(Mouse_Button button) {
	return input.mouse.pressed[button];
}

b8 is_mouse_button_released(Mouse_Button button) {
	return input.mouse.released[button];
}

b8 is_mouse_button_down(Mouse_Button button) {
	return input.mouse.down[button];
}

b8 is_mouse_button_up(Mouse_Button button) {
	return !input.mouse.down[button];
}

i32 get_mouse_wheel(void) {
	return input.mouse.wheel;
}

void get_mouse_position(i32* x, i32* y) {
	*x = input.mouse.position.x;
	*y = input.mouse.position.y;
}

void get_mouse_position_delta(i32* x, i32* y) {
	*x = input.mouse.position.x - input.mouse.position.prev_x;
	*y = input.mouse.position.y - input.mouse.position.prev_y;
}
