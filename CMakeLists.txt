cmake_minimum_required(VERSION 3.20)

set(CMAKE_POLICY_DEFAULT_CMP0048 NEW)
set(CMAKE_POLICY_DEFAULT_CMP0077 NEW)

list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_LIST_DIR}/cmake)

include(RapidObjGetVersion)

rapidobj_get_version(VERSION)

project(RapidObj VERSION ${VERSION} LANGUAGES CXX)

include(GNUInstallDirs)
include(CMakePackageConfigHelpers)

option(RAPIDOBJ_BuildTests "Build Tests." OFF)
option(RAPIDOBJ_BuildTools "Build Tools." OFF)
option(RAPIDOBJ_BuildExamples "Build Examples." OFF)

set(RAPIDOBJ_INCLUDE_DIR "${CMAKE_INSTALL_INCLUDEDIR}" CACHE STRING "Header Include Folder.")
set(RAPIDOBJ_CMAKE_DIR "${CMAKE_INSTALL_LIBDIR}/cmake" CACHE STRING "CMake Config Folder.")

if (RAPIDOBJ_BuildTests OR RAPIDOBJ_BuildTools)
    add_subdirectory(external EXCLUDE_FROM_ALL)
endif()

add_subdirectory(include)

if (RAPIDOBJ_BuildTests)
    include(CTest)
    add_subdirectory(tests)
endif()

if (RAPIDOBJ_BuildTools)
    add_subdirectory(tools)
endif()

if (RAPIDOBJ_BuildExamples)
    add_subdirectory(example)
endif()
