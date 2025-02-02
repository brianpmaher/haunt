#pragma once

#include "entry/app.h"
#include "entry/engine.h"

#include "core/log.h"

int main(int argc, char** argv) {
	App_Config config = app_config();

	if (!_engine_init(&config)) {
		log_fatal("Engine intialization failed");
		return APP_RESULT_FAILURE;
	}

	void* state = null;
	App_Result result = app_start(&state, argc, argv);
	if (result != APP_RESULT_CONTINUE) {
		log_info("App start returned %d", result);
		_engine_shutdown();
		return result;
	}

	while (_engine_is_running()) {
		if (!_engine_update()) {
			log_error("Engine update failed");
			break;
		}

		App_Result result = app_update(state);
		if (result != APP_RESULT_CONTINUE) {
			log_info("App update returned %d", result);
			break;
		}

		result = app_render(state);
		if (result != APP_RESULT_CONTINUE) {
			log_info("App render returned %d", result);
			break;
		}
	}

	result = app_shutdown(state);
	if (result == APP_RESULT_CONTINUE) {
		log_warn("App shutdown returned APP_RESULT_CONTINUE, ignoring");
		result = APP_RESULT_SUCCESS;
	}

	_engine_shutdown();

	return result;
}
