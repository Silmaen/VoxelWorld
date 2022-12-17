#[=======================================================================[.rst:
FindStbImage
-------

Finds the StbImage library.

Imported Targets
^^^^^^^^^^^^^^^^

This module provides the following imported targets, if found:

``owl::StbImage``
  The StbImage library

Result Variables
^^^^^^^^^^^^^^^^

This will define the following variables:

``StbImage_FOUND``
  True if the system has the StbImage library.
``StbImage_VERSION``
  The version of the StbImage library which was found.
``StbImage_INCLUDE_DIRS``
  Include directories needed to use StbImage.
``StbImage_LIBRARIES``
  Libraries needed to link to StbImage.

Cache Variables
^^^^^^^^^^^^^^^

The following cache variables may also be set:

``StbImage_INCLUDE_DIR``
  The directory containing ``StbImage.h``.
``StbImage_LIBRARY``
  The path to the StbImage library.

#]=======================================================================]

if (EXISTS ${PROJECT_SOURCE_DIR}/external/stb)

    set(StbImage_FOUND TRUE)
    set(StbImage_ROOT_DIR ${PROJECT_SOURCE_DIR}/external/stb)
    set(StbImage_INCLUDE_DIRS ${StbImage_ROOT_DIR})
    set(StbImage_LIBRARY)

    add_library(stbImage STATIC ${StbImage_ROOT_DIR}/stb_image.h ${StbImage_ROOT_DIR}/stb_image.cpp)

    add_library(owl::StbImage INTERFACE IMPORTED)
    target_link_libraries(owl::StbImage INTERFACE stbImage)
    target_include_directories(owl::StbImage SYSTEM INTERFACE ${StbImage_INCLUDE_DIRS})
else ()

    set(DebugBreak_FOUND FALSE)
    set(DebugBreak_NOT-FOUND TRUE)
    message(ERROR "StbImage not found in ${StbImage_ROOT_DIR}")
endif ()