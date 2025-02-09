#include "renderer/opengl.h"

#include "core/log.h"
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

void gl_init(void) {
	glDebugMessageCallback(&gl_debug_callback, NULL);
	glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
}
