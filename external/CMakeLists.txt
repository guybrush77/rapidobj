cmake_minimum_required(VERSION 3.14)

include(FetchContent)


#------------------------------------------------------------------------------------
# Fetch cereal
#------------------------------------------------------------------------------------

set(JUST_INSTALL_CEREAL ON)

FetchContent_Declare(
    cereal
    GIT_REPOSITORY  https://github.com/USCiLab/cereal
    GIT_TAG         v1.3.0
    GIT_SHALLOW     TRUE
)

FetchContent_MakeAvailable(cereal)


#------------------------------------------------------------------------------------
# Fetch cxxopt
#------------------------------------------------------------------------------------

set(CXXOPTS_BUILD_EXAMPLES OFF)
set(CXXOPTS_BUILD_TESTS OFF)

FetchContent_Declare(
    cxxopts
    GIT_REPOSITORY  https://github.com/jarro2783/cxxopts
    GIT_TAG         v2.2.1
    GIT_SHALLOW     TRUE
)

FetchContent_MakeAvailable(cxxopts)


#------------------------------------------------------------------------------------
# Fetch doctest
#------------------------------------------------------------------------------------

set(DOCTEST_WITH_TESTS OFF)
set(DOCTEST_NO_INSTALL ON)

FetchContent_Declare(
    doctest
    GIT_REPOSITORY  https://github.com/onqtam/doctest
    GIT_TAG         2.4.6
    GIT_SHALLOW     TRUE
)

FetchContent_MakeAvailable(doctest)


#------------------------------------------------------------------------------------
# Fetch OBJ-Loader
#------------------------------------------------------------------------------------

FetchContent_Declare(
    objloader
    GIT_REPOSITORY  https://github.com/Bly7/OBJ-Loader.git
    GIT_TAG         origin/master
    GIT_SHALLOW     TRUE
)

FetchContent_MakeAvailable(objloader)

add_library(objloader INTERFACE)

target_include_directories(objloader INTERFACE "${objloader_SOURCE_DIR}/Source")


#------------------------------------------------------------------------------------
# Fetch tinyobjloader
#------------------------------------------------------------------------------------

FetchContent_Declare(
    tinyobjloader
    GIT_REPOSITORY    https://github.com/tinyobjloader/tinyobjloader
    GIT_SHALLOW       TRUE
)

FetchContent_MakeAvailable(tinyobjloader)


#------------------------------------------------------------------------------------
# Fetch xxhash
#------------------------------------------------------------------------------------

FetchContent_Declare(
    xxhash
    GIT_REPOSITORY    https://github.com/Cyan4973/xxHash
    GIT_TAG           v0.8.0
    GIT_SHALLOW       TRUE
)

FetchContent_MakeAvailable(xxhash)

add_library(xxhash INTERFACE)

target_include_directories(xxhash INTERFACE "${xxhash_SOURCE_DIR}")
