cmake_minimum_required(VERSION 3.22 FATAL_ERROR)
cmake_policy(VERSION 3.22)
project(csv CXX)
set (CMAKE_CXX_STANDARD 20)

find_package(GTest REQUIRED)

add_subdirectory(${CMAKE_SOURCE_DIR}/src/hellolib)
add_subdirectory(${CMAKE_SOURCE_DIR}/src/helloapp)
add_subdirectory(${CMAKE_SOURCE_DIR}/test)