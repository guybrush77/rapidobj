cmake_minimum_required(VERSION 3.14)

add_library(rapidobj INTERFACE)

target_compile_features(rapidobj INTERFACE cxx_std_17)

target_sources(rapidobj INTERFACE "rapidobj/rapidobj.hpp")

if (UNIX AND NOT APPLE)
    find_package(Threads REQUIRED)
    target_link_libraries(rapidobj INTERFACE Threads::Threads aio)
endif()

target_include_directories(rapidobj INTERFACE .)
