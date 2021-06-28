message(STATUS "Looking for aio library")

find_path(AIO_INCLUDE_DIR libaio.h)

find_library(AIO_LIBRARIES aio)

if (NOT AIO_INCLUDE_DIR OR NOT AIO_LIBRARIES)
    message("  Failed to find aio library")
    message("  To install aio on Debian: sudo apt install libaio-dev")
    message("  To install aio on RHEL or Fedora: sudo yum install libaio-devel")
endif()

include(FindPackageHandleStandardArgs)

find_package_handle_standard_args(Aio DEFAULT_MSG AIO_LIBRARIES AIO_INCLUDE_DIR)

mark_as_advanced(AIO_INCLUDE_DIR AIO_LIBRARIES)
