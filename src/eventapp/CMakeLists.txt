
include_directories(
    ${CMAKE_SOURCE_DIR}/include
)

add_executable(eventapp
    main.cpp
)

target_link_libraries(eventapp PUBLIC
    purecomplib
)
