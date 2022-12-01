cmake_minimum_required(VERSION 3.10)

set(PROJECT_NAME translator)
project(${PROJECT_NAME})

set(MP2_TESTS   "test_${PROJECT_NAME}")
set(MP2_CUSTOM_PROJECT "${PROJECT_NAME}")
set(MP2_INCLUDE "${CMAKE_CURRENT_SOURCE_DIR}/include")

add_subdirectory(include)
add_subdirectory(gtest)
add_subdirectory(test)