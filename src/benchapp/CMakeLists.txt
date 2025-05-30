
find_package(benchmark REQUIRED)

include_directories(
    ${CMAKE_SOURCE_DIR}/include
)

add_executable(benchapp
    main.cpp
)

target_link_libraries(benchapp PUBLIC
    classiclib
    purecomplib
    templatecastlib
    benchmark::benchmark
)
