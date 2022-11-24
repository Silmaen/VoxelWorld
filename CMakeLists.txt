#
#
#
cmake_minimum_required(VERSION 3.22)
#
# ---=== GENERALITIES ===---
#
project(VoxelWorld VERSION 0.0.1 LANGUAGES CXX)
set(PRJPREFIX EVL)
set(PRJPREFIX_LOWER evl)
set(${PRJPREFIX}_ROOT_DIR "${PROJECT_SOURCE_DIR}")
set(CMAKE_INSTALL_PREFIX ${PROJECT_BUILD_DIR}/Install)
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_VERBOSE_MAKEFILE off)
#
# ---=== Base Configs ===---
#
include(cmake/BaseConfig.cmake)
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
enable_testing()
add_subdirectory(test)
