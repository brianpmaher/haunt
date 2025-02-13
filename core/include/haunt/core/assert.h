#pragma once

#include "haunt/core/export.h"

#define ASSERT_ENABLED 1

#if defined(__clang__) || defined(__gcc__)
#define static_assert _Static_assert
#endif

#if ASSERT_ENABLED
#if _MSC_VER
#include <intrin.h>
#define _debug_break() __debugbreak()
#else
#define _debug_break() __builtin_trap()
#endif
export void _assert(const char* expression, const char* message, const char* file, int line);
#define assert(expression)                                                                         \
	if (expression) {                                                                              \
	} else {                                                                                       \
		_assert(#expression, "", __FILE__, __LINE__);                                              \
		_debug_break();                                                                            \
	}
#define assert_message(expression, message)                                                        \
	if (expression) {                                                                              \
	} else {                                                                                       \
		_assert(#expression, message, __FILE__, __LINE__);                                         \
		_debug_break();                                                                            \
	}
#else
#define assert(expression)
#define assert_message(expression, message)
#endif // ASSERT_ENABLED
