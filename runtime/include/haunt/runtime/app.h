#pragma once

#include "haunt/core/types.h"

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

extern App_Config app_config(void);

extern void app_start(void** state, int argc, char** argv);

extern void app_update(void* state);

extern void app_shutdown(void* state);
