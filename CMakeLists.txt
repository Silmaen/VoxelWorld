#
#
#
cmake_minimum_required(VERSION 3.22)
set(CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake" "${CMAKE_CURRENT_SOURCE_DIR}/external/cmake" "${CMAKE_MODULE_PATH}")
#
# ---=== GENERALITIES ===---
#
project(Owl VERSION 0.0.1 LANGUAGES CXX)
set(PRJPREFIX OWL)
set(PRJPREFIX_LOWER owl)
#
# ---=== Base Configs ===---
#
include(BaseConfig)
#
# ---=== Dependencies ===---
#
add_subdirectory(external)

#
# ---=== The Program ===---
#
add_subdirectory(source)

#
# ---=== TESTING PROCEDURES ===---
#
option(${PRJPREFIX}_TESTING "Enable the unit tests" ON)
if (${PRJPREFIX}_TESTING)
    enable_testing()
    add_subdirectory(test)
endif ()
