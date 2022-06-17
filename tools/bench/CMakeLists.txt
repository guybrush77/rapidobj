cmake_minimum_required(VERSION 3.20)

add_executable(bench)

target_sources(bench PRIVATE "src/bench.cpp")

target_compile_features(bench PRIVATE cxx_std_17)

target_link_libraries(bench PRIVATE cxxopts fast_obj rapidobj tinyobjloader)
