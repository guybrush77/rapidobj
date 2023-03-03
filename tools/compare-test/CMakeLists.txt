cmake_minimum_required(VERSION 3.20)

add_library(compare-test STATIC)

target_sources(compare-test PRIVATE
    "include/compare-test/compare-test.hpp"
    "src/compare-test.cpp"
)

target_include_directories(compare-test PUBLIC include)

target_compile_features(compare-test PRIVATE cxx_std_17)

target_link_libraries(compare-test PUBLIC rapidobj PRIVATE serializer)
