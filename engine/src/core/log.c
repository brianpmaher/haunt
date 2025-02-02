#include "core/log.h"
#include "core/mem.h"

#include "platform/platform.h"

#include <stdio.h>
#include <string.h>
#include <stdarg.h>

static const char* level_strings[LOG_LEVEL_COUNT] = {
	"[FATAL] ",
	"[ERROR] ",
	"[WARN] ",
	"[INFO] ",
	"[DEBUG] ",
	"[TRACE] ",
};

static const Platform_Console_Color level_colors[LOG_LEVEL_COUNT] = {
	PLATFORM_CONSOLE_COLOR_RED_BG,
	PLATFORM_CONSOLE_COLOR_RED,
	PLATFORM_CONSOLE_COLOR_YELLOW,
	PLATFORM_CONSOLE_COLOR_WHITE,
	PLATFORM_CONSOLE_COLOR_BLUE,
	PLATFORM_CONSOLE_COLOR_GREEN
};

void assert_handler(const char* expression, const char* message, const char* file, int line) {
	log_output(LOG_LEVEL_FATAL, "Assertion failed: (%s), message: \"%s\", file: \"%s\", line: %d", expression, message, file, line);
}

b8 logging_system_init() {
	// TODO: Create log file
	return true;
}

void logging_system_shutdown() {
	// TODO: Cleanup logging / close log file
}

void log_output(Log_Level level, const char* message, ...) {
	b8 is_error = level <= LOG_LEVEL_ERROR;

	const i32 buffer_size = 32768;
	char buffer1[buffer_size];
	// TODO: Use mem_zero
	memset(buffer1, 0, sizeof(buffer1));

	va_list args;
	va_start(args, message);
	vsnprintf(buffer1, sizeof(buffer1), message, args);
	va_end(args);

	char buffer2[buffer_size];
	sprintf(buffer2, "%s%s\n", level_strings[level], buffer1);

	Platform_Console_Color color = level_colors[level];
	if (is_error) {
		platform_console_write_error(buffer2, color);
	} else {
		platform_console_write(buffer2, color);
	}
}
