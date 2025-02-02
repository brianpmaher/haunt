#pragma once

#include "core/types.h"

typedef enum Event_Type {
	// Engine events
	EVENT_TYPE_APP_QUIT,
	EVENT_TYPE_WINDOW_CLOSE,
	EVENT_TYPE_WINDOW_RESIZE,
	EVENT_TYPE_KEY_PRESS,
	EVENT_TYPE_KEY_RELEASE,
	EVENT_TYPE_MOUSE_MOVE,
	EVENT_TYPE_MOUSE_WHEEL,
	EVENT_TYPE_MOUSE_BUTTON_PRESS,
	EVENT_TYPE_MOUSE_BUTTON_RELEASE,
	// Begin custom application events
	EVENT_TYPE_CUSTOM = 256,
	// End custom application events
	EVENT_TYPE_MAX = 512
} Event_Type;

// Using event code to allow for custom application events
typedef u16 Event_Code;
