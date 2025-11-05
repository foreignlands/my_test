# generated from ament/cmake/core/templates/nameConfig.cmake.in

# prevent multiple inclusion
if(_server_CONFIG_INCLUDED)
  # ensure to keep the found flag the same
  if(NOT DEFINED server_FOUND)
    # explicitly set it to FALSE, otherwise CMake will set it to TRUE
    set(server_FOUND FALSE)
  elseif(NOT server_FOUND)
    # use separate condition to avoid uninitialized variable warning
    set(server_FOUND FALSE)
  endif()
  return()
endif()
set(_server_CONFIG_INCLUDED TRUE)

# output package information
if(NOT server_FIND_QUIETLY)
  message(STATUS "Found server: 0.0.0 (${server_DIR})")
endif()

# warn when using a deprecated package
if(NOT "" STREQUAL "")
  set(_msg "Package 'server' is deprecated")
  # append custom deprecation text if available
  if(NOT "" STREQUAL "TRUE")
    set(_msg "${_msg} ()")
  endif()
  # optionally quiet the deprecation message
  if(NOT ${server_DEPRECATED_QUIET})
    message(DEPRECATION "${_msg}")
  endif()
endif()

# flag package as ament-based to distinguish it after being find_package()-ed
set(server_FOUND_AMENT_PACKAGE TRUE)

# include all config extra files
set(_extras "")
foreach(_extra ${_extras})
  include("${server_DIR}/${_extra}")
endforeach()
