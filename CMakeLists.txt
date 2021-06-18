cmake_minimum_required(VERSION 3.14)

set(CMAKE_POLICY_DEFAULT_CMP0048 NEW)
set(CMAKE_POLICY_DEFAULT_CMP0077 NEW)

list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_LIST_DIR}/cmake)

include(RapidObjGetVersion)
include(GNUInstallDirs)
include(CMakePackageConfigHelpers)

rapidobj_get_version(VERSION)

project(RapidObj VERSION ${VERSION} LANGUAGES CXX)

option(RAPIDOBJ_BuildTests "Build Tests." OFF)
option(RAPIDOBJ_BuildTools "Build Tools." OFF)
option(RAPIDOBJ_BuildExamples "Build Examples." OFF)

if (RAPIDOBJ_BuildTests OR RAPIDOBJ_BuildTools)
    add_subdirectory(external EXCLUDE_FROM_ALL)
endif()

add_subdirectory(include)

if (RAPIDOBJ_BuildTests)
    add_subdirectory(tests)
endif()

if (RAPIDOBJ_BuildTools)
    add_subdirectory(tools)
endif()

if (RAPIDOBJ_BuildExamples)
    add_subdirectory(example)
endif()
