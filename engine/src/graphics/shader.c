#include "graphics/shader.h"
#include "graphics/renderer_opengl.h"
#include "core/log.h"
#include "core/memory.h"
#include "core/types.h"

#include <glad/glad.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static b8 compile_shader(u32* out_id, const char* source, GLenum type) {
    *out_id = glCreateShader(type);
    glShaderSource(*out_id, 1, &source, NULL);
    glCompileShader(*out_id);

    // Check compilation status
    i32 success;
    glGetShaderiv(*out_id, GL_COMPILE_STATUS, &success);
    if (!success) {
        char info_log[512];
        glGetShaderInfoLog(*out_id, 512, NULL, info_log);
        log_error("Shader compilation failed: %s", info_log);
        return false;
    }

    return true;
}

static char* read_file(const char* path) {
    FILE* file = fopen(path, "rb");
    if (!file) {
        log_error("Failed to open shader file: %s", path);
        return null;
    }

    // Get file size
    fseek(file, 0, SEEK_END);
    size_t size = ftell(file);
    fseek(file, 0, SEEK_SET);

    // Allocate buffer for file content plus null terminator
    char* buffer = memory_alloc(size + 1, MEMORY_TAG_SHADER);
    if (!buffer) {
        log_error("Failed to allocate memory for shader file: %s", path);
        fclose(file);
        return null;
    }

    // Read file content
    size_t read = fread(buffer, 1, size, file);
    fclose(file);

    if (read != size) {
        log_error("Failed to read shader file: %s", path);
        memory_free(buffer, size + 1, MEMORY_TAG_SHADER);
        return null;
    }

    // Null terminate the string
    buffer[size] = '\0';
    return buffer;
}

b8 shader_create(Shader* out_shader, const char* vertex_source, const char* fragment_source) {
    out_shader->is_valid = false;

    // Compile vertex shader
    if (!compile_shader(&out_shader->vertex_id, vertex_source, GL_VERTEX_SHADER)) {
        log_error("Failed to compile vertex shader");
        return false;
    }

    // Compile fragment shader
    if (!compile_shader(&out_shader->fragment_id, fragment_source, GL_FRAGMENT_SHADER)) {
        log_error("Failed to compile fragment shader");
        glDeleteShader(out_shader->vertex_id);
        return false;
    }

    // Create and link program
    out_shader->program_id = glCreateProgram();
    glAttachShader(out_shader->program_id, out_shader->vertex_id);
    glAttachShader(out_shader->program_id, out_shader->fragment_id);
    glLinkProgram(out_shader->program_id);

    // Check linking status
    i32 success;
    glGetProgramiv(out_shader->program_id, GL_LINK_STATUS, &success);
    if (!success) {
        char info_log[512];
        glGetProgramInfoLog(out_shader->program_id, 512, NULL, info_log);
        log_error("Shader program linking failed: %s", info_log);
        glDeleteShader(out_shader->vertex_id);
        glDeleteShader(out_shader->fragment_id);
        glDeleteProgram(out_shader->program_id);
        return false;
    }

    // Clean up individual shaders
    glDeleteShader(out_shader->vertex_id);
    glDeleteShader(out_shader->fragment_id);

    out_shader->is_valid = true;
    return true;
}

b8 shader_create_from_files(Shader* out_shader, const char* vertex_path, const char* fragment_path) {
    // Read vertex shader
    char* vertex_source = read_file(vertex_path);
    if (!vertex_source) {
        return false;
    }

    // Read fragment shader
    char* fragment_source = read_file(fragment_path);
    if (!fragment_source) {
        memory_free(vertex_source, strlen(vertex_source) + 1, MEMORY_TAG_SHADER);
        return false;
    }

    // Create shader from sources
    b8 result = shader_create(out_shader, vertex_source, fragment_source);

    // Clean up
    memory_free(vertex_source, strlen(vertex_source) + 1, MEMORY_TAG_SHADER);
    memory_free(fragment_source, strlen(fragment_source) + 1, MEMORY_TAG_SHADER);

    return result;
}

void shader_destroy(Shader* shader) {
    if (shader->program_id) {
        glDeleteProgram(shader->program_id);
    }
    shader->program_id = 0;
    shader->vertex_id = 0;
    shader->fragment_id = 0;
    shader->is_valid = false;
}

void shader_bind(Shader* shader) {
    if (shader && shader->is_valid) {
        glUseProgram(shader->program_id);
    }
}

void shader_unbind(void) {
    glUseProgram(0);
}

b8 shader_is_valid(Shader* shader) {
    return shader && shader->is_valid;
} 