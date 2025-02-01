#pragma once

// Determine platform
#	if defined(WIN32) || defined(_WIN32) || defined(__WIN32__)
#		define PLATFORM_WINDOWS 1
#	ifndef _WIN64
#		error "64-bit Windows is required"
#	endif
#elif defined(__linux__) || defined(__gnu_linux__)
#	define PLATFORM_LINUX 1
#	if defined(__ANDROID__)
#		define PLATFORM_ANDROID 1
#	endif
#elif defined(__APPLE__) || defined(__MACH__)
#	define PLATFORM_APPLE 1
#	include <TargetConditionals.h>
#	if TARGET_IPHONE_SIMULATOR
#		define PLATFORM_IOS 1
#		define PLATFORM_IOS_SIMULATOR 1
#	elif TARGET_OS_IPHONE
#		define PLATFORM_IOS 1
#	elif TARGET_OS_MAC
#		define PLATFORM_MAC 1
#	else
#		error "Unknown Apple platform"
#	endif
#else
#	error "Unknown platform"
#endif
