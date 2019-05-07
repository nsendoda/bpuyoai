cmake_minimum_required(VERSION 2.8)

add_executable(field-test main.cpp)
target_link_libraries(field-test
    gtest
    gtest_main
    pthread
    )

add_test(
    NAME field
    COMMAND $<TARGET_FILE:field-test>
    )

# run with: ctest -L lib
set_property(
    TEST field
    PROPERTY LABELS lib field
    )
