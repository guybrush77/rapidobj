function(rapidobj_get_version Arg)

    file(STRINGS include/rapidobj/rapidobj.hpp rapidobj_version_defines
        REGEX "#define RAPIDOBJ_VERSION_(MAJOR|MINOR|PATCH)")

    foreach(version_define ${rapidobj_version_defines})
        if(version_define MATCHES "#define RAPIDOBJ_VERSION_(MAJOR|MINOR|PATCH) +([^ ]+)$")
            set(RAPIDOBJ_VERSION_${CMAKE_MATCH_1} "${CMAKE_MATCH_2}" CACHE INTERNAL "")
        endif()
    endforeach()

    set(VERSION ${RAPIDOBJ_VERSION_MAJOR}.${RAPIDOBJ_VERSION_MINOR}.${RAPIDOBJ_VERSION_PATCH})

    message(STATUS "RapidObj version ${VERSION}")

    set(${Arg} ${VERSION} PARENT_SCOPE)

endfunction()
