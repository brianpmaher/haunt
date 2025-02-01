#include "entry/app.h"

#include "core/log.h"
#include "core/mem.h"

#include "platform/platform.h"

typedef struct App {
	b8 running;
	b8 suspended;
	Platform platform;
	i32 width;
	i32 height;
	f64 prev_time;
} App;

static b8 initialized = false;
static App app;

b8 _app_init(App_Config* config) {
	if (initialized) {
		log_fatal("App already initialized");
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

	initialized = true;

	return true;
}

b8 _app_update(void) {
	if (!platform_pump_messages(&app.platform)) {
		app.running = false;
	}

	return true;
}

void _app_shutdown(void) {
	platform_shutdown(&app.platform);
}

HAPI b8 _app_is_running(void) {
	return app.running;
}
