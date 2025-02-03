#pragma once

#include "core/context.h"
#include "platform/platform.h"

#ifdef PLATFORM_WINDOWS
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#endif

#include <glad/glad.h>
#include <GL/glcorearb.h>

#ifdef PLATFORM_WINDOWS
#include <GL/glext.h>
#include <GL/wglext.h>
#endif

#define GL_DEBUG_ENABLED 1
#define VSYNC_ENABLED 1
#define SRGB_ENABLED 1
#define MSAA_ENABLED 1

void gl_init(void);
