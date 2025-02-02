#pragma once

#include "core/types.h"
#include "core/event/event_type.h"
#include "core/input/mouse_button.h"

typedef union Mouse_Move_Event {
	Event_Code code; // EVENT_TYPE_MOUSE_MOVE
	i32 x;
	i32 y;
	i32 delta_x;
	i32 delta_y;
} Mouse_Move_Event;

typedef struct Mouse_Wheel_Event {
	Event_Code code; // EVENT_TYPE_MOUSE_WHEEL
	i32 delta;
} Mouse_Wheel_Event;

typedef struct Mouse_Button_Press_Event {
	Event_Code code; // EVENT_TYPE_MOUSE_BUTTON_PRESS
	Mouse_Button button;
} Mouse_Button_Press_Event;

typedef struct Mouse_Button_Release_Event {
	Event_Code code; // EVENT_TYPE_MOUSE_BUTTON_RELEASE
	Mouse_Button button;
} Mouse_Button_Release_Event;

Mouse_Move_Event mouse_move_event_create(i32 x, i32 y, i32 delta_x, i32 delta_y);

Mouse_Wheel_Event mouse_wheel_event_create(i32 delta);

Mouse_Button_Press_Event mouse_button_press_event_create(Mouse_Button button);

Mouse_Button_Release_Event mouse_button_release_event_create(Mouse_Button button);
