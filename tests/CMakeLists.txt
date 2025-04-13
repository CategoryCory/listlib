add_executable(list_tests test_list.c unity.c)

target_include_directories(list_tests PRIVATE
    ${PROJECT_SOURCE_DIR}/include
    ${CMAKE_CURRENT_SOURCE_DIR})

target_link_libraries(list_tests PRIVATE list)

add_test(NAME ListTests COMMAND list_tests)