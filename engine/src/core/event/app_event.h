#pragma once

#include "core/event/event_type.h"

typedef struct App_Quit_Event {
	Event_Code code; // EVENT_TYPE_APP_QUIT
} App_Quit_Event;

App_Quit_Event app_quit_event_create(void);
