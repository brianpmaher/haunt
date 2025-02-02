#include "entry/engine.h"

#include "core/log.h"
#include "core/memory.h"
#include "core/event/event.h"
#include "core/input/input.h"

#include "platform/platform.h"

typedef struct App {
	b8 running;
	b8 suspended;
	Platform platform;
	i32 width;
	i32 height;
	f64 prev_time;
} App;

static App app;

static b8 init_subsystems(void);

static void shutdown_subsystems(void);

b8 _engine_init(const App_Config* config) {
	if (!init_subsystems()) {
		log_fatal("Failed to init subsystems");
		return false;
	}

	app.running = true;
	app.suspended = false;

	if (!platform_start(
			&app.platform,
			config->name,
			config->window.x,
			config->window.y,
			config->window.width,
			config->window.height)) {
		log_fatal("Failed to start platform");
		return false;
	}

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

	if (!platform_pump_messages(&app.platform)) {
		app.running = false;
	}

	// log_trace("Engine updated");
	return true;
}

void _engine_shutdown(void) {
	platform_shutdown(&app.platform);
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
	return app.running;
}
