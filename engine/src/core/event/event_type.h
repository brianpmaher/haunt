#pragma once

#include "core/types.h"
#include "core/input/key_sym.h"
#include "core/input/mouse_button.h"

// Should never be fired by the application
typedef enum Event_Type {
	// Engine events
	EVENT_TYPE_APP_QUIT,
	EVENT_TYPE_WINDOW_CLOSE,
	EVENT_TYPE_WINDOW_RESIZE,
	EVENT_TYPE_KEY_PRESS,
	EVENT_TYPE_KEY_RELEASE,
	EVENT_TYPE_MOUSE_MOVE,
	EVENT_TYPE_MOUSE_SCROLL,
	EVENT_TYPE_MOUSE_BUTTON_PRESS,
	EVENT_TYPE_MOUSE_BUTTON_RELEASE,
	// Not an actual event code that is fired.  Application should define its own event code that starts here.
	EVENT_TYPE_CUSTOM,
} Event_Type;

// Using event code to allow for custom application events
typedef u16 Event_Code;

typedef struct App_Quit_Event {
	Event_Code code; // EVENT_TYPE_APP_QUIT
} App_Quit_Event;

typedef union App_Event {
	App_Quit_Event quit;
} App_Event;

typedef struct Window_Close_Event {
	Event_Code code; // EVENT_TYPE_WINDOW_CLOSE
} Window_Close_Event;

typedef struct Window_Resize_Event {
	Event_Code code; // EVENT_TYPE_WINDOW_RESIZE
	i32 width;
	i32 height;
} Window_Resize_Event;

typedef union Window_Event {
	Window_Close_Event close;
	Window_Resize_Event resize;
} Window_Event;

typedef struct Key_Press_Event {
	Event_Code code; // EVENT_TYPE_KEY_PRESS
	Key_Sym key;
	Key_Mod_Flags mods;
} Key_Press_Event;

typedef struct Key_Release_Event {
	Event_Code code; // EVENT_TYPE_KEY_RELEASE
	Key_Sym key;
	Key_Mod_Flags mods;
} Key_Release_Event;

typedef union Key_Event {
	Key_Press_Event press;
	Key_Release_Event release;
} Key_Event;

typedef union Mouse_Move_Event {
	Event_Code code; // EVENT_TYPE_MOUSE_MOVE
	i32 x;
	i32 y;
} Mouse_Move_Event;

typedef struct Mouse_Scroll_Event {
	Event_Code code; // EVENT_TYPE_MOUSE_SCROLL
	i32 delta;
} Mouse_Scroll_Event;

typedef struct Mouse_Button_Press_Event {
	Event_Code code; // EVENT_TYPE_MOUSE_BUTTON_PRESS
	Mouse_Button button;
} Mouse_Button_Press_Event;

typedef struct Mouse_Button_Release_Event {
	Event_Code code; // EVENT_TYPE_MOUSE_BUTTON_RELEASE
	Mouse_Button button;
} Mouse_Button_Release_Event;

typedef union Mouse_Event {
	Mouse_Scroll_Event scroll;
	Mouse_Button_Press_Event button_press;
	Mouse_Button_Release_Event button_release;
} Mouse_Event;

typedef union Event {
	Event_Code code;
	App_Event app;
	Window_Event window;
	Key_Event key;
	Mouse_Event mouse;
} Event;
