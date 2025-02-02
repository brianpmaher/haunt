#pragma once

#include "core/types.h"
#include "core/input/key_sym.h"
#include "core/input/mouse_button.h"

typedef enum Core_Event_Type {
	// Engine events
	CORE_EVENT_TYPE_APP_QUIT,
	CORE_EVENT_TYPE_WINDOW_CLOSE,
	CORE_EVENT_TYPE_WINDOW_RESIZE,
	CORE_EVENT_TYPE_KEY_PRESS,
	CORE_EVENT_TYPE_KEY_RELEASE,
	CORE_EVENT_TYPE_MOUSE_MOVE,
	CORE_EVENT_TYPE_MOUSE_SCROLL,
	CORE_EVENT_TYPE_MOUSE_BUTTON_PRESS,
	CORE_EVENT_TYPE_MOUSE_BUTTON_RELEASE,
	// Not an actual event type that is fired.  Application should define its own event type that starts here.
	CORE_EVENT_TYPE_CUSTOM,
} Core_Event_Type;

// Using event type to allow for custom application events
typedef u16 Event_Type;

typedef struct App_Quit_Event {
	Event_Type type; // EVENT_TYPE_APP_QUIT
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
	Key_Sym key;
	Key_Mod_Flags mods;
} Key_Press_Event;

typedef struct Key_Release_Event {
	Event_Type type; // EVENT_TYPE_KEY_RELEASE
	Key_Sym key;
	Key_Mod_Flags mods;
} Key_Release_Event;

typedef union Key_Event {
	Key_Press_Event press;
	Key_Release_Event release;
} Key_Event;

typedef union Mouse_Move_Event {
	Event_Type type; // EVENT_TYPE_MOUSE_MOVE
	i32 x;
	i32 y;
} Mouse_Move_Event;

typedef struct Mouse_Scroll_Event {
	Event_Type type; // EVENT_TYPE_MOUSE_SCROLL
	i32 delta;
} Mouse_Scroll_Event;

typedef struct Mouse_Button_Press_Event {
	Event_Type type; // EVENT_TYPE_MOUSE_BUTTON_PRESS
	Mouse_Button button;
} Mouse_Button_Press_Event;

typedef struct Mouse_Button_Release_Event {
	Event_Type type; // EVENT_TYPE_MOUSE_BUTTON_RELEASE
	Mouse_Button button;
} Mouse_Button_Release_Event;

typedef union Mouse_Event {
	Mouse_Scroll_Event scroll;
	Mouse_Button_Press_Event button_press;
	Mouse_Button_Release_Event button_release;
} Mouse_Event;

typedef struct Custom_Event {
	Event_Type type;
	void* data;
} Custom_Event;

typedef union Event {
	Event_Type type;
	App_Event app;
	Window_Event window;
	Key_Event key;
	Mouse_Event mouse;
	Custom_Event custom;
} Event;

typedef b8 (*On_Event)(Event* event);

HAPI b8 event_subscribe(Event_Type type, On_Event on_event);

HAPI b8 event_unsubscribe(Event_Type type, On_Event on_event);
