project(haunt_engine)

add_library(${PROJECT_NAME} src/haunt.c)

# TODO: Don't export src publicly
target_include_directories(${PROJECT_NAME}
    PUBLIC
        include
        src
)
