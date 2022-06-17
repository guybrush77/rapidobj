cmake_minimum_required(VERSION 3.20)

add_library(serializer STATIC)

target_sources(serializer PRIVATE
    "include/serializer/serializer.hpp"
    "src/serializer.cpp"
)

target_include_directories(serializer PUBLIC include)

target_compile_features(serializer PRIVATE cxx_std_17)

target_link_libraries(serializer PUBLIC rapidobj PRIVATE cereal xxhash)
