
include_directories(
    ${CMAKE_SOURCE_DIR}/include
)

add_executable(helloapp
    main.cpp
)

target_link_libraries(helloapp PUBLIC
    hellolib
)
