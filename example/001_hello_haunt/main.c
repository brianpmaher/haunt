#include "haunt/core.h"

#include <stdio.h>

int main(void) {
	log_info("Haunt Version: %s\n", get_engine_version());
}
