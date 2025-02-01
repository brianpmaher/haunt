#include "platform/platform.h"

#include "core/context.h"
#include "core/log.h"
#include "core/mem.h"

#ifdef PLATFORM_WINDOWS

#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <windowsx.h> // GET_X_LPARAM, GET_Y_LPARAM

typedef struct Clock {
	f64 frequency;
	LARGE_INTEGER start;
} Clock;

typedef struct Internal {
	const char* class_name;
	HINSTANCE hinst;
	// TODO: Handle multiple windows
	HWND hwnd;
	Clock clock;
} Internal;

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

static Internal* create_internal(void) {
	Internal* internal = mem_alloc(sizeof(Internal), MEM_TAG_PLATFORM);
	internal->class_name = "haunt_window_class";
	internal->hinst = GetModuleHandle(0);
	return internal;
}

static LRESULT CALLBACK process_message(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);

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
	i32 client_x = x;
	i32 client_y = y;
	i32 client_width = width;
	i32 client_height = height;
	i32 window_x = client_x;
	i32 window_y = client_y;
	i32 window_width = client_width;
	i32 window_height = client_height;

	// Setup window style
	i32 window_style = WS_OVERLAPPEDWINDOW | WS_SYSMENU | WS_CAPTION;
	i32 window_ex_style = WS_EX_APPWINDOW;
	window_style |= WS_MAXIMIZEBOX;
	window_style |= WS_MINIMIZEBOX;
	window_style |= WS_THICKFRAME;

	// Get border size
	RECT border_rect;
	AdjustWindowRectEx(&border_rect, window_style, false, window_ex_style);
	window_x += border_rect.left;
	window_y += border_rect.top;
	window_width += border_rect.right - border_rect.left;
	window_height += border_rect.bottom - border_rect.top;

	// Create window
	HWND handle = CreateWindowExA(
		window_ex_style, class_name, app_name,
		window_style, window_x, window_y, window_width, window_height,
		null, null, hinst, null);
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

static void show_window(Internal* internal) {
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
	// Initialize internal state
	platform->internal = create_internal();
	Internal* internal = (Internal*)platform->internal;

	// Create window
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

	// Start clock
	clock_start(&internal->clock);

	return true;
}

void platform_shutdown(Platform* platform) {
	Internal* internal = (Internal*)platform->internal;

	// Destroy window
	if (internal->hwnd) {
		DestroyWindow(internal->hwnd);
		internal->hwnd = null;
	}

	// Unregister window class
	UnregisterClassA(internal->class_name, internal->hinst);

	// Free internal state
	mem_free(platform->internal, sizeof(Internal), MEM_TAG_PLATFORM);
}

b8 platform_pump_messages(Platform* platform) {
	MSG msg;
	while (PeekMessageA(&msg, null, 0, 0, PM_REMOVE)) {
		TranslateMessage(&msg);
		DispatchMessageA(&msg);
	}
	return true;
}

static LRESULT CALLBACK process_message(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam) {
	switch (msg) {
		case WM_ERASEBKGND:
			// Tell OS that app is handling background erase to prevent flickering
			return 1;
		case WM_CLOSE:
			// TODO: Event handling
			return 0;
		case WM_DESTROY:
			PostQuitMessage(0);
			return 0;
		case WM_SIZE: {
			// RECT r;
			// GetClientRect(hwnd, &r);
			// u32 width = r.right - r.left;
			// u32 height = r.bottom - r.top;
			// TODO: Event handling
		} break;
		case WM_KEYDOWN:
		case WM_SYSKEYDOWN:
		case WM_KEYUP:
		case WM_SYSKEYUP: {
			// Key pressed/released
			// b8 pressed = msg == WM_KEYDOWN || msg == WM_SYSTEMKEYDOWN;
			// TODO: Event handling
		} break;
		case WM_MOUSEMOVE: {
			// Mouse move
			// i32 x = GET_X_LPARAM(lparam);
			// i32 y = GET_Y_LPARAM(lparam);
			// TODO: Event handling
		} break;
		case WM_MOUSEWHEEL: {
			// i32 delta = GET_WHEEL_DELTA_WPARAM(wparam);
			// if (delta != 0) {
			// 	// Flatten delta for OS-independent handling
			// 	delta = delta > 0 ? 1 : -1;
			// }
			// TODO: Event handling
		} break;
		case WM_LBUTTONDOWN:
		case WM_MBUTTONDOWN:
		case WM_RBUTTONDOWN:
		case WM_LBUTTONUP:
		case WM_MBUTTONUP:
		case WM_RBUTTONUP: {
			// b8 pressed = msg == WM_LBUTTONDOWN || msg == WM_MBUTTONDOWN || msg == WM_RBUTTONDOWN;
			// TODO: Event handling
		} break;
	}

	return DefWindowProcA(hwnd, msg, wparam, lparam);
}

void* platform_mem_alloc(u64 size, b8 aligned) {
	return VirtualAlloc(null, size, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);
}

void platform_mem_free(void* block, b8 aligned) {
	VirtualFree(block, 0, MEM_RELEASE);
}

void* platform_mem_zero(void* block, u64 size) {
	ZeroMemory(block, size);
	return block;
}

void* platform_mem_copy(void* dest, const void* src, u64 size) {
	CopyMemory(dest, src, size);
	return dest;
}

void* platform_mem_set(void* dest, i32 value, u64 size) {
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
	Internal* internal = (Internal*)platform->internal;

	LARGE_INTEGER current;
	QueryPerformanceCounter(&current);
	return (f64)current.QuadPart * internal->clock.frequency;
}

void platform_sleep(u64 ms) {
	Sleep(ms);
}

#endif
