cmake_minimum_required(VERSION 3.30)
project(listlib C)

set(CMAKE_C_STANDARD 23)

add_library(list STATIC src/list.c include/list.h)

target_include_directories(list PUBLIC include)

enable_testing()
add_subdirectory(tests)
