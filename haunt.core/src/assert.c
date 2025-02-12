#include "haunt/core/assert.h"

#include <stdio.h>

// TODO: Implement assert_handler
void assert_handler(const char* expression, const char* message, const char* file, int line) {
	printf("Yo!");
}
