#pragma once

#ifdef DLL_EXPORT
#	ifdef _MSC_VER
#		define export __declspec(dllexport)
#	else
#		define export __attribute__((visibility("default")))
#	endif
#else
#	ifdef _MSC_VER
#		define export __declspec(dllimport)
#	else
#		define export
#	endif
#endif
