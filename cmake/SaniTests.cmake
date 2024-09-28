# ----------------------------------------------------------------------------------------------------------------------
# Clang tidy options
if (${PRJPREFIX}_COMPILER_CLANG)
    option(${PRJPREFIX}_ENABLE_CLANG_TIDY "Enable clang-tidy" OFF)
    option(${PRJPREFIX}_ENABLE_ADDRESS_SANITIZER "Enable address sanitizer" OFF)
    option(${PRJPREFIX}_ENABLE_THREAD_SANITIZER "Enable thread sanitizer" OFF)
    option(${PRJPREFIX}_ENABLE_UNDEFINED_BEHAVIOR_SANITIZER "Enable undefined behavior sanitizer" OFF)
    option(${PRJPREFIX}_ENABLE_LEAK_SANITIZER "Enable memory leaks sanitizer" OFF)
    option(${PRJPREFIX}_ENABLE_MEMORY_SANITIZER "Enable memory sanitizer" OFF)
endif ()
# ----------------------------------------------------------------------------------------------------------------------


# ----------------------------------------------------------------------------------------------------------------------
# Clang-tidy
if (${PRJPREFIX}_ENABLE_CLANG_TIDY)
    math(EXPR ${PRJPREFIX}_SANITIZER_COUNT "${${PRJPREFIX}_SANITIZER_COUNT} + 1")
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

# ----------------------------------------------------------------------------------------------------------------------
# Clang sanitizer - address
# ----------------------------------------------------------------------------------------------------------------------
if (${PRJPREFIX}_ENABLE_ADDRESS_SANITIZER)
    math(EXPR ${PRJPREFIX}_SANITIZER_COUNT "${${PRJPREFIX}_SANITIZER_COUNT} + 1")
    target_compile_options(${CMAKE_PROJECT_NAME}_Base INTERFACE -fsanitize=address -fsanitize-recover=address -O0 -g3 -fno-omit-frame-pointer -fno-optimize-sibling-calls)
    target_link_options(${CMAKE_PROJECT_NAME}_Base INTERFACE -fsanitize=address)
    target_compile_definitions(${CMAKE_PROJECT_NAME}_Base INTERFACE ${PRJPREFIX}_SANITIZER)
    target_compile_definitions(${CMAKE_PROJECT_NAME}_Base INTERFACE ${PRJPREFIX}_ADDRESS_SANITIZER)
endif ()

# ----------------------------------------------------------------------------------------------------------------------
# Clang sanitizer - thread Sanitizer
# ----------------------------------------------------------------------------------------------------------------------
if (${PRJPREFIX}_ENABLE_THREAD_SANITIZER)
    math(EXPR ${PRJPREFIX}_SANITIZER_COUNT "${${PRJPREFIX}_SANITIZER_COUNT} + 1")
    target_compile_options(${CMAKE_PROJECT_NAME}_Base INTERFACE -fsanitize=thread -O0 -g3)
    target_link_options(${CMAKE_PROJECT_NAME}_Base INTERFACE -fsanitize=thread)
    target_compile_definitions(${CMAKE_PROJECT_NAME}_Base INTERFACE ${PRJPREFIX}_SANITIZER)
    target_compile_definitions(${CMAKE_PROJECT_NAME}_Base INTERFACE ${PRJPREFIX}_THREAD_SANITIZER)
endif ()

# ----------------------------------------------------------------------------------------------------------------------
# Clang sanitizer - Undefined behavior
# ----------------------------------------------------------------------------------------------------------------------
if (${PRJPREFIX}_ENABLE_UNDEFINED_BEHAVIOR_SANITIZER)
    math(EXPR ${PRJPREFIX}_SANITIZER_COUNT "${${PRJPREFIX}_SANITIZER_COUNT} + 1")
    target_compile_options(${CMAKE_PROJECT_NAME}_Base INTERFACE -fsanitize=undefined -fsanitize-recover=undefined -O0 -g3 -fno-omit-frame-pointer)
    target_link_options(${CMAKE_PROJECT_NAME}_Base INTERFACE -fsanitize=undefined)
    target_compile_definitions(${CMAKE_PROJECT_NAME}_Base INTERFACE ${PRJPREFIX}_SANITIZER)
    target_compile_definitions(${CMAKE_PROJECT_NAME}_Base INTERFACE ${PRJPREFIX}_UNDEFINED_BEHAVIOR_SANITIZER)
endif ()

# ----------------------------------------------------------------------------------------------------------------------
# Clang sanitizer - leak Sanitizer
# ----------------------------------------------------------------------------------------------------------------------
if (${PRJPREFIX}_ENABLE_LEAK_SANITIZER)
    math(EXPR ${PRJPREFIX}_SANITIZER_COUNT "${${PRJPREFIX}_SANITIZER_COUNT} + 1")
    target_compile_options(${CMAKE_PROJECT_NAME}_Base INTERFACE -fsanitize=leak -fsanitize-recover=leak -O0 -g3 -fno-omit-frame-pointer)
    target_link_options(${CMAKE_PROJECT_NAME}_Base INTERFACE -fsanitize=leak)
    target_compile_definitions(${CMAKE_PROJECT_NAME}_Base INTERFACE ${PRJPREFIX}_SANITIZER)
    target_compile_definitions(${CMAKE_PROJECT_NAME}_Base INTERFACE ${PRJPREFIX}_LEAK_SANITIZER)
endif ()

# ----------------------------------------------------------------------------------------------------------------------
# Clang sanitizer - Memory Sanitizer
# ----------------------------------------------------------------------------------------------------------------------
if (${PRJPREFIX}_ENABLE_MEMORY_SANITIZER)
    math(EXPR ${PRJPREFIX}_SANITIZER_COUNT "${${PRJPREFIX}_SANITIZER_COUNT} + 1")
    target_compile_options(${CMAKE_PROJECT_NAME}_Base INTERFACE -fsanitize=memory -fsanitize-recover=memory -O0 -g3 -fno-omit-frame-pointer -fno-optimize-sibling-calls)
    target_link_options(${CMAKE_PROJECT_NAME}_Base INTERFACE -fsanitize=memory)
    target_compile_definitions(${CMAKE_PROJECT_NAME}_Base INTERFACE ${PRJPREFIX}_SANITIZER)
    target_compile_definitions(${CMAKE_PROJECT_NAME}_Base INTERFACE ${PRJPREFIX}_MEMORY_SANITIZER)
endif ()

# ----------------------------------------------------------------------------------------------------------------------
# Compatibility check
if (${PRJPREFIX}_SANITIZER_COUNT GREATER 1)
    message(FATAL_ERROR "You can only use code coverage/inspection tools one by one.")
endif ()

if (${PRJPREFIX}_ENABLE_COVERAGE AND ${PRJPREFIX}_SANITIZER_COUNT GREATER 0)
    message(FATAL_ERROR "You can only use code coverage/inspection tools one by one.")
endif ()
# ----------------------------------------------------------------------------------------------------------------------
