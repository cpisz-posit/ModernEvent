cmake_minimum_required(VERSION 3.24 FATAL_ERROR)
cmake_policy(VERSION 3.24)
project(csv CXX)
set (CMAKE_CXX_STANDARD 20)

find_package(GTest REQUIRED)

add_subdirectory(${CMAKE_SOURCE_DIR}/src/classiclib)
add_subdirectory(${CMAKE_SOURCE_DIR}/src/purecomplib)
add_subdirectory(${CMAKE_SOURCE_DIR}/src/templatecastlib)
add_subdirectory(${CMAKE_SOURCE_DIR}/src/benchapp)
add_subdirectory(${CMAKE_SOURCE_DIR}/test)