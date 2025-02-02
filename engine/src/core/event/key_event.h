#pragma once

#include "core/event/event_type.h"
#include "core/input/key.h"

typedef struct Key_Press_Event {
	Event_Code code; // EVENT_TYPE_KEY_PRESS
	Key key;
} Key_Press_Event;

typedef struct Key_Release_Event {
	Event_Code code; // EVENT_TYPE_KEY_RELEASE
	Key key;
} Key_Release_Event;

Key_Press_Event key_press_event_create(Key key);

Key_Release_Event key_release_event_create(Key key);
