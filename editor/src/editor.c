#include <haunt.h>

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

	return APP_RESULT_CONTINUE;
}

App_Result app_update(void* state) {
	return APP_RESULT_CONTINUE;
}

App_Result app_render(void* state) {
	return APP_RESULT_CONTINUE;
}

App_Result app_on_resize(void* state) {
	return APP_RESULT_CONTINUE;
}

App_Result app_shutdown(void* state) {
	return APP_RESULT_SUCCESS;
}
