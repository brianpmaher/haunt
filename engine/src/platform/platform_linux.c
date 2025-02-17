#include "platform/platform.h"

#include "core/context.h"
#include "core/log.h"
#include "core/memory.h"
#include "core/input.h"
#include "core/event.h"

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

static Key x11_keycode_to_key(KeySym keysym) {
	switch (keysym) {
		case XK_BackSpace: return KEY_BACKSPACE;
		case XK_Return: return KEY_RETURN;
		case XK_Tab: return KEY_TAB;
		case XK_Pause: return KEY_PAUSE;
		case XK_Caps_Lock: return KEY_CAPS_LOCK;
		case XK_Escape: return KEY_ESCAPE;
		case XK_space: return KEY_SPACE;
		case XK_Page_Up: return KEY_PAGE_UP;
		case XK_Page_Down: return KEY_PAGE_DOWN;
		case XK_End: return KEY_END;
		case XK_Home: return KEY_HOME;
		case XK_Left: return KEY_LEFT;
		case XK_Up: return KEY_UP;
		case XK_Right: return KEY_RIGHT;
		case XK_Down: return KEY_DOWN;
		case XK_Insert: return KEY_INSERT;
		case XK_Delete: return KEY_DELETE;
		case XK_0: return KEY_0;
		case XK_1: return KEY_1;
		case XK_2: return KEY_2;
		case XK_3: return KEY_3;
		case XK_4: return KEY_4;
		case XK_5: return KEY_5;
		case XK_6: return KEY_6;
		case XK_7: return KEY_7;
		case XK_8: return KEY_8;
		case XK_9: return KEY_9;
		case XK_a: return KEY_A;
		case XK_b: return KEY_B;
		case XK_c: return KEY_C;
		case XK_d: return KEY_D;
		case XK_e: return KEY_E;
		case XK_f: return KEY_F;
		case XK_g: return KEY_G;
		case XK_h: return KEY_H;
		case XK_i: return KEY_I;
		case XK_j: return KEY_J;
		case XK_k: return KEY_K;
		case XK_l: return KEY_L;
		case XK_m: return KEY_M;
		case XK_n: return KEY_N;
		case XK_o: return KEY_O;
		case XK_p: return KEY_P;
		case XK_q: return KEY_Q;
		case XK_r: return KEY_R;
		case XK_s: return KEY_S;
		case XK_t: return KEY_T;
		case XK_u: return KEY_U;
		case XK_v: return KEY_V;
		case XK_w: return KEY_W;
		case XK_x: return KEY_X;
		case XK_y: return KEY_Y;
		case XK_z: return KEY_Z;
		case XK_Shift_L:
		case XK_Shift_R: return KEY_SHIFT;
		case XK_Control_L:
		case XK_Control_R: return KEY_CONTROL;
		case XK_Alt_L:
		case XK_Alt_R: return KEY_ALT;
		default: return KEY_COUNT; // Invalid/unsupported key
	}
}

// Add this helper function to convert X11 button to our engine's mouse button
static Mouse_Button x11_button_to_mouse_button(unsigned int button) {
	switch (button) {
		case Button1: return MOUSE_BUTTON_LEFT;
		case Button2: return MOUSE_BUTTON_MIDDLE;
		case Button3: return MOUSE_BUTTON_RIGHT;
		default: return MOUSE_BUTTON_COUNT;  // Invalid button
	}
}

b8 platform_pump_messages(Platform* platform) {
	Platform_Internal* internal = (Platform_Internal*)platform->internal;

	XEvent event;
	while (XPending(internal->display)) {
		XNextEvent(internal->display, &event);

		switch (event.type) {
			case KeyPress:
			case KeyRelease: {
				// Get the key symbol
				KeySym keysym = XLookupKeysym(&event.xkey, 0);
				Key key = x11_keycode_to_key(keysym);
				
				// Only handle valid keys
				if (key != KEY_COUNT) {
					// Create event context
					Event_Context context = {0};
					context.vals[0] = key;
					
					// Fire the appropriate event
					if (event.type == KeyPress) {
						event_fire(EVENT_TYPE_KEY_PRESS, context, null);
					} else {
						event_fire(EVENT_TYPE_KEY_RELEASE, context, null);
					}
				}
			} break;

			case ButtonPress:
			case ButtonRelease: {
				// Handle mouse wheel
				if (event.xbutton.button == Button4) {  // Mouse wheel up
					Event_Context context = {0};
					context.vals[0] = 1;  // Positive for scroll up
					event_fire(EVENT_TYPE_MOUSE_WHEEL, context, null);
					break;
				}
				if (event.xbutton.button == Button5) {  // Mouse wheel down
					Event_Context context = {0};
					context.vals[0] = -1;  // Negative for scroll down
					event_fire(EVENT_TYPE_MOUSE_WHEEL, context, null);
					break;
				}

				// Handle mouse buttons
				Mouse_Button button = x11_button_to_mouse_button(event.xbutton.button);
				if (button != MOUSE_BUTTON_COUNT) {
					Event_Context context = {0};
					context.vals[0] = button;
					
					if (event.type == ButtonPress) {
						event_fire(EVENT_TYPE_MOUSE_BUTTON_PRESS, context, null);
					} else {
						event_fire(EVENT_TYPE_MOUSE_BUTTON_RELEASE, context, null);
					}
				}
			} break;

			case MotionNotify: {
				// Handle mouse movement
				Event_Context context = {0};
				context.vals[0] = event.xmotion.x;
				context.vals[1] = event.xmotion.y;
				event_fire(EVENT_TYPE_MOUSE_MOVE, context, null);
			} break;

			case ClientMessage:
				if ((Atom)event.xclient.data.l[0] == internal->wm_delete_window) {
					return false;
				}
				break;
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
