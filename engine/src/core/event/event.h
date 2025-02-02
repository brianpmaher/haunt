#pragma once

#include "core/event/event_type.h"

typedef b8 (*On_Event)(Event* event, void* sender, void* listener);

b8 event_system_init(void);

void event_system_shutdown(void);

HAPI b8 event_register(Event_Code code, void* listener, On_Event on_event);

HAPI b8 event_unregister(Event_Code code, void* listener, On_Event on_event);

HAPI b8 event_fire(Event_Code code, Event event, void* sender);
