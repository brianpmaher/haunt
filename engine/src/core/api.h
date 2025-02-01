#pragma once

#ifdef DLL_EXPORT
#	ifdef _MSC_VER
#		define HAPI __declspec(dllexport)
#	else
#		define HAPI __attribute__((visibility("default")))
#	endif
#else // DLL_IMPORT
#	ifdef _MSC_VER
#		define HAPI __declspec(dllimport)
#	else
#		define HAPI
#	endif
#endif
