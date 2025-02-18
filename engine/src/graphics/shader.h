#pragma once

#include "core/types.h"

typedef struct Shader {
    u32 program_id;  // OpenGL shader program ID
    u32 vertex_id;   // Vertex shader ID
    u32 fragment_id; // Fragment shader ID
    b8 is_valid;     // Indicates if shader compiled successfully
} Shader;

// Create and compile a shader from source files
b8 shader_create(
    Shader* out_shader,
    const char* vertex_source,
    const char* fragment_source);

// Destroy a shader and free resources
void shader_destroy(Shader* shader);

// Bind a shader for use
void shader_bind(Shader* shader);

// Unbind the currently bound shader
void shader_unbind(void);

// Check if a shader is valid
b8 shader_is_valid(Shader* shader);

// Add this new function declaration
b8 shader_create_from_files(
    Shader* out_shader,
    const char* vertex_path,
    const char* fragment_path); 