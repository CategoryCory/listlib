cmake_minimum_required(VERSION 3.30)
project(listlib VERSION 0.1.1 LANGUAGES C)

set(CMAKE_C_STANDARD 23)

add_library(list STATIC src/list.c include/list.h)

target_include_directories(list PUBLIC include)

enable_testing()
add_subdirectory(tests)

add_custom_target(doc
        COMMAND doxygen ${CMAKE_SOURCE_DIR}/Doxyfile
        WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
        COMMENT "Generating API documentation with Doxygen"
)