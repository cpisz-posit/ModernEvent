include_directories(
    ${CMAKE_SOURCE_DIR}/include
    ${GTEST_INCLUDE_DIRS}
)

add_executable(eventtest
    main.cpp
    testSessionEvents.cpp
)

target_link_libraries(eventtest PUBLIC
    classiclib
    purecomplib
    GTest::gtest
    GTest::gmock
)