project(glad LANGUAGES C VERSION "0.1.36")

add_library(${PROJECT_NAME} STATIC src/glad.c)

target_include_directories(${PROJECT_NAME}
    PUBLIC
        include
    PRIVATE
        ../GL/include
        ../KHR/include
)
