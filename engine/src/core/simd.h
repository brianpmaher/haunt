#pragma once

/**
 * Checks for SIMD support.
 */

// Config
#define SIMD_ENABLED 1

// Defines the following preprocessor symbols:
// SIMD_USE_SSE
// SIMD_USE_NEON

#if SIMD_ENABLED
#	ifdef _MSC_VER
#		if defined(_M_AMD64) || ( defined(_M_IX86_FP) && _M_IX86_FP >= 1 )
#			define SIMD_USE_SSE 1
#		endif
#	else
#		ifdef __SSE__
#			define SIMD_USE_SSE 1
#		endif
#	endif
#	ifdef __ARM_NEON
#		define SIMD_USE_NEON 1
#	endif
#endif

#ifdef SIMD_USE_SSE
#	include <xmmintrin.h>
#endif

#ifdef SIMD_USE_NEON
#	include <arm_neon.h>
#endif
