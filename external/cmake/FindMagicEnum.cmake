#[=======================================================================[.rst:
FindMagicEnum
-------

Finds the MagicEnum library.

Imported Targets
^^^^^^^^^^^^^^^^

This module provides the following imported targets, if found:

``owl::MagicEnum``
  The MagicEnum library

Result Variables
^^^^^^^^^^^^^^^^

This will define the following variables:

``MagicEnum_FOUND``
  True if the system has the MagicEnum library.
``MagicEnum_VERSION``
  The version of the MagicEnum library which was found.
``MagicEnum_INCLUDE_DIRS``
  Include directories needed to use MagicEnum.
``MagicEnum_LIBRARIES``
  Libraries needed to link to MagicEnum.

Cache Variables
^^^^^^^^^^^^^^^

The following cache variables may also be set:

``MagicEnum_INCLUDE_DIR``
  The directory containing ``MagicEnum.h``.
``MagicEnum_LIBRARY``
  The path to the MagicEnum library.

#]=======================================================================]

if (EXISTS ${PROJECT_SOURCE_DIR}/external/magic_enum)

    set(MagicEnum_FOUND TRUE)
    set(MagicEnum_ROOT_DIR ${PROJECT_SOURCE_DIR}/external/magic_enum)
    set(MagicEnum_INCLUDE_DIRS ${MagicEnum_ROOT_DIR}/include)
    set(MagicEnum_LIBRARY)

    add_library(owl::MagicEnum INTERFACE IMPORTED)
    target_include_directories(owl::MagicEnum SYSTEM INTERFACE ${MagicEnum_INCLUDE_DIRS})
else ()

    set(DebugBreak_FOUND FALSE)
    set(DebugBreak_NOT-FOUND TRUE)
    message(ERROR "MagicEnum not found in ${MagicEnum_ROOT_DIR}")
endif ()
