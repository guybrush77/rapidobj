cmake_minimum_required(VERSION 3.20)

add_executable(make-test)

target_sources(make-test PRIVATE "src/make-test.cpp")

target_compile_features(make-test PRIVATE cxx_std_17)

target_link_libraries(make-test PRIVATE cxxopts serializer tinyobjloader)
