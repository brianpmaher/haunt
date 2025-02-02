#pragma once

#include "core/types.h"
#include "core/event/event_type.h"

typedef struct Window_Close_Event {
	Event_Code code; // EVENT_TYPE_WINDOW_CLOSE
} Window_Close_Event;

typedef struct Window_Resize_Event {
	Event_Code code; // EVENT_TYPE_WINDOW_RESIZE
	i32 width;
	i32 height;
} Window_Resize_Event;

Window_Close_Event window_close_event_create(void);

Window_Resize_Event window_resize_event_create(i32 width, i32 height);
