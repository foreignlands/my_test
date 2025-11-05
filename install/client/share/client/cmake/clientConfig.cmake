# generated from ament/cmake/core/templates/nameConfig.cmake.in

# prevent multiple inclusion
if(_client_CONFIG_INCLUDED)
  # ensure to keep the found flag the same
  if(NOT DEFINED client_FOUND)
    # explicitly set it to FALSE, otherwise CMake will set it to TRUE
    set(client_FOUND FALSE)
  elseif(NOT client_FOUND)
    # use separate condition to avoid uninitialized variable warning
    set(client_FOUND FALSE)
  endif()
  return()
endif()
set(_client_CONFIG_INCLUDED TRUE)

# output package information
if(NOT client_FIND_QUIETLY)
  message(STATUS "Found client: 0.0.0 (${client_DIR})")
endif()

# warn when using a deprecated package
if(NOT "" STREQUAL "")
  set(_msg "Package 'client' is deprecated")
  # append custom deprecation text if available
  if(NOT "" STREQUAL "TRUE")
    set(_msg "${_msg} ()")
  endif()
  # optionally quiet the deprecation message
  if(NOT ${client_DEPRECATED_QUIET})
    message(DEPRECATION "${_msg}")
  endif()
endif()

# flag package as ament-based to distinguish it after being find_package()-ed
set(client_FOUND_AMENT_PACKAGE TRUE)

# include all config extra files
set(_extras "")
foreach(_extra ${_extras})
  include("${client_DIR}/${_extra}")
endforeach()
