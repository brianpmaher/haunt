#include "core/input/input.h"

void input_system_init(void) {
}

void input_system_shutdown(void){
}

void input_system_update_key(Key key, b8 pressed){
}

void input_system_update_mouse_button(Mouse_Button button, b8 pressed){
}

void input_system_update_mouse_wheel(i32 y){
}

void input_system_udpate_mouse_position(i32 x, i32 y){
}

// Key pressed this frame
b8 input_is_key_pressed(Key key){
	return false;
}

// Key released this frame
b8 input_is_key_released(Key key){
	return false;
}

// Key is down
b8 input_is_key_down(Key key){
	return false;
}

// Key is up
b8 input_is_key_up(Key key){
	return false;
}

// Key pressed this frame
b8 input_is_mouse_button_pressed(Mouse_Button button){
	return false;
}

// Key released this frame
b8 input_is_mouse_button_released(Mouse_Button button){
	return false;
}

// Key is down
b8 input_is_mouse_button_down(Mouse_Button button){
	return false;
}

// Key is up
b8 input_is_mouse_button_up(Mouse_Button button){
	return false;
}

i32 input_get_mouse_wheel_delta(void){
	return 0;
}

void input_get_mouse_position(i32* x, i32* y){
}

void input_get_mouse_position_delta(i32* x, i32* y){
}
