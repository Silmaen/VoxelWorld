#[=======================================================================[.rst:
FindEntt
-------

Finds the Entt library.

Imported Targets
^^^^^^^^^^^^^^^^

This module provides the following imported targets, if found:

``owl::Entt``
  The Entt library

Result Variables
^^^^^^^^^^^^^^^^

This will define the following variables:

``Entt_FOUND``
  True if the system has the Entt library.
``Entt_VERSION``
  The version of the Entt library which was found.
``Entt_INCLUDE_DIRS``
  Include directories needed to use Entt.
``Entt_LIBRARIES``
  Libraries needed to link to Entt.

Cache Variables
^^^^^^^^^^^^^^^

The following cache variables may also be set:

``Entt_INCLUDE_DIR``
  The directory containing ``Entt.h``.
``Entt_LIBRARY``
  The path to the Entt library.

#]=======================================================================]

if (EXISTS ${PROJECT_SOURCE_DIR}/external/Entt)

    set(Entt_FOUND TRUE)
    set(Entt_VERSION 3.11.1)
    set(Entt_ROOT_DIR ${PROJECT_SOURCE_DIR}/external/Entt)
    set(Entt_INCLUDE_DIRS ${Entt_ROOT_DIR}/include)
    set(Entt_LIBRARY)

    add_library(owl::Entt INTERFACE IMPORTED)
    target_include_directories(owl::Entt SYSTEM INTERFACE ${Entt_INCLUDE_DIRS})
else ()

    set(DebugBreak_FOUND FALSE)
    set(DebugBreak_NOT-FOUND TRUE)
    message(ERROR "Entt not found in ${Entt_ROOT_DIR}")
endif ()
