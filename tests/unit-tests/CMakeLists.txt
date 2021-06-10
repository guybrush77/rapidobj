cmake_minimum_required(VERSION 3.14)

add_executable(unit-tests)

target_sources(unit-tests PRIVATE
   "src/test_main.cpp"
   "src/test_material_parsing.cpp"
   "src/test_parsing.cpp"
)

target_compile_features(unit-tests PRIVATE cxx_std_17)

target_link_libraries(unit-tests PRIVATE doctest rapidobj)
