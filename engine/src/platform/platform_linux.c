#include "platform/platform.h"

#include "core/context.h"
#include "core/log.h"
#include "core/memory.h"

#ifdef PLATFORM_LINUX

// GLAD must come before any OpenGL headers
#include <glad/glad.h>

#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/keysym.h>
#include <X11/XKBlib.h>
#include <sys/time.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <GL/glx.h>

typedef struct Clock {
	f64 frequency;
	struct timeval start;
} Clock;

typedef struct Platform_Internal {
	Display* display;
	Window window;
	XVisualInfo* visual_info;
	Colormap color_map;
	XSetWindowAttributes window_attributes;
	Atom wm_delete_window;
	Clock clock;
	GLXContext gl_context;
} Platform_Internal;

static const char* console_colors[PLATFORM_CONSOLE_COLOR_COUNT] = {
	"\033[0m",     // PLATFORM_CONSOLE_COLOR_WHITE
	"\033[91m",    // PLATFORM_CONSOLE_COLOR_RED
	"\033[101m",   // PLATFORM_CONSOLE_COLOR_RED_BG
	"\033[92m",    // PLATFORM_CONSOLE_COLOR_GREEN
	"\033[94m",    // PLATFORM_CONSOLE_COLOR_BLUE
	"\033[93m",    // PLATFORM_CONSOLE_COLOR_YELLOW
};

static Platform_Internal* create_internal(void) {
	Platform_Internal* internal = memory_alloc(sizeof(Platform_Internal), MEMORY_TAG_PLATFORM);
	memory_zero(internal, sizeof(Platform_Internal));
	return internal;
}

b8 platform_start(Platform* platform, const char* app_name, i32 x, i32 y, i32 width, i32 height) {
	log_info("Platform start...");
	platform->internal = create_internal();
	Platform_Internal* internal = (Platform_Internal*)platform->internal;

	// Open X display
	log_info("Opening X display...");
	internal->display = XOpenDisplay(NULL);
	if (!internal->display) {
		log_fatal("Failed to open X display");
		return false;
	}
	log_info("X display opened successfully");

	// Get default screen
	int screen = DefaultScreen(internal->display);
	Window root = RootWindow(internal->display, screen);

	// Get visual info
	XVisualInfo visual_info_template;
	visual_info_template.screen = screen;
	visual_info_template.depth = 24;
	visual_info_template.class = TrueColor;
	
	int visual_info_count;
	internal->visual_info = XGetVisualInfo(
		internal->display,
		VisualScreenMask | VisualDepthMask | VisualClassMask,
		&visual_info_template,
		&visual_info_count
	);

	if (!internal->visual_info) {
		log_fatal("Failed to get visual info");
		return false;
	}

	// Create colormap
	internal->color_map = XCreateColormap(
		internal->display,
		root,
		internal->visual_info->visual,
		AllocNone
	);

	// Set window attributes
	internal->window_attributes.colormap = internal->color_map;
	internal->window_attributes.event_mask = 
		ExposureMask | KeyPressMask | KeyReleaseMask |
		ButtonPressMask | ButtonReleaseMask |
		PointerMotionMask | StructureNotifyMask;

	// Create window
	internal->window = XCreateWindow(
		internal->display,
		root,
		x, y,
		width, height,
		0,
		internal->visual_info->depth,
		InputOutput,
		internal->visual_info->visual,
		CWColormap | CWEventMask,
		&internal->window_attributes
	);

	if (!internal->window) {
		log_fatal("Failed to create window");
		return false;
	}

	// Set window title
	XStoreName(internal->display, internal->window, app_name);

	// Handle window close
	internal->wm_delete_window = XInternAtom(internal->display, "WM_DELETE_WINDOW", False);
	XSetWMProtocols(internal->display, internal->window, &internal->wm_delete_window, 1);

	// Show window
	XMapWindow(internal->display, internal->window);
	XFlush(internal->display);

	// After window creation, create OpenGL context
	log_info("Creating OpenGL context...");
	internal->gl_context = glXCreateContext(internal->display, internal->visual_info, NULL, GL_TRUE);
	if (!internal->gl_context) {
		log_fatal("Failed to create OpenGL context");
		return false;
	}

	// Make the context current
	if (!glXMakeCurrent(internal->display, internal->window, internal->gl_context)) {
		log_fatal("Failed to make OpenGL context current");
		return false;
	}

	// Initialize GLAD
	log_info("Initializing GLAD...");
	if (!gladLoadGL()) {
		log_fatal("Failed to initialize GLAD");
		return false;
	}

	return true;
}

void platform_shutdown(Platform* platform) {
	Platform_Internal* internal = (Platform_Internal*)platform->internal;

	if (internal->display) {
		if (internal->gl_context) {
			glXMakeCurrent(internal->display, None, NULL);
			glXDestroyContext(internal->display, internal->gl_context);
		}
		if (internal->window) {
			XDestroyWindow(internal->display, internal->window);
		}
		if (internal->color_map) {
			XFreeColormap(internal->display, internal->color_map);
		}
		if (internal->visual_info) {
			XFree(internal->visual_info);
		}
		XCloseDisplay(internal->display);
	}

	memory_free(platform->internal, sizeof(Platform_Internal), MEMORY_TAG_PLATFORM);
}

b8 platform_pump_messages(Platform* platform) {
	Platform_Internal* internal = (Platform_Internal*)platform->internal;

	XEvent event;
	while (XPending(internal->display)) {
		XNextEvent(internal->display, &event);

		switch (event.type) {
			case ClientMessage:
				if ((Atom)event.xclient.data.l[0] == internal->wm_delete_window) {
					return false;
				}
				break;
			// TODO: Handle other events
		}
	}

	return true;
}

b8 platform_swap_buffers(Platform* platform) {
	Platform_Internal* internal = (Platform_Internal*)platform->internal;
	glXSwapBuffers(internal->display, internal->window);
	return true;
}

void* platform_memory_alloc(u64 size, b8 aligned) {
	// TODO: Handle alignment
	return malloc(size);
}

void platform_memory_free(void* block, b8 aligned) {
	// TODO: Handle alignment
	free(block);
}

void* platform_memory_zero(void* block, u64 size) {
	return memset(block, 0, size);
}

void* platform_memory_copy(void* dest, const void* src, u64 size) {
	return memcpy(dest, src, size);
}

void* platform_memory_set(void* dest, i32 value, u64 size) {
	return memset(dest, value, size);
}

void platform_console_write(const char* message, Platform_Console_Color color) {
	printf("%s%s\033[0m", console_colors[color], message);
}

void platform_console_write_error(const char* message, Platform_Console_Color color) {
	fprintf(stderr, "%s%s\033[0m", console_colors[color], message);
}

f64 platform_get_time(Platform* platform) {
	struct timeval now;
	gettimeofday(&now, NULL);
	return (f64)now.tv_sec + (f64)now.tv_usec / 1000000.0;
}

void platform_sleep(u64 ms) {
	sleep(ms * 1000);
}
 
b8 platform_is_debugging(void) {
	log_error("platform_is_debugging is not implemented for Linux platform");
	return false;
}

#endif // PLATFORM_LINUX
