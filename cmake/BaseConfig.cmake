#
#
#
include(OwlUtils)
include(Depmanager)
#
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_INSTALL_PREFIX ${PROJECT_BUILD_DIR}/Install)
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

add_library(${CMAKE_PROJECT_NAME}_Base INTERFACE)
#
# ---=== Supported OS ===---
#
set(${PRJPREFIX}_GNU_MINIMAL 10)
set(${PRJPREFIX}_CLANG_MINIMAL 13)
set(${PRJPREFIX}_GNU_MINIMAL_CPP23 11)
set(${PRJPREFIX}_CLANG_MINIMAL_CPP23 14)

if (CMAKE_SYSTEM_NAME MATCHES "Windows")
    set(EXE_EXT ".exe")
    set(LIB_EXT ".dll")
    message(STATUS "Detected Operating System '${CMAKE_SYSTEM_NAME}'")
    target_compile_definitions(${CMAKE_PROJECT_NAME}_Base INTERFACE OWL_PLATFORM_WINDOWS)
elseif (CMAKE_SYSTEM_NAME MATCHES "Linux")
    set(EXE_EXT "")
    set(LIB_EXT ".so")
    message(STATUS "Detected Operating System '${CMAKE_SYSTEM_NAME}'")
    target_compile_definitions(${CMAKE_PROJECT_NAME}_Base INTERFACE OWL_PLATFORM_LINUX)
else ()
    message(FATAL_ERROR "Unsupported Operating System '${CMAKE_SYSTEM_NAME}'")
endif ()
set(CMAKE_INSTALL_RPATH $ORIGIN)

#
# ---=== Supported Compiler ===----
#
if (${CMAKE_CXX_COMPILER_ID} MATCHES "GNU")
    if (${CMAKE_CXX_COMPILER_VERSION} VERSION_LESS ${${PRJPREFIX}_GNU_MINIMAL})
        message(FATAL_ERROR "${CMAKE_CXX_COMPILER_ID} compiler version too old: ${CMAKE_CXX_COMPILER_VERSION}, need ${${PRJPREFIX}_GNU_MINIMAL}")
    endif ()
    if (${CMAKE_CXX_COMPILER_VERSION} VERSION_LESS ${${PRJPREFIX}_GNU_MINIMAL_CPP23})
        message(WARNING "${CMAKE_CXX_COMPILER_ID} compiler version ${CMAKE_CXX_COMPILER_VERSION} is too old for cpp 23 downgrading")
        set(CMAKE_CXX_STANDARD 20)
    endif ()
    message(STATUS "Using GNU compiler")
    target_compile_options(${CMAKE_PROJECT_NAME}_Base INTERFACE
            -Werror -Wall -Wextra -pedantic
            -Wdeprecated
            -Wdeprecated-declarations
            -Wcast-align
            -Wcast-qual
            -Wno-mismatched-new-delete
    )
    target_compile_definitions(${CMAKE_PROJECT_NAME}_Base INTERFACE
            STBI_NO_SIMD)
elseif (${CMAKE_CXX_COMPILER_ID} MATCHES "Clang")
    if (${CMAKE_CXX_COMPILER_VERSION} VERSION_LESS ${${PRJPREFIX}_CLANG_MINIMAL})
        message(FATAL_ERROR "${CMAKE_CXX_COMPILER_ID} compiler version too old: ${CMAKE_CXX_COMPILER_VERSION}, need ${${PRJPREFIX}_CLANG_MINIMAL}")
    endif ()
    if (${CMAKE_CXX_COMPILER_VERSION} VERSION_LESS ${${PRJPREFIX}_CLANG_MINIMAL_CPP23})
        message(WARNING "${CMAKE_CXX_COMPILER_ID} compiler version ${CMAKE_CXX_COMPILER_VERSION} is too old for cpp 23 downgrading")
        set(CMAKE_CXX_STANDARD 20)
    endif ()
    message(STATUS "Using Clang compiler")
    target_compile_options(${CMAKE_PROJECT_NAME}_Base INTERFACE
            -Werror -Weverything -pedantic
            -Wno-c++98-compat
            -Wno-c++98-compat-pedantic
            -Wno-c++20-compat
            -Wno-padded
            -Wno-used-but-marked-unused
            -Wno-exit-time-destructors
            -Wno-global-constructors
            -Wno-reserved-macro-identifier
            -Wno-unused-macros
            -Wno-ctad-maybe-unsupported
            -Wno-format-security)
    if (${CMAKE_CXX_COMPILER_VERSION} VERSION_GREATER_EQUAL 16)
        target_compile_options(${CMAKE_PROJECT_NAME}_Base INTERFACE
                -Wno-unsafe-buffer-usage
                -Wno-cast-function-type-strict
        )
    endif ()
else ()
    message(FATAL_ERROR "Unsupported compiler: ${CMAKE_CXX_COMPILER_ID}")
endif ()

add_custom_target(${CMAKE_PROJECT_NAME}_SuperBase)
if (OWL_PLATFORM_WINDOWS)
    if (MINGW)
        cmake_path(GET CMAKE_CXX_COMPILER PARENT_PATH COMPILER_PATH)
        message(STATUS "MinGW environment detected: add dependence to dlls from ${COMPILER_PATH}")
        set(REQUIRED_LIBS libgcc_s_seh-1.dll libstdc++-6.dll libwinpthread-1.dll)
        foreach (lib IN ITEMS ${REQUIRED_LIBS})
            if (NOT EXISTS ${COMPILER_PATH}/${lib})
                message(WARNING "Required Dll not found: ${COMPILER_PATH}/${lib}")
            else ()
                message(STATUS "Adding Dll: ${COMPILER_PATH}/${lib}")
                add_custom_command(TARGET ${CMAKE_PROJECT_NAME}_SuperBase
                        DEPENDS ${COMPILER_PATH}/${lib}
                        COMMAND ${CMAKE_COMMAND} -E copy_if_different ${COMPILER_PATH}/${lib} ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}
                        COMMENT "Copying ${lib} into ${${CMAKE_RUNTIME_OUTPUT_DIRECTORY}}")
            endif ()
        endforeach ()
    endif ()
endif ()
add_dependencies(${CMAKE_PROJECT_NAME}_Base ${CMAKE_PROJECT_NAME}_SuperBase)

target_compile_definitions(${CMAKE_PROJECT_NAME}_Base INTERFACE "${PRJPREFIX}_MAJOR=\"${CMAKE_PROJECT_VERSION_MAJOR}\"")
target_compile_definitions(${CMAKE_PROJECT_NAME}_Base INTERFACE "${PRJPREFIX}_MINOR=\"${CMAKE_PROJECT_VERSION_MINOR}\"")
target_compile_definitions(${CMAKE_PROJECT_NAME}_Base INTERFACE "${PRJPREFIX}_PATCH=\"${CMAKE_PROJECT_VERSION_PATCH}\"")
target_compile_definitions(${CMAKE_PROJECT_NAME}_Base INTERFACE ${PRJPREFIX}_AUTHOR="Silmaen")

if (CMAKE_BUILD_TYPE MATCHES "Debug")
    target_compile_definitions(${CMAKE_PROJECT_NAME}_Base INTERFACE ${PRJPREFIX}_DEBUG)
endif ()

if (${PRJPREFIX}_COVERAGE)
    include(cmake/CoverageConfig.cmake)
else ()
    if (CMAKE_BUILD_TYPE MATCHES "Debug")
        message(STATUS "No Coverage configured. Set ${PRJPREFIX}_COVERAGE=on to enable it.")
    endif ()
endif ()

include(cmake/DocumentationConfig.cmake)
