#pragma once

#include "core/api.h"

// Config
#define ASSERT_ENABLED 1

// Static assert
#if defined(__clang__) || defined(__gcc__)
#	define static_assert _Static_assert
#endif

// Debug break and assert
#if ASSERT_ENABLED
#	if _MSC_VER
#		include <intrin.h>
#		define debug_break() __debugbreak()
#	else
#		define debug_break() __builtin_trap()
#	endif
HAPI void assert_handler(const char* expression, const char* message, const char* file, int line);
#	define assert(expression) if (expression) {} else { assert_handler(#expression, "", __FILE__, __LINE__); debug_break(); }
#	define assert_message(expression, message) if (expression) {} else { assert_handler(#expression, message, __FILE__, __LINE__); debug_break(); }
#else
#	define assert(expression)
#	define assert_message(expression, message)
#endif
