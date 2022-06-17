cmake_minimum_required(VERSION 3.20)

add_executable(readobj)

target_sources(readobj PRIVATE "src/readobj.cpp")

target_compile_features(readobj PRIVATE cxx_std_17)

target_link_libraries(readobj PRIVATE rapidobj)
