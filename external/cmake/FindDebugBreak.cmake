#[=======================================================================[.rst:
FindDebugBreak
-------

Finds the DebugBreak library.

Imported Targets
^^^^^^^^^^^^^^^^

This module provides the following imported targets, if found:

``owl::DebugBreak``
  The Debug Break library

Result Variables
^^^^^^^^^^^^^^^^

This will define the following variables:

``DebugBreak_FOUND``
  True if the system has the Debug break library.
``DebugBreak_VERSION``
  The version of the DebugBreak library which was found.
``DebugBreak_INCLUDE_DIRS``
  Include directories needed to use DebugBreak.
``DebugBreak_LIBRARIES``
  Libraries needed to link to DebugBreak.

Cache Variables
^^^^^^^^^^^^^^^

The following cache variables may also be set:

``DebugBreak_INCLUDE_DIR``
  The directory containing ``debugbreak.h``.
``DebugBreak_LIBRARY``
  The path to the DebugBreak library.

#]=======================================================================]
if (EXISTS ${PROJECT_SOURCE_DIR}/external/debugbreak)

    set(DebugBreak_FOUND TRUE)
    set(DebugBreak_VERSION 1.0)
    set(DebugBreak_ROOT_DIR ${PROJECT_SOURCE_DIR}/external/debugbreak)
    set(DebugBreak_INCLUDE_DIRS ${DebugBreak_ROOT_DIR})
    set(DebugBreak_LIBRARY)

    add_library(owl::DebugBreak INTERFACE IMPORTED)
    target_include_directories(owl::DebugBreak SYSTEM INTERFACE ${DebugBreak_INCLUDE_DIRS})
    message(STATUS "found DebugBreak in ${DebugBreak_ROOT_DIR}")
else ()
    set(DebugBreak_FOUND FALSE)
    set(DebugBreak_NOT-FOUND TRUE)
    message(ERROR "DebugBreak not found in ${DebugBreak_ROOT_DIR}")
endif ()
