#ifndef HAUNT_PLATFORM_INC_H
#define HAUNT_PLATFORM_INC_H

#ifdef PLATFORM_WINDOWS
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <Windowsx.h> // GET_X_LPARAM, GET_Y_LPARAM
#endif

#include <glad/glad.h>
#include <GL/glcorearb.h>

#ifdef PLATFORM_WINDOWS
#include <GL/glext.h>
#include <GL/wglext.h>
#endif

#include "platform/platform.h"

#endif // HAUNT_PLATFORM_INC_H
