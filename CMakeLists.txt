cmake_minimum_required(VERSION 3.22 FATAL_ERROR)
cmake_policy(VERSION 3.22)
project(csv CXX)
set (CMAKE_CXX_STANDARD 20)

find_package(GTest REQUIRED)

add_subdirectory(${CMAKE_SOURCE_DIR}/src/eventlib)
add_subdirectory(${CMAKE_SOURCE_DIR}/src/eventapp)
add_subdirectory(${CMAKE_SOURCE_DIR}/test)