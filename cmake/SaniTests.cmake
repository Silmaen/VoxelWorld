# ----------------------------------------------------------------------------------------------------------------------
# Clang tidy options
if (${PRJPREFIX}_COMPILER_CLANG)
    option(${PRJPREFIX}_ENABLE_CLANG_TIDY "Enable clang-tidy" OFF)
endif ()
# ----------------------------------------------------------------------------------------------------------------------

# ----------------------------------------------------------------------------------------------------------------------
# Compatibility check
if (${PRJPREFIX}_SANITIZER_COUNT GREATER 0 AND ${PRJPREFIX}_ENABLE_CLANG_TIDY)
    message(FATAL_ERROR "You can only use code coverage/inspection tools one by one.")
endif ()

if (${PRJPREFIX}_ENABLE_COVERAGE AND ${PRJPREFIX}_ENABLE_CLANG_TIDY)
    message(FATAL_ERROR "You can only use code coverage/inspection tools one by one.")
endif ()
# ----------------------------------------------------------------------------------------------------------------------

# ----------------------------------------------------------------------------------------------------------------------
# Clang-tidy
if (${PRJPREFIX}_ENABLE_CLANG_TIDY)
    find_program(CLANG_TIDY_EXECUTABLE clang-tidy)
    if (CLANG_TIDY_EXECUTABLE)
        message(STATUS "Found clang-tidy: ${CLANG_TIDY_EXECUTABLE}")
        set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
        set(CMAKE_CXX_SCAN_FOR_MODULES ON)
        set(CMAKE_CXX_CLANG_TIDY "${CLANG_TIDY_EXECUTABLE}" -p "${CMAKE_BINARY_DIR}")

        target_compile_definitions(${CMAKE_PROJECT_NAME}_Base INTERFACE ${PRJPREFIX}_USE_CLANG_TIDY)
        message(STATUS "CLANG-TIDY activated.")
    else ()
        set(${PRJPREFIX}_ENABLE_CLANG_TIDY OFF CACHE BOOL "No Clang tidy found" FORCE)
        message(WARNING "No clang-tidy found on the system, deactivating it.")
    endif ()
endif ()
# ----------------------------------------------------------------------------------------------------------------------
