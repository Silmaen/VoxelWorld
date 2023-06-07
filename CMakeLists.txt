#
#
#
cmake_minimum_required(VERSION 3.24)
set(CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake" "${CMAKE_CURRENT_SOURCE_DIR}/external/cmake" "${CMAKE_MODULE_PATH}")
set(PRJPREFIX OWL)
set(PRJPREFIX_LOWER owl)
set(ENGINE_NAME OwlEngine)
project(Owl VERSION 0.1.0 LANGUAGES C CXX)
#
# ---=== Options ===---
#
option(${PRJPREFIX}_BUILD_SHARED "If the Library must be build as shared library" OFF)
option(${PRJPREFIX}_TESTING "Enable the unit tests" ON)
option(${PRJPREFIX}_DOC_ONLY "To only generate documentation" OFF)
option(${PRJPREFIX}_BUILD_NEST "If wanted to generate Owl Nest" ON)
option(${PRJPREFIX}_BUILD_SANDBOX "If wanted to generate Sandbox" ON)
#
# ---=== Base Configs ===---
#
include(BaseConfig)

if (NOT ${PRJPREFIX}_DOC_ONLY)
    #
    # ---=== The Program ===---
    #
    add_subdirectory(source)

    #
    # ---=== TESTING PROCEDURES ===---
    #
    if (${PRJPREFIX}_TESTING)
        enable_testing()
        add_subdirectory(test)
    endif ()
endif ()