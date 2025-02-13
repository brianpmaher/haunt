#pragma once

#include "core/export.h"
#include "core/types.h"

typedef enum Key {
	// Modifiers
	KEY_BACKSPACE = 0x08,
	KEY_TAB = 0x09,
	KEY_CLEAR = 0x0C,
	KEY_RETURN = 0x0D,
	KEY_SHIFT = 0x10,
	KEY_CONTROL = 0x11,
	KEY_ALT = 0x12,
	KEY_PAUSE = 0x13,
	KEY_CAPS_LOCK = 0x14,
	KEY_ESCAPE = 0x1B,
	KEY_SPACE = 0x20,

	// Navigation
	KEY_PAGE_UP = 0x21,
	KEY_PAGE_DOWN = 0x22,
	KEY_END = 0x23,
	KEY_HOME = 0x24,

	// Arrows
	KEY_LEFT = 0x25,
	KEY_UP = 0x26,
	KEY_RIGHT = 0x27,
	KEY_DOWN = 0x28,

	// System
	KEY_SELECT = 0x29,
	KEY_PRINT = 0x2A,
	KEY_EXECUTE = 0x2B,
	KEY_PRINTSCREEN = 0x2C,
	KEY_INSERT = 0x2D,
	KEY_DELETE = 0x2E,
	KEY_HELP = 0x2F,

	// Numbers
	KEY_0 = 0x30,
	KEY_1 = 0x31,
	KEY_2 = 0x32,
	KEY_3 = 0x33,
	KEY_4 = 0x34,
	KEY_5 = 0x35,
	KEY_6 = 0x36,
	KEY_7 = 0x37,
	KEY_8 = 0x38,
	KEY_9 = 0x39,

	// Letters
	KEY_A = 0x41,
	KEY_B = 0x42,
	KEY_C = 0x43,
	KEY_D = 0x44,
	KEY_E = 0x45,
	KEY_F = 0x46,
	KEY_G = 0x47,
	KEY_H = 0x48,
	KEY_I = 0x49,
	KEY_J = 0x4A,
	KEY_K = 0x4B,
	KEY_L = 0x4C,
	KEY_M = 0x4D,
	KEY_N = 0x4E,
	KEY_O = 0x4F,
	KEY_P = 0x50,
	KEY_Q = 0x51,
	KEY_R = 0x52,
	KEY_S = 0x53,
	KEY_T = 0x54,
	KEY_U = 0x55,
	KEY_V = 0x56,
	KEY_W = 0x57,
	KEY_X = 0x58,
	KEY_Y = 0x59,
	KEY_Z = 0x5A,

	// Numpad
	KEY_PAD_0 = 0x60,
	KEY_PAD_1 = 0x61,
	KEY_PAD_2 = 0x62,
	KEY_PAD_3 = 0x63,
	KEY_PAD_4 = 0x64,
	KEY_PAD_5 = 0x65,
	KEY_PAD_6 = 0x66,
	KEY_PAD_7 = 0x67,
	KEY_PAD_8 = 0x68,
	KEY_PAD_9 = 0x69,
	KEY_PAD_MULTIPLY = 0x6A,
	KEY_PAD_ADD = 0x6B,
	KEY_PAD_SEPARATOR = 0x6C,
	KEY_PAD_SUBTRACT = 0x6D,
	KEY_PAD_DECIMAL = 0x6E,
	KEY_PAD_DIVIDE = 0x6F,

	// Function keys
	KEY_F1 = 0x70,
	KEY_F2 = 0x71,
	KEY_F3 = 0x72,
	KEY_F4 = 0x73,
	KEY_F5 = 0x74,
	KEY_F6 = 0x75,
	KEY_F7 = 0x76,
	KEY_F8 = 0x77,
	KEY_F9 = 0x78,
	KEY_F10 = 0x79,
	KEY_F11 = 0x7A,
	KEY_F12 = 0x7B,
	KEY_F13 = 0x7C,
	KEY_F14 = 0x7D,
	KEY_F15 = 0x7E,
	KEY_F16 = 0x7F,
	KEY_F17 = 0x80,
	KEY_F18 = 0x81,
	KEY_F19 = 0x82,
	KEY_F20 = 0x83,
	KEY_F21 = 0x84,
	KEY_F22 = 0x85,
	KEY_F23 = 0x86,
	KEY_F24 = 0x87,

	KEY_COUNT,
} Key;

typedef enum Mouse_Button {
	MOUSE_BUTTON_LEFT,
	MOUSE_BUTTON_MIDDLE,
	MOUSE_BUTTON_RIGHT,

	MOUSE_BUTTON_COUNT,
} Mouse_Button;

void input_update(void);

void input_process_key(Key key, b8 pressed);

void input_process_mouse_button(Mouse_Button button, b8 pressed);

void input_process_mouse_wheel(i32 y);

void input_process_mouse_position(i32 x, i32 y);

export b8 input_is_key_pressed(Key key);

export b8 input_is_key_released(Key key);

export b8 input_is_key_down(Key key);

export b8 input_is_key_up(Key key);

export b8 input_is_mouse_button_pressed(Mouse_Button button);

export b8 input_is_mouse_button_released(Mouse_Button button);

export b8 input_is_mouse_button_down(Mouse_Button button);

export b8 input_is_mouse_button_up(Mouse_Button button);

export i32 input_get_mouse_wheel(void);

export void input_get_mouse_position(i32* x, i32* y);

export void input_get_mouse_position_delta(i32* x, i32* y);
