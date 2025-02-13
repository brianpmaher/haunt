project(haunt_core LANGUAGES C VERSION "0.1.0")

add_subdirectory(deps/glad)

add_library(${PROJECT_NAME}
    SHARED
        src/assert.c
        src/core.c
        src/log.c
)
add_library(haunt::core ALIAS ${PROJECT_NAME})

target_include_directories(${PROJECT_NAME}
    PUBLIC
        include
    PRIVATE
        src
)

target_compile_definitions(${PROJECT_NAME}
    PRIVATE
        DLL_EXPORT
        VERSION="${PROJECT_VERSION}"
)
