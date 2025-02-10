#include "entry/engine.h"

#include "core/log.h"
#include "core/memory.h"
#include "core/event.h"
#include "core/input.h"
#include "math/linalg.h"
#include "platform/platform.h"
#include "graphics/renderer.h"

typedef struct Engine {
	b8 running;
	b8 suspended;
	Platform platform;
	i32 width;
	i32 height;
	f64 prev_time;
} Engine;

static Engine engine;

static b8 handle_window_close(Event_Code code, Event_Context* context, void* sender, void* listener) {
	engine.running = false;
	event_fire(EVENT_TYPE_APP_QUIT, (Event_Context){0}, null);
	return false;
}

static b8 handle_window_resize(Event_Code code, Event_Context* context, void* sender, void* listener) {
	engine.width = context->vals[0];
	engine.height = context->vals[1];
	renderer_resize(engine.width, engine.height);
	return false;
}

b8 _engine_init(const App_Config* config) {
	engine.running = true;
	engine.suspended = false;

	event_register(EVENT_TYPE_WINDOW_CLOSE, null, handle_window_close);
	event_register(EVENT_TYPE_WINDOW_RESIZE, null, handle_window_resize);

	if (!platform_start(
			&engine.platform,
			config->name,
			config->window.x,
			config->window.y,
			config->window.width,
			config->window.height)) {
		log_fatal("Failed to start platform");
		return false;
	}

	renderer_init();

	log_debug("Engine initialized");
	return true;
}

b8 _engine_update(void) {
	input_update();

	if (!platform_pump_messages(&engine.platform)) {
		engine.running = false;
	}

	// log_trace("Engine updated");
	return true;
}

b8 _engine_render(void) {
	f64 time = platform_get_time(&engine.platform);
	Color color = color_rgb(
		sin(time) * 2.0 / 2.0,
		sin(time - 2.0 * pi / 3.0) * 2.0 / 2.0,
		sin(time - 4.0 * pi / 3.0) * 2.0 / 2.0);
	set_clear_color(color);
	clear_screen();
	return platform_swap_buffers(&engine.platform);
}

void _engine_shutdown(void) {
	platform_shutdown(&engine.platform);
	memory_report_allocations();

	log_debug("Engine shutdown");
}

b8 _engine_is_running(void) {
	return engine.running;
}
