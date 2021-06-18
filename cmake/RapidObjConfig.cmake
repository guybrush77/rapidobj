if (CMAKE_SYSTEM_NAME MATCHES "Linux")
    list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_LIST_DIR})
    find_package(Threads REQUIRED)
    find_package(Aio REQUIRED)
endif()

include(${CMAKE_CURRENT_LIST_DIR}/RapidObj.cmake)
