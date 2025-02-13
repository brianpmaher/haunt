#include "haunt/core/assert.h"

#include "haunt/core/log.h"

void _assert(const char* expression, const char* message, const char* file, int line) {
	log_fatal(
		"Assertion failed: %s\nMessage: %s\nFile: %s\nLine: %d", expression, message, file, line);
}
