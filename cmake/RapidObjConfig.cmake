if (CMAKE_SYSTEM_NAME MATCHES "Linux")
    list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_LIST_DIR})
    find_package(Threads REQUIRED)
endif()

include(${CMAKE_CURRENT_LIST_DIR}/RapidObj.cmake)
