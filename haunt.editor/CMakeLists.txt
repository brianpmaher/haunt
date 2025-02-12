project(haunt_runtime LANGUAGES C VERSION "0.1.0")

add_subdirectory(../haunt.core/deps/SDL)

add_library(${PROJECT_NAME} SHARED src/core.c)
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
