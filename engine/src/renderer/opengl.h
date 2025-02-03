#pragma once

#include "core/context.h"
#include "platform/platform.h"

#ifdef PLATFORM_WINDOWS
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#endif

#include <glad/glad.h>
#include <GL/glcorearb.h>

#define GL_DEBUG_ENABLED 1

void gl_init(void);
