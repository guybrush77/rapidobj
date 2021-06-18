cmake_minimum_required(VERSION 3.14)

add_library(rapidobj INTERFACE)
add_library(rapidobj::rapidobj ALIAS rapidobj)

target_compile_features(rapidobj INTERFACE cxx_std_17)

target_include_directories(rapidobj INTERFACE
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
    $<INSTALL_INTERFACE:.>
)

if(CMAKE_SYSTEM_NAME MATCHES "Linux")
    find_package(Threads REQUIRED)
    find_package(Aio REQUIRED)
    target_link_libraries(rapidobj INTERFACE Threads::Threads aio)
    target_link_libraries(rapidobj INTERFACE $<$<AND:$<CXX_COMPILER_ID:GNU>,$<VERSION_LESS:$<CXX_COMPILER_VERSION>,9.0>>:stdc++fs>)
endif()

write_basic_package_version_file(RapidObjConfigVersion.cmake VERSION ${VERSION} COMPATIBILITY AnyNewerVersion)

install(TARGETS rapidobj EXPORT RapidObj DESTINATION ${CMAKE_INSTALL_LIBDIR})
install(EXPORT RapidObj DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/RapidObj NAMESPACE rapidobj::)
install(FILES ${PROJECT_SOURCE_DIR}/cmake/RapidObjConfig.cmake DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/RapidObj)
install(FILES ${PROJECT_BINARY_DIR}/include/RapidObjConfigVersion.cmake DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/RapidObj)
install(DIRECTORY rapidobj DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})

if(CMAKE_SYSTEM_NAME MATCHES "Linux")
    install(FILES ${PROJECT_SOURCE_DIR}/cmake/FindAio.cmake DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/RapidObj)
endif()
