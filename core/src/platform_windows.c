#include "platform.h"

#include "haunt/core/graphics.h"
#include "haunt/core/input.h"
#include "haunt/core/log.h"

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <Windowsx.h>

#include <glad/glad.h>

#include <GL/glcorearb.h>
#include <GL/glext.h>
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
	HDC device_context;
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

static PFNWGLCHOOSEPIXELFORMATARBPROC wglChoosePixelFormatARB = null;
static PFNWGLCREATECONTEXTATTRIBSARBPROC wglCreateContextAttribsARB = null;
static PFNWGLSWAPINTERVALEXTPROC wglSwapIntervalEXT = null;

static void set_window_data(HWND hwnd, Platform* platform) {
	SetWindowLongPtr(hwnd, GWLP_USERDATA, (LONG_PTR)platform);
}

static Platform* get_window_data(HWND hwnd) {
	LONG_PTR ptr = GetWindowLongPtr(hwnd, GWLP_USERDATA);
	return (Platform*)ptr;
}

static LRESULT CALLBACK process_message(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam) {
	Platform* platform = get_window_data(hwnd);

	switch (msg) {
	case WM_ERASEBKGND:
		// Tell OS that engine is handling background erase to prevent flickering
		return 1;
	case WM_CLOSE:
		platform->should_close = true;
		return 0;
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	case WM_KEYDOWN:
	case WM_SYSKEYDOWN:
	case WM_KEYUP:
	case WM_SYSKEYUP: {
		b8 pressed = msg == WM_KEYDOWN || msg == WM_SYSKEYDOWN;
		WORD vkcode = LOWORD(wparam);
		Key key = (Key)vkcode;
		input_process_key(key, pressed);
	} break;
	case WM_MOUSEMOVE: {
		i32 x = GET_X_LPARAM(lparam);
		i32 y = GET_Y_LPARAM(lparam);
		input_process_mouse_position(x, y);
	} break;
	case WM_MOUSEWHEEL: {
		i32 delta = GET_WHEEL_DELTA_WPARAM(wparam);
		if (delta != 0) {
			// Flatten delta for OS-independent handling
			delta = delta > 0 ? 1 : -1;
			input_process_mouse_wheel(delta);
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
			input_process_mouse_button(MOUSE_BUTTON_LEFT, pressed);
		} else if (msg == WM_MBUTTONDOWN || msg == WM_MBUTTONUP) {
			input_process_mouse_button(MOUSE_BUTTON_MIDDLE, pressed);
		} else if (msg == WM_RBUTTONDOWN || msg == WM_RBUTTONUP) {
			input_process_mouse_button(MOUSE_BUTTON_RIGHT, pressed);
		}
	} break;
	}

	return DefWindowProcA(hwnd, msg, wparam, lparam);
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

static HWND create_window(
	HINSTANCE hinst, const char* class_name, const char* app_name, i32 x, i32 y, i32 width,
	i32 height) {
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
		window_ex_style, class_name, app_name, window_style, window_x, window_y, window_width,
		window_height, 0, 0, hinst, 0);
	if (!handle) {
		MessageBoxA(null, "Failed to create window", "Error", MB_OK | MB_ICONERROR);
		log_fatal("Failed to create window");
		return null;
	}

	return handle;
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

static Platform_Internal* create_internal(void) {
	Platform_Internal* internal = platform_memory_alloc(sizeof(Platform_Internal));
	internal->class_name = "haunt_window_class";
	internal->hinst = GetModuleHandle(0);
	return internal;
}

static void clock_start(Clock* clock) {
	LARGE_INTEGER frequency;
	QueryPerformanceFrequency(&frequency);
	clock->frequency = 1.0 / (f64)frequency.QuadPart;
	QueryPerformanceCounter(&clock->start);
}

static b8 cstring_equals(const char* a, const char* b, u64 blen) {
	while (*a && blen-- && *b) {
		if (*a++ != *b++) {
			return false;
		}
	}

	return (blen && *a == *b) || (!blen && *a == 0);
}

static void load_wgl_functions(void) {
	// WGL functions need valid GL context, so create dummy window for dummy GL context
	HWND dummy = CreateWindowExA(
		0, "STATIC", "DummyWindow", WS_OVERLAPPED, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT,
		CW_USEDEFAULT, NULL, NULL, NULL, NULL);
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

	i32 format = ChoosePixelFormat(dc, &desc);
	if (!format) {
		log_fatal("Cannot choose OpenGL pixel format for dummy window!");
	}

	i32 ok = DescribePixelFormat(dc, format, sizeof(desc), &desc);
	assert_message(ok, "Failed to describe OpenGL pixel format");

	// Reason to create dummy window is that SetPixelFormat can be called only once for the window
	if (!SetPixelFormat(dc, format, &desc)) {
		log_fatal("Cannot set OpenGL pixel format for dummy window!");
	}

	HGLRC rc = wglCreateContext(dc);
	assert_message(rc, "Failed to create OpenGL context for dummy window");

	ok = wglMakeCurrent(dc, rc);
	assert_message(ok, "Failed to make current OpenGL context for dummy window");

	// https://www.khronos.org/registry/OpenGL/extensions/ARB/WGL_ARB_extensions_string.txt
	PFNWGLGETEXTENSIONSSTRINGARBPROC wglGetExtensionsStringARB =
		(void*)wglGetProcAddress("wglGetExtensionsStringARB");
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
		if (cstring_equals("WGL_ARB_pixel_format", start, length)) {
			// https://www.khronos.org/registry/OpenGL/extensions/ARB/WGL_ARB_pixel_format.txt
			wglChoosePixelFormatARB = (void*)wglGetProcAddress("wglChoosePixelFormatARB");
		} else if (cstring_equals("WGL_ARB_create_context", start, length)) {
			// https://www.khronos.org/registry/OpenGL/extensions/ARB/WGL_ARB_create_context.txt
			wglCreateContextAttribsARB = (void*)wglGetProcAddress("wglCreateContextAttribsARB");
		} else if (cstring_equals("WGL_EXT_swap_control", start, length)) {
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

b8 platform_init(Platform* platform, const char* app_name, i32 x, i32 y, i32 width, i32 height) {
	platform->internal = create_internal();
	Platform_Internal* internal = (Platform_Internal*)platform->internal;

	load_wgl_functions();

	// Register window class and create window
	if (!register_window_class(internal->hinst, internal->class_name)) {
		log_fatal("Failed to register window class");
		return false;
	}
	internal->hwnd =
		create_window(internal->hinst, internal->class_name, app_name, x, y, width, height);
	if (!internal->hwnd) {
		log_fatal("Failed to create window");
		return false;
	}

	set_window_data(internal->hwnd, platform);

	internal->device_context = GetDC(internal->hwnd);
	HDC dc = internal->device_context;
	// Set pixel format for OpenGL context
	{
		i32 attrib[] = {
			WGL_DRAW_TO_WINDOW_ARB,
			GL_TRUE,
			WGL_SUPPORT_OPENGL_ARB,
			GL_TRUE,
			WGL_DOUBLE_BUFFER_ARB,
			GL_TRUE,
			WGL_PIXEL_TYPE_ARB,
			WGL_TYPE_RGBA_ARB,
			WGL_COLOR_BITS_ARB,
			24,
			WGL_DEPTH_BITS_ARB,
			24,
			WGL_STENCIL_BITS_ARB,
			8,

#if GRAPHICS_SRGB_ENABLED
			// sRGB framebuffer from WGL_ARB_framebuffer_sRGB extension
		    // https://www.khronos.org/registry/OpenGL/extensions/ARB/ARB_framebuffer_sRGB.txt
			WGL_FRAMEBUFFER_SRGB_CAPABLE_ARB,
			GL_TRUE,
#endif

// TODO: Use Graphics_Settings to set vsync and msaa
#if 0
			// Multisampled framebuffer from WGL_ARB_multisample extension
		    // https://www.khronos.org/registry/OpenGL/extensions/ARB/ARB_multisample.txt
			WGL_SAMPLE_BUFFERS_ARB,
			1,
			WGL_SAMPLES_ARB,
			4, // 4x MSAA
#endif

			0,
		};

		i32 format;
		u32 formats;
		if (!wglChoosePixelFormatARB(dc, attrib, NULL, 1, &format, &formats) || formats == 0) {
			log_fatal("OpenGL does not support required pixel format!");
		}

		PIXELFORMATDESCRIPTOR desc = {.nSize = sizeof(desc)};
		i32 ok = DescribePixelFormat(dc, format, sizeof(desc), &desc);
		assert(ok && "Failed to describe OpenGL pixel format");

		if (!SetPixelFormat(dc, format, &desc)) {
			log_fatal("Cannot set OpenGL selected pixel format!");
		}
	}

	// Create modern OpenGL context
	{
		i32 attrib[] = {
			WGL_CONTEXT_MAJOR_VERSION_ARB,
			4,
			WGL_CONTEXT_MINOR_VERSION_ARB,
			6,
			WGL_CONTEXT_PROFILE_MASK_ARB,
			WGL_CONTEXT_CORE_PROFILE_BIT_ARB,
#if GRAPHICS_DEBUG_ENABLED
			// Ask for debug context for debug builds to enable debug callback
			WGL_CONTEXT_FLAGS_ARB,
			WGL_CONTEXT_DEBUG_BIT_ARB,
#endif
			0,
		};

		HGLRC rc = wglCreateContextAttribsARB(dc, NULL, attrib);
		if (!rc) {
			log_fatal("Cannot create modern OpenGL context! OpenGL version 4.6 not supported?");
		}

		b32 ok = wglMakeCurrent(dc, rc);
		assert(ok && "Failed to make current OpenGL context");

		gladLoadGL();
	}

	wglSwapIntervalEXT(platform->graphics_settings.vsync ? 1 : 0);

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

	platform_memory_free(platform->internal);
}

b8 platform_pump_messages(Platform* platform) {
	MSG msg;
	while (PeekMessageA(&msg, null, 0, 0, PM_REMOVE)) {
		TranslateMessage(&msg);
		DispatchMessageA(&msg);
	}
	return true;
}

b8 platform_swap_buffers(Platform* platform) {
	Platform_Internal* internal = (Platform_Internal*)platform->internal;
	return SwapBuffers(internal->device_context);
}

void* platform_memory_reserve(u64 size) {
	return VirtualAlloc(null, size, MEM_RESERVE, PAGE_READWRITE);
}

void* platform_memory_commit(u64 size) {
	return VirtualAlloc(null, size, MEM_COMMIT, PAGE_READWRITE);
}

void* platform_memory_alloc(u64 size) {
	return VirtualAlloc(null, size, MEM_RESERVE | MEM_COMMIT, PAGE_READWRITE);
}

void platform_memory_free(void* block) {
	VirtualFree(block, 0, MEM_RELEASE);
}

void* platform_memory_copy(void* dest, const void* src, u64 size) {
	CopyMemory(dest, src, size);
	return dest;
}

void* platform_memory_set(void* dest, i32 value, u64 size) {
	FillMemory(dest, size, value);
	return dest;
}

void* platform_memory_zero(void* block, u64 size) {
	ZeroMemory(block, size);
	return block;
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

f64 platform_get_time(Platform* platform) {
	Platform_Internal* internal = (Platform_Internal*)platform->internal;

	LARGE_INTEGER current;
	QueryPerformanceCounter(&current);
	return (f64)current.QuadPart * internal->clock.frequency;
}

void platform_sleep(u64 ms) {
	Sleep(ms);
}

b8 platform_is_debugging(void) {
	return IsDebuggerPresent();
}

Vec2i platform_get_window_size(Platform* platform) {
	Platform_Internal* internal = (Platform_Internal*)platform->internal;

	RECT r;
	GetClientRect(internal->hwnd, &r);
	u32 width = r.right - r.left;
	u32 height = r.bottom - r.top;
	return (Vec2i){(i32)width, (i32)height};
}
