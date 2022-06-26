cmake_minimum_required(VERSION 3.20)

set(TEST_DATA_DIR ${CMAKE_CURRENT_SOURCE_DIR}/data)

add_subdirectory(func-tests)
add_subdirectory(unit-tests)

add_test(NAME unit-tests COMMAND $<TARGET_FILE:unit-tests>)
add_test(NAME func-tests COMMAND $<TARGET_FILE:func-tests>)
