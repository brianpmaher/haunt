#pragma once

#include "core/event/event_type.h"

typedef b8 (*On_Event)(Event* event, void* sender, void* listener);

HAPI b8 event_register(Event_Code code, On_Event on_event, void* listener);

HAPI b8 event_unregister(Event_Code code, On_Event on_event, void* listener);

HAPI b8 event_fire(Event_Code code, Event event, void* sender);
