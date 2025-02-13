cmake_minimum_required(VERSION 3.16)

project(haunt)

set(CMAKE_C_STANDARD 11)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/$<CONFIGURATION>)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/$<CONFIGURATION>)

add_subdirectory(core)
add_subdirectory(runtime)
add_subdirectory(editor)
add_subdirectory(example)
