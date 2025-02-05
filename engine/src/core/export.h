#ifndef HAUNT_CORE_API_H
#define HAUNT_CORE_API_H

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

#endif // HAUNT_CORE_API_H
