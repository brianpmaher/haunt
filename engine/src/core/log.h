#pragma once

#include "core/export.h"
#include "core/types.h"

#define LOG_WARN_ENABLED  1
#define LOG_INFO_ENABLED  1
#define LOG_DEBUG_ENABLED 1
#define LOG_TRACE_ENABLED 1

typedef enum Log_Level {
	LOG_LEVEL_FATAL,
	LOG_LEVEL_ERROR,
	LOG_LEVEL_WARN,
	LOG_LEVEL_INFO,
	LOG_LEVEL_DEBUG,
	LOG_LEVEL_TRACE,
	LOG_LEVEL_COUNT,
} Log_Level;

export void log_output(Log_Level level, const char* message, ...);

#define log_fatal(message, ...) log_output(LOG_LEVEL_FATAL, message, ##__VA_ARGS__)

#define log_error(message, ...) log_output(LOG_LEVEL_ERROR, message, ##__VA_ARGS__)

#if LOG_WARN_ENABLED
#define log_warn(message, ...) log_output(LOG_LEVEL_WARN, message, ##__VA_ARGS__)
#else
#define log_warn(message, ...)
#endif

#if LOG_INFO_ENABLED
#define log_info(message, ...) log_output(LOG_LEVEL_INFO, message, ##__VA_ARGS__)
#else
#define log_info(message, ...)
#endif

#if LOG_DEBUG_ENABLED
#define log_debug(message, ...) log_output(LOG_LEVEL_DEBUG, message, ##__VA_ARGS__)
#else
#define log_debug(message, ...)
#endif

#if LOG_TRACE_ENABLED
#define log_trace(message, ...) log_output(LOG_LEVEL_TRACE, message, ##__VA_ARGS__)
#else
#define log_trace(message, ...)
#endif
