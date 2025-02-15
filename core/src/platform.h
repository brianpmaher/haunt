#pragma once

#include "haunt/core/graphics.h"
#include "haunt/core/math.h"
#include "haunt/core/types.h"

typedef struct Platform_Internal Platform_Internal;

typedef struct Platform {
	Platform_Internal* internal;
	b8 should_close;
	Graphics_Settings graphics_settings;
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

void* platform_memory_reserve(u64 size);
void* platform_memory_commit(u64 size);
void* platform_memory_alloc(u64 size);

void platform_memory_free(void* block);

void* platform_memory_copy(void* dest, const void* src, u64 size);

void* platform_memory_set(void* dest, i32 value, u64 size);
void* platform_memory_zero(void* block, u64 size);

void platform_console_write(const char* message, Platform_Console_Color color);
void platform_console_write_error(const char* message, Platform_Console_Color color);

f64 platform_get_time(Platform* platform);

void platform_sleep(u64 ms);

b8 platform_is_debugging(void);

Vec2i platform_get_window_size(Platform* platform);
