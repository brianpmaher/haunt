#include "platform/platform.h"

#include "core/context.h"
#include "core/log.h"
#include "core/memory.h"
#include "core/input/input.h"
#include "core/event.h"

#ifdef PLATFORM_WINDOWS

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <Windowsx.h> // GET_X_LPARAM, GET_Y_LPARAM
#include "renderer/opengl.h.h"
#include <GL/wglext.h>

typedef struct Clock {
	f64 frequency;
	LARGE_INTEGER start;
} Clock;

typedef struct Platform_Internal {
	const char* class_name;
	HINSTANCE hinst;
	// TODO: Handle multiple windows
	HWND hwnd;
	Clock clock;
} Platform_Internal;

static const u8 console_colors[PLATFORM_CONSOLE_COLOR_COUNT] = {
	// PLATFORM_CONSOLE_COLOR_WHITE
	FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_RED | FOREGROUND_INTENSITY,
	// PLATFORM_CONSOLE_COLOR_RED
	FOREGROUND_RED | FOREGROUND_INTENSITY,
	// PLATFORM_CONSOLE_COLOR_RED_BG
	BACKGROUND_RED | BACKGROUND_INTENSITY,
	// PLATFORM_CONSOLE_COLOR_GREEN
	FOREGROUND_GREEN | FOREGROUND_INTENSITY,
	// PLATFORM_CONSOLE_COLOR_BLUE
	FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_INTENSITY,
	// PLATFORM_CONSOLE_COLOR_YELLOW
	FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_INTENSITY,
};

// OpenGL
static PFNWGLCHOOSEPIXELFORMATARBPROC wglChoosePixelFormatARB = null;
static PFNWGLCREATECONTEXTATTRIBSARBPROC wglCreateContextAttribsARB = null;
static PFNWGLSWAPINTERVALEXTPROC wglSwapIntervalEXT = null;
static void load_wgl_functions(void);
static b8 equals_cstr_blen(const char* a, const char* b, size_t blen);

static LRESULT CALLBACK process_message(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);
static Platform_Internal* create_internal(void) {
	Platform_Internal* internal = memory_alloc(sizeof(Platform_Internal), MEMORY_TAG_PLATFORM);
	internal->class_name = "haunt_window_class";
	internal->hinst = GetModuleHandle(0);
	return internal;
}

static b8 register_window_class(HINSTANCE hinst, const char* class_name) {
	HICON icon = LoadIcon(hinst, IDI_APPLICATION);
	WNDCLASS wc = {0};
    wc.style = CS_DBLCLKS;
    wc.lpfnWndProc = process_message;
    wc.cbClsExtra = 0;
    wc.cbWndExtra = 0;
    wc.hInstance = hinst;
    wc.hIcon = icon;
    wc.hCursor = LoadCursor(null, IDC_ARROW);
    wc.hbrBackground = null;
    wc.lpszMenuName = null;
    wc.lpszClassName = class_name;
	if (!RegisterClassA(&wc)) {
		MessageBoxA(null, "Failed to register window class", "Error", MB_OK | MB_ICONERROR);
		log_fatal("Failed to register window class");
		return false;
	}
	return true;
}

static HWND create_window(HINSTANCE hinst, const char* class_name, const char* app_name, i32 x, i32 y, i32 width, i32 height) {
	// Set window properties
	u32 client_x = x;
	u32 client_y = y;
	u32 client_width = width;
	u32 client_height = height;
	u32 window_x = client_x;
	u32 window_y = client_y;
	u32 window_width = client_width;
	u32 window_height = client_height;

	// Setup window style
	u32 window_style = WS_OVERLAPPEDWINDOW | WS_SYSMENU | WS_CAPTION;
	u32 window_ex_style = WS_EX_APPWINDOW;
	window_style |= WS_MAXIMIZEBOX;
	window_style |= WS_MINIMIZEBOX;
	window_style |= WS_THICKFRAME;

	// Get border size
	RECT border_rect = {0, 0, 0, 0};
	AdjustWindowRectEx(&border_rect, window_style, false, window_ex_style);
	window_x += border_rect.left;
	window_y += border_rect.top;
	window_width += border_rect.right - border_rect.left;
	window_height += border_rect.bottom - border_rect.top;

	// Create window
	HWND handle = CreateWindowExA(
		window_ex_style, class_name, app_name,
		window_style, window_x, window_y, window_width, window_height,
		0, 0, hinst, 0);
	if (!handle) {
		MessageBoxA(null, "Failed to create window", "Error", MB_OK | MB_ICONERROR);
		log_fatal("Failed to create window");
		return null;
	}

	return handle;
}

static void set_window_data(HWND hwnd, Platform* platform) {
	SetWindowLongPtrA(hwnd, GWLP_USERDATA, (LONG_PTR)platform);
}

static Platform* get_window_data(HWND hwnd) {
	return (Platform*)GetWindowLongPtrA(hwnd, GWLP_USERDATA);
}

static Platform* get_active_window_data(void) {
	return get_window_data(GetActiveWindow());
}

static void show_window(Platform_Internal* internal) {
	// TODO: make these parameters
	b8 activate = true;
	b8 minimized = false;
	b8 maximized = false;
	i32 show_window_flags = 0;
	show_window_flags |= activate ? SW_SHOW : SW_SHOWNOACTIVATE;
	if (minimized) {
		show_window_flags |= SW_MINIMIZE;
	} else if (maximized) {
		show_window_flags |= SW_MAXIMIZE;
	}
	ShowWindow(internal->hwnd, show_window_flags);
}

static void clock_start(Clock* clock);

b8 platform_start(Platform* platform, const char* app_name, i32 x, i32 y, i32 width, i32 height) {
	platform->internal = create_internal();
	Platform_Internal* internal = (Platform_Internal*)platform->internal;

	load_wgl_functions();

	// Register window class and create window
	if (!register_window_class(internal->hinst, internal->class_name)) {
		log_fatal("Failed to register window class");
		return false;
	}
	internal->hwnd = create_window(internal->hinst, internal->class_name, app_name, x, y, width, height);
	if (!internal->hwnd) {
		log_fatal("Failed to create window");
		return false;
	}
	set_window_data(internal->hwnd, platform);
	show_window(internal);

	clock_start(&internal->clock);

	return true;
}

void platform_shutdown(Platform* platform) {
	Platform_Internal* internal = (Platform_Internal*)platform->internal;

	if (internal->hwnd) {
		DestroyWindow(internal->hwnd);
		internal->hwnd = null;
	}

	UnregisterClassA(internal->class_name, internal->hinst);

	memory_free(platform->internal, sizeof(Platform_Internal), MEMORY_TAG_PLATFORM);
}

b8 platform_pump_messages(Platform* platform) {
	MSG msg;
	while (PeekMessageA(&msg, null, 0, 0, PM_REMOVE)) {
		TranslateMessage(&msg);
		DispatchMessageA(&msg);
	}
	return true;
}

void* platform_memory_alloc(u64 size, b8 aligned) {
	return VirtualAlloc(null, size, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);
}

void platform_memory_free(void* block, b8 aligned) {
	VirtualFree(block, 0, MEM_RELEASE);
}

void* platform_memory_zero(void* block, u64 size) {
	ZeroMemory(block, size);
	return block;
}

void* platform_memory_copy(void* dest, const void* src, u64 size) {
	CopyMemory(dest, src, size);
	return dest;
}

void* platform_memory_set(void* dest, i32 value, u64 size) {
	FillMemory(dest, size, value);
	return dest;
}

void platform_console_write(const char* message, Platform_Console_Color color) {
	LPDWORD written = null;
	HANDLE console = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(console, console_colors[color]);
	WriteConsoleA(console, message, strlen(message), written, NULL);
	OutputDebugStringA(message);
}

void platform_console_write_error(const char* message, Platform_Console_Color color) {
	LPDWORD written = null;
	HANDLE console = GetStdHandle(STD_ERROR_HANDLE);
	SetConsoleTextAttribute(console, console_colors[color]);
	WriteConsoleA(console, message, strlen(message), written, NULL);
	OutputDebugStringA(message);
}

static void clock_start(Clock* clock) {
	LARGE_INTEGER frequency;
	QueryPerformanceFrequency(&frequency);
	clock->frequency = 1.0 / (f64)frequency.QuadPart;
	QueryPerformanceCounter(&clock->start);
}

f64 platform_get_time() {
	Platform* platform = get_active_window_data();
	Platform_Internal* internal = (Platform_Internal*)platform->internal;

	LARGE_INTEGER current;
	QueryPerformanceCounter(&current);
	return (f64)current.QuadPart * internal->clock.frequency;
}

void platform_sleep(u64 ms) {
	Sleep(ms);
}

static b8 equals_cstr_blen(const char* a, const char* b, size_t blen) {
	while (*a && blen-- && *b) {
		if (*a++ != *b++) {
			return false;
		}
	}

	return (blen && *a == *b) || (!blen && *a == 0);
}

static void load_wgl_functions(void) {
	// WGL functions need valid GL context, so create dummy window for dummy GL
	// context
	HWND dummy = CreateWindowExW(
		0, L"STATIC", L"DummyWindow", WS_OVERLAPPED, CW_USEDEFAULT,
		CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, NULL, NULL, NULL, NULL);
	assert_message(dummy, "Failed to create dummy window");

	HDC dc = GetDC(dummy);
	assert_message(dc, "Failed to get device context for dummy window");

	PIXELFORMATDESCRIPTOR desc = {
		.nSize = sizeof(desc),
		.nVersion = 1,
		.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER,
		.iPixelType = PFD_TYPE_RGBA,
		.cColorBits = 24,
	};

	int format = ChoosePixelFormat(dc, &desc);
	if (!format) {
		log_fatal("Cannot choose OpenGL pixel format for dummy window!");
	}

	int ok = DescribePixelFormat(dc, format, sizeof(desc), &desc);
	assert_message(ok, "Failed to describe OpenGL pixel format");

	// Reason to create dummy window is that SetPixelFormat can be called only
	// once for the window
	if (!SetPixelFormat(dc, format, &desc)) {
		log_fatal("Cannot set OpenGL pixel format for dummy window!");
	}

	HGLRC rc = wglCreateContext(dc);
	assert_message(rc, "Failed to create OpenGL context for dummy window");

	ok = wglMakeCurrent(dc, rc);
	assert_message(ok, "Failed to make current OpenGL context for dummy window");

	// https://www.khronos.org/registry/OpenGL/extensions/ARB/WGL_ARB_extensions_string.txt
	PFNWGLGETEXTENSIONSSTRINGARBPROC wglGetExtensionsStringARB = (void *)wglGetProcAddress("wglGetExtensionsStringARB");
	if (!wglGetExtensionsStringARB) {
		log_fatal("OpenGL does not support WGL_ARB_extensions_string extension!");
	}

	const char* ext = wglGetExtensionsStringARB(dc);
	assert_message(ext, "Failed to get OpenGL WGL extension string");

	const char* start = ext;
	while (true) {
		while (*ext != 0 && *ext != ' ') {
			ext++;
		}

		size_t length = ext - start;
		if (equals_cstr_blen("WGL_ARB_pixel_format", start, length)) {
			// https://www.khronos.org/registry/OpenGL/extensions/ARB/WGL_ARB_pixel_format.txt
			wglChoosePixelFormatARB =
				(void*)wglGetProcAddress("wglChoosePixelFormatARB");
		} else if (equals_cstr_blen("WGL_ARB_create_context", start, length)) {
			// https://www.khronos.org/registry/OpenGL/extensions/ARB/WGL_ARB_create_context.txt
			wglCreateContextAttribsARB =
				(void*)wglGetProcAddress("wglCreateContextAttribsARB");
		} else if (equals_cstr_blen("WGL_EXT_swap_control", start, length)) {
			// https://www.khronos.org/registry/OpenGL/extensions/EXT/WGL_EXT_swap_control.txt
			wglSwapIntervalEXT = (void*)wglGetProcAddress("wglSwapIntervalEXT");
		}

		if (*ext == 0) {
			break;
		}

		ext++;
		start = ext;
	}

	if (!wglChoosePixelFormatARB || !wglCreateContextAttribsARB || !wglSwapIntervalEXT) {
		log_fatal("OpenGL does not support required WGL extensions for modern context!");
	}

	wglMakeCurrent(NULL, NULL);
	wglDeleteContext(rc);
	ReleaseDC(dummy, dc);
	DestroyWindow(dummy);
}

static LRESULT CALLBACK process_message(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam) {
	switch (msg) {
		case WM_ERASEBKGND:
			// Tell OS that app is handling background erase to prevent flickering
			return 1;
		case WM_CLOSE:
			event_fire(EVENT_TYPE_WINDOW_CLOSE, (Event_Context){0}, null);
			return 0;
		case WM_DESTROY:
			PostQuitMessage(0);
			return 0;
		case WM_SIZE: {
			RECT r;
			GetClientRect(hwnd, &r);
			u32 width = r.right - r.left;
			u32 height = r.bottom - r.top;
			event_fire(EVENT_TYPE_WINDOW_RESIZE, (Event_Context){ (i32)width, (i32)height }, null);
		} break;
		case WM_KEYDOWN:
		case WM_SYSKEYDOWN:
		case WM_KEYUP:
		case WM_SYSKEYUP: {
			b8 pressed = msg == WM_KEYDOWN || msg == WM_SYSKEYDOWN;
			WORD vkcode = LOWORD(wparam);
			Key key = (Key)vkcode;
			input_system_process_key(key, pressed);
		} break;
		case WM_MOUSEMOVE: {
			i32 x = GET_X_LPARAM(lparam);
			i32 y = GET_Y_LPARAM(lparam);
			input_system_process_mouse_position(x, y);
		} break;
		case WM_MOUSEWHEEL: {
			i32 delta = GET_WHEEL_DELTA_WPARAM(wparam);
			if (delta != 0) {
				// Flatten delta for OS-independent handling
				delta = delta > 0 ? 1 : -1;
				input_system_process_mouse_wheel(delta);
			}
		} break;
		case WM_LBUTTONDOWN:
		case WM_MBUTTONDOWN:
		case WM_RBUTTONDOWN:
		case WM_LBUTTONUP:
		case WM_MBUTTONUP:
		case WM_RBUTTONUP: {
			b8 pressed = msg == WM_LBUTTONDOWN || msg == WM_MBUTTONDOWN || msg == WM_RBUTTONDOWN;
			if (msg == WM_LBUTTONDOWN || msg == WM_LBUTTONUP) {
				input_system_process_mouse_button(MOUSE_BUTTON_LEFT, pressed);
			} else if (msg == WM_MBUTTONDOWN || msg == WM_MBUTTONUP) {
				input_system_process_mouse_button(MOUSE_BUTTON_MIDDLE, pressed);
			} else if (msg == WM_RBUTTONDOWN || msg == WM_RBUTTONUP) {
				input_system_process_mouse_button(MOUSE_BUTTON_RIGHT, pressed);
			}
		} break;
	}

	return DefWindowProcA(hwnd, msg, wparam, lparam);
}

b8 platform_is_debugging(void) {
	return IsDebuggerPresent();
}

#endif // PLATFORM_WINDOWS
