#pragma once

#include "core/type.h"

typedef struct Platform {
	void* internal;
} Platform;

typedef enum Platform_Console_Color {
	PLATFORM_CONSOLE_COLOR_WHITE,
	PLATFORM_CONSOLE_COLOR_RED,
	PLATFORM_CONSOLE_COLOR_RED_BG,
	PLATFORM_CONSOLE_COLOR_GREEN,
	PLATFORM_CONSOLE_COLOR_BLUE,
	PLATFORM_CONSOLE_COLOR_YELLOW,
	PLATFORM_CONSOLE_COLOR_COUNT,
} Platform_Console_Color;

b8 platform_start(Platform* platform, const char* app_name, i32 x, i32 y, i32 width, i32 height);

void platform_shutdown(Platform* platform);

b8 platform_pump_messages(Platform* platform);

void* platform_mem_alloc(u64 size, b8 aligned);

void platform_mem_free(void* block, b8 aligned);

void* platform_mem_zero(void* block, u64 size);

void* platform_mem_copy(void* dest, const void* src, u64 size);

void* platform_mem_set(void* dest, i32 value, u64 size);

void platform_console_write(const char* message, Platform_Console_Color color);
void platform_console_write_error(const char* message, Platform_Console_Color color);

f64 platform_get_time();

void platform_sleep(u64 ms);
