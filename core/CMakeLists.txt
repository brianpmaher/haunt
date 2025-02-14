project(haunt_core LANGUAGES C VERSION "0.1.0")

add_subdirectory(deps/glad)

add_library(${PROJECT_NAME}
    SHARED
        src/arena.c
        src/assert.c
        src/core.c
        src/input.c
        src/log.c
)

if(WIN32)
    target_sources(${PROJECT_NAME}
        PRIVATE
            src/platform_windows.c
    )
endif()

target_include_directories(${PROJECT_NAME}
    PUBLIC
        include
    PRIVATE
        src
)

target_link_libraries(${PROJECT_NAME}
    PRIVATE
        glad
)

if(WIN32)
    target_link_libraries(${PROJECT_NAME}
        PRIVATE
            user32
            gdi32
            opengl32
    )
endif()

target_compile_definitions(${PROJECT_NAME}
    PRIVATE
        DLL_EXPORT
        VERSION="${PROJECT_VERSION}"
)

add_library(haunt::core ALIAS ${PROJECT_NAME})
