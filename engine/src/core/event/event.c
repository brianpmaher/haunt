#include "core/event/event.h"

#include "core/log.h"

#define EVENT_CODE_MAX EVENT_TYPE_MAX
#define EVENT_CODE_ENTRY_MAX 512

typedef struct Registered_Event {
	void* listener;
	On_Event on_event;
} Registered_Event;

typedef struct Event_Code_Entry {
	// TODO: Use a dynamic_array
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

b8 event_register(Event_Code code, void* listener, On_Event on_event) {
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

b8 event_unregister(Event_Code code, void* listener, On_Event on_event) {
	if (code >= EVENT_CODE_MAX) {
		log_error("Event code %d is out of range", code);
		return false;
	}

	Event_Code_Entry* entry = &event_system.entries[code];
	for (u32 i = 0; i < entry->event_count; i++) {
		Registered_Event* event = &entry->events[i];
		if (event->listener == listener && event->on_event == on_event) {
			// TODO: use dynamic_array_unordered_remove
			entry->events[i] = entry->events[entry->event_count - 1];
			entry->event_count--;
			return true;
		}
	}

	// Not found
	return false;
}

b8 event_fire(Event event, void* sender) {
	if (event.code >= EVENT_CODE_MAX) {
		log_error("Event code %d is out of range", event.code);
		return false;
	}

	Event_Code_Entry* entry = &event_system.entries[event.code];
	for (u32 i = 0; i < entry->event_count; i++) {
		Registered_Event* registered_event = &entry->events[i];
		b8 handled = registered_event->on_event(&event, sender, registered_event->listener);
		if (handled) {
			return true;
		}
	}

	return true;
}

App_Quit_Event app_quit_event_create(void) {
	App_Quit_Event event;
	event.code = EVENT_TYPE_APP_QUIT;
	return event;
}

Window_Close_Event window_close_event_create(void) {
	Window_Close_Event event;
	event.code = EVENT_TYPE_WINDOW_CLOSE;
	return event;
}

Window_Resize_Event window_resize_event_create(i32 width, i32 height) {
	Window_Resize_Event event;
	event.code = EVENT_TYPE_WINDOW_RESIZE;
	event.width = width;
	event.height = height;
	return event;
}

Key_Press_Event key_press_event_create(Key key) {
	Key_Press_Event event;
	event.code = EVENT_TYPE_KEY_RELEASE;
	event.key = key;
	return event;
}

Key_Release_Event key_release_event_create(Key key) {
	Key_Release_Event event;
	event.code = EVENT_TYPE_KEY_RELEASE;
	event.key = key;
	return event;
}

Mouse_Move_Event mouse_move_event_create(i32 x, i32 y, i32 delta_x, i32 delta_y) {
	Mouse_Move_Event event;
	event.code = EVENT_TYPE_MOUSE_MOVE;
	event.x = x;
	event.y = y;
	event.delta_x = delta_x;
	event.delta_y = delta_y;
	return event;
}

Mouse_Wheel_Event mouse_wheel_event_create(i32 delta) {
	Mouse_Wheel_Event event;
	event.code = EVENT_TYPE_MOUSE_WHEEL;
	event.delta = delta;
	return event;
}

Mouse_Button_Press_Event mouse_button_press_event_create(Mouse_Button button) {
	Mouse_Button_Press_Event event;
	event.code = EVENT_TYPE_MOUSE_BUTTON_PRESS;
	event.button = button;
	return event;
}

Mouse_Button_Release_Event mouse_button_release_event_create(Mouse_Button button) {
	Mouse_Button_Release_Event event;
	event.code = EVENT_TYPE_MOUSE_BUTTON_RELEASE;
	event.button = button;
	return event;
}
