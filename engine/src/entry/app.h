#pragma once

#include "core/types.h"
#include "core/event.h"

typedef struct Window_Config {
	i32 x;
	i32 y;
	i32 width;
	i32 height;
} Window_Config;

typedef struct App_Config {
	const char* name;
	Window_Config window;
} App_Config;

typedef enum App_Result {
	APP_RESULT_SUCCESS,
	APP_RESULT_FAILURE,
	APP_RESULT_CONTINUE,
} App_Result;

extern App_Config app_config(void);

extern App_Result app_start(void** state, int argc, char** argv);

extern App_Result app_event(void* state, Event* event);

extern App_Result app_update(void* state);

extern App_Result app_render(void* state);

extern App_Result app_on_resize(void* state);

extern App_Result app_shutdown(void* state);
