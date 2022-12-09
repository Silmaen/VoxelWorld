#
#
#
cmake_minimum_required(VERSION 3.22)
set(CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake" "${CMAKE_CURRENT_SOURCE_DIR}/external/cmake" "${CMAKE_MODULE_PATH}")
#
# ---=== GENERALITIES ===---
#
project(Owl VERSION 0.0.1 LANGUAGES C CXX)
set(PRJPREFIX OWL)
set(PRJPREFIX_LOWER owl)
set(ENGINE_NAME OwlEngine)
#
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
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
