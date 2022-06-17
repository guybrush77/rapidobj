cmake_minimum_required(VERSION 3.20)

add_executable(func-tests)

target_sources(func-tests PRIVATE
    "src/test_main.cpp"
    "src/test_rapidobj.cpp"
)

target_compile_features(func-tests PRIVATE cxx_std_17)

target_compile_definitions(func-tests PUBLIC TEST_DATA_DIR=${TEST_DATA_DIR})

target_link_libraries(func-tests PRIVATE doctest compare-test)
