#ifndef HAUNT_CORE_ASSERT_H
#define HAUNT_CORE_ASSERT_H

#include "haunt/core/export.h"
#include "haunt/core/types.h"

#define ASSERT_ENABLED 1

// TODO: Support MSVC static_assert
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
export void _assert(const char* expression, const char* message, const char* file, i32 line);
#define assert(expression)                            \
	if (expression) {                                 \
	} else {                                          \
		_assert(#expression, "", __FILE__, __LINE__); \
		_debug_break();                               \
	}
#define assert_message(expression, message)                \
	if (expression) {                                      \
	} else {                                               \
		_assert(#expression, message, __FILE__, __LINE__); \
		_debug_break();                                    \
	}
#else
#define assert(expression)
#define assert_message(expression, message)
#endif // ASSERT_ENABLED

#endif // HAUNT_CORE_ASSERT_H
