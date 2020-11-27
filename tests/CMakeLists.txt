project(tests)

add_subdirectory(googletest-1.10.0)
include_directories(${gtest_SOURCE_DIR}/include ${gtest_SOURCE_DIR})

add_executable(tests_run test_read_command.cpp)

target_link_libraries(tests_run teru_dap)
target_link_libraries(tests_run gtest gtest_main)
