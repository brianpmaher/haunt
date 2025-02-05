#pragma once

#include "core/types.h"

typedef enum Event_Type {
	// Engine events
	EVENT_TYPE_APP_QUIT, // ()
	EVENT_TYPE_WINDOW_CLOSE, // ()
	EVENT_TYPE_WINDOW_RESIZE, // (i32 x, i32 y)
	EVENT_TYPE_KEY_PRESS, // (i32 key)
	EVENT_TYPE_KEY_RELEASE, // (i32 key)
	EVENT_TYPE_MOUSE_MOVE, // (i32 x, i32 y)
	EVENT_TYPE_MOUSE_WHEEL, // (i32 y)
	EVENT_TYPE_MOUSE_BUTTON_PRESS, // (i32 button)
	EVENT_TYPE_MOUSE_BUTTON_RELEASE, // (i32 button)
	// Begin custom application events
	EVENT_TYPE_CUSTOM = 256,
	// End custom application events
	EVENT_TYPE_MAX = 512
} Event_Type;

// Using event code to allow for custom application events
typedef u32 Event_Code;

// TODO: Event context is currently very specific to the engine events, maybe add a pointer type or something to make it useful for other event types.
typedef union Event_Context {
	i32 vals[4];
} Event_Context;

typedef b8 (*On_Event)(Event_Code code, Event_Context* context, void* sender, void* listener);

b8 event_system_init(void);

void event_system_shutdown(void);

export b8 event_register(Event_Code code, void* listener, On_Event on_event);

export b8 event_unregister(Event_Code code, void* listener, On_Event on_event);

export b8 event_fire(Event_Code code, Event_Context context, void* sender);
