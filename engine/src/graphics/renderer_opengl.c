#include "graphics/renderer.h"
#include "graphics/renderer_opengl.h"

#include "core/log.h"
#include "core/context.h"
#include "core/types.h"

#include "platform/platform.h"


// TODO: Linux implementation?
#if GL_DEBUG_ENABLED
static void gl_debug_callback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar *message, const void *user) {
	log_debug(message);

	if (severity == GL_DEBUG_SEVERITY_HIGH || severity == GL_DEBUG_SEVERITY_MEDIUM) {
		if (platform_is_debugging()) {
			assert_message(false, "OpenGL error - check the callstack in debugger");
		}

		log_fatal("OpenGL API usage error! Use debugger to examine call stack!");
	}
}
#endif

void renderer_init(void) {
#if GL_DEBUG_ENABLED
	glDebugMessageCallback(&gl_debug_callback, NULL);
	glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
#endif
}

void renderer_resize(i32 width, i32 height) {
	glViewport(0, 0, width, height);
}

void set_clear_color(Color color) {
	glClearColor(color.r, color.g, color.b, color.a);
}

// TODO: Pass other clear bits?
void clear_screen(void) {
	glClear(GL_COLOR_BUFFER_BIT);
}
