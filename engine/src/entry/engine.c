#include "entry/engine.h"

#include "core/log.h"
#include "core/memory.h"
#include "core/event.h"
#include "core/input.h"
#include "math/linalg.h"
#include "platform/platform.h"
#include "renderer/opengl.h"

typedef struct Engine {
	b8 running;
	b8 suspended;
	Platform platform;
	i32 width;
	i32 height;
	f64 prev_time;
} Engine;

static Engine engine;

static b8 init_subsystems(void);

static void shutdown_subsystems(void);

static b8 handle_window_close(Event_Code code, Event_Context* context, void* sender, void* listener);

b8 _engine_init(const App_Config* config) {
	if (!init_subsystems()) {
		log_fatal("Failed to init subsystems");
		return false;
	}

	engine.running = true;
	engine.suspended = false;

	event_register(EVENT_TYPE_WINDOW_CLOSE, null, handle_window_close);

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

	gl_init();

	log_debug("Engine initialized");
	return true;
}

static b8 init_subsystems(void) {
	if (!logging_system_init()) {
		return false;
	}
	memory_system_init();
	event_system_init();
	input_system_init();

	return true;
}

b8 _engine_update(void) {
	input_system_update();

	if (!platform_pump_messages(&engine.platform)) {
		engine.running = false;
	}

	// log_trace("Engine updated");
	return true;
}

b8 _engine_render(void) {
	f64 time = platform_get_time(&engine.platform);
	f32 red = sin(time) * 2.0 / 2.0;
	f32 green = sin(time - 2.0 * pi / 3.0) * 2.0 / 2.0;
	f32 blue = sin(time - 4.0 * pi / 3.0) * 2.0 / 2.0;
	glClearColor(red, green, blue, 1.0);
	glClear(GL_COLOR_BUFFER_BIT);

	return platform_swap_buffers(&engine.platform);
}

void _engine_shutdown(void) {
	platform_shutdown(&engine.platform);
	shutdown_subsystems();

	log_debug("Engine shutdown");
}

static void shutdown_subsystems(void) {
	input_system_shutdown();
	event_system_shutdown();
	logging_system_shutdown();
	memory_system_shutdown();
}

b8 _engine_is_running(void) {
	return engine.running;
}

static b8 handle_window_close(Event_Code code, Event_Context* context, void* sender, void* listener) {
	engine.running = false;
	event_fire(EVENT_TYPE_APP_QUIT, (Event_Context){0}, null);
	return true;
}
