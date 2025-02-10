#pragma once

#include "render/render.h"

#include "core/context.h"
#include "core/types.h"

#ifdef PLATFORM_WINDOWS
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#endif

#include <glad/glad.h>

#include <GL/glcorearb.h>
#include <GL/glext.h>
#include <GL/glcorearb.h>

#ifdef PLATFORM_WINDOWS
#include <GL/wglext.h>
#endif

#define GL_DEBUG_ENABLED 1
