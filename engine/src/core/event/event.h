#pragma once

#include "core/event/event_type.h"
#include "core/event/app_event.h"
#include "core/event/window_event.h"
#include "core/event/key_event.h"
#include "core/event/mouse_event.h"

typedef union Event {
	Event_Code code;
	App_Quit_Event app_quit;
	Window_Close_Event window_close;
	Window_Resize_Event window_resize;
	Key_Press_Event key_press;
	Key_Release_Event key_release;
	Mouse_Move_Event mouse_move;
	Mouse_Wheel_Event mouse_wheel;
	Mouse_Button_Press_Event mouse_button_press;
	Mouse_Button_Release_Event mouse_button_release;
} Event;

typedef b8 (*On_Event)(Event* event, void* sender, void* listener);

b8 event_system_init(void);

void event_system_shutdown(void);

HAPI b8 event_register(Event_Code code, void* listener, On_Event on_event);

HAPI b8 event_unregister(Event_Code code, void* listener, On_Event on_event);

HAPI b8 event_fire(Event event, void* sender);
