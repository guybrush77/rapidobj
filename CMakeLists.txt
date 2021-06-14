cmake_minimum_required(VERSION 3.14)

project(rapidobj LANGUAGES CXX)

set(CMAKE_POLICY_DEFAULT_CMP0077 NEW)

add_subdirectory(external)
add_subdirectory(include)
add_subdirectory(tests)
add_subdirectory(tools)
add_subdirectory(example)
