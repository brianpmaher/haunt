#pragma once

#include "entry/app.h"

#include "core/log.h"

int main(int argc, char** argv) {
	// Init subsystems
	if (!_sys_log_init()) {
		// Can't log here
		return APP_RESULT_FAILURE;
	}
	_sys_mem_init();

	// App config
	App_Config config = app_config();

	// Engine init
	if (!_app_init(&config)) {
		log_fatal("Failed to create app");
		return APP_RESULT_FAILURE;
	}

	// App start
	void* state = null;
	App_Result result = app_start(&state, argc, argv);
	if (result != APP_RESULT_CONTINUE) {
		log_info("App start returned %d", result);
		_app_shutdown();
		return result;
	}

	// Main loop
	while (_app_is_running()) {
		// Engine update
		if (!_app_update()) {
			log_error("Engine update failed");
			break;
		}

		// App update
		App_Result result = app_update(state);
		if (result != APP_RESULT_CONTINUE) {
			log_info("App update returned %d", result);
			break;
		}

		// App render
		result = app_render(state);
		if (result != APP_RESULT_CONTINUE) {
			log_info("App render returned %d", result);
			break;
		}
	}

	// Engine shutdown
	_app_shutdown();

	// App shutdown
	result = app_shutdown(state);
	if (result == APP_RESULT_CONTINUE) {
		log_warn("App shutdown returned APP_RESULT_CONTINUE, ignoring");
		result = APP_RESULT_SUCCESS;
	}

	// Shutdown subsystems
	_sys_log_shutdown();
	_sys_mem_shutdown();

	return result;
}
