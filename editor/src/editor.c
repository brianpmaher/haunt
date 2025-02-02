#include <haunt.h>

typedef struct Editor {
	b8 should_quit;
} Editor;

static b8 handle_key_press(Event_Code code, Event_Context* context, void* sender, void* listener);

App_Config app_config(void) {
	App_Config config;
	config.name = "Haunt Editor";
	config.window.x = 100;
	config.window.y = 100;
	config.window.width = 1280;
	config.window.height = 720;
	return config;
}

App_Result app_start(void** state, int argc, char** argv) {
	log_info("Haunt editor started");

	// Allocate editor state
	*state = memory_alloc(sizeof(Editor), MEMORY_TAG_EDITOR);
	Editor* editor = (Editor*)*state;

	// Register events
	event_register(EVENT_TYPE_KEY_PRESS, editor, handle_key_press);

	return APP_RESULT_CONTINUE;
}

App_Result app_update(void* state) {
	Editor* editor = (Editor*)state;

	if (editor->should_quit) {
		return APP_RESULT_SUCCESS;
	}

	return APP_RESULT_CONTINUE;
}

App_Result app_render(void* state) {
	return APP_RESULT_CONTINUE;
}

App_Result app_on_resize(void* state) {
	return APP_RESULT_CONTINUE;
}

App_Result app_shutdown(void* state) {
	Editor* editor = (Editor*)state;
	memory_free(editor, sizeof(Editor), MEMORY_TAG_EDITOR);

	return APP_RESULT_SUCCESS;
}

static b8 handle_key_press(Event_Code code, Event_Context* context, void* sender, void* listener) {
	Editor* editor = (Editor*)listener;

	Key key = context->vals[0];

	if (key == KEY_ESCAPE) {
		log_info("Received escape key, shutting down...");
		editor->should_quit = true;
		return true;
	}

	return false;
}
