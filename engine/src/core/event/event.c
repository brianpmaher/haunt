#include "core/event/event.h"

#include "core/log.h"

#define EVENT_CODE_MAX 512
#define EVENT_CODE_ENTRY_MAX 512

typedef struct Registered_Event {
	void* listener;
	On_Event on_event;
} Registered_Event;

typedef struct Event_Code_Entry {
	// TODO: Use a darray
	Registered_Event events[EVENT_CODE_ENTRY_MAX];
	u32 event_count;
} Event_Code_Entry;

typedef struct Event_System {
	Event_Code_Entry entries[EVENT_CODE_MAX];
	u32 entry_count;
} Event_System;

static Event_System event_system = {0};

b8 event_system_init(void) {
	return true;
}

void event_system_shutdown(void) {
}

// TODO: Implement
HAPI b8 event_register(Event_Code code, void* listener, On_Event on_event) {
	if (code >= EVENT_CODE_MAX) {
		log_error("Event code %d is out of range", code);
		return false;
	}

	Event_Code_Entry* entry = &event_system.entries[code];
	if (entry->event_count >= EVENT_CODE_ENTRY_MAX) {
		log_error("Event code entry %d is full", code);
		return false;
	}

	for (u32 i = 0; i < entry->event_count; i++) {
		Registered_Event* event = &entry->events[i];
		if (event->listener == listener && event->on_event == on_event) {
			log_error("Event code %d already registered", code);
			return false;
		}
	}

	Registered_Event* event = &entry->events[entry->event_count];
	event->listener = listener;
	event->on_event = on_event;
	entry->event_count++;

	return true;
}

// TODO: Implement
HAPI b8 event_unregister(Event_Code code, void* listener, On_Event on_event) {
	if (code >= EVENT_CODE_MAX) {
		log_error("Event code %d is out of range", code);
		return false;
	}

	Event_Code_Entry* entry = &event_system.entries[code];
	b8 found = false;
	for (u32 i = 0; i < entry->event_count; i++) {
		Registered_Event* event = &entry->events[i];
		if (event->listener == listener && event->on_event == on_event) {
			found = true;
			// TODO: use darray_unordered_remove_at
			entry->events[i] = entry->events[entry->event_count - 1];
			entry->event_count--;
			break;
		}
	}

	if (!found) {
		log_error("Event code %d not found", code);
		return false;
	}

	return true;
}

// TODO: Implement
HAPI b8 event_fire(Event_Code code, Event event, void* sender) {
	return true;
}
