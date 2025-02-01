#pragma once

#include "core/type.h"

typedef enum Event_Type {
	EVENT_TYPE_APP_QUIT,
	EVENT_TYPE_WINDOW_CLOSE,
	EVENT_TYPE_WINDOW_RESIZE,
	EVENT_TYPE_KEY_PRESS,
	EVENT_TYPE_KEY_RELEASE,
	EVENT_TYPE_MOUSE_MOVE,
	EVENT_TYPE_MOUSE_SCROLL,
	EVENT_TYPE_MOUSE_BUTTON_PRESS,
	EVENT_TYPE_MOUSE_BUTTON_RELEASE,
} Event_Type;

typedef struct App_Quit_Event {
	Event_Type type; // EVENT_TYPE_APP_QUIT,
} App_Quit_Event;

typedef union App_Event {
	App_Quit_Event quit;
} App_Event;

typedef struct Window_Close_Event {
	Event_Type type; // EVENT_TYPE_WINDOW_CLOSE
} Window_Close_Event;

typedef struct Window_Resize_Event {
	Event_Type type; // EVENT_TYPE_WINDOW_RESIZE
	i32 width;
	i32 height;
} Window_Resize_Event;

typedef union Window_Event {
	Window_Close_Event close;
	Window_Resize_Event resize;
} Window_Event;

typedef struct Key_Press_Event {
	Event_Type type; // EVENT_TYPE_KEY_PRESS
	i32 key;
} Key_Press_Event;

typedef struct Key_Release_Event {
	Event_Type type; // EVENT_TYPE_KEY_RELEASE
	i32 key;
} Key_Release_Event;

typedef union Event {
	Event_Type type;
	App_Event app;
	Window_Event window;
} Event;
