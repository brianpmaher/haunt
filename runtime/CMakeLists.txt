project(haunt_runtime LANGUAGES C)

add_library(${PROJECT_NAME}
    SHARED
        src/runtime.c
)

target_include_directories(${PROJECT_NAME}
    PUBLIC
        include
    PRIVATE
        src
)

target_link_libraries(${PROJECT_NAME}
    PUBLIC
        haunt::core
)

target_compile_definitions(${PROJECT_NAME}
    PRIVATE
        DLL_EXPORT
)

add_library(haunt::runtime ALIAS ${PROJECT_NAME})
