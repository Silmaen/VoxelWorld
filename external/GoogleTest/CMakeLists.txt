# Note: CMake support is community-based. The maintainers do not use CMake
# internally.

cmake_minimum_required(VERSION 3.10.0)

if (POLICY CMP0048)
    cmake_policy(SET CMP0048 NEW)
endif (POLICY CMP0048)

project(googletest-distribution)
set(GOOGLETEST_VERSION 1.10.0)

include(CMakeDependentOption)
include(GNUInstallDirs)

if (CMAKE_CXX_COMPILER_ID MATCHES "Clang")
    if (CMAKE_CXX_COMPILER_VERSION GREATER_EQUAL 11)
        add_definitions(
                -Wno-suggest-override
                -Wno-suggest-destructor-override
                -Wno-disabled-macro-expansion
        )
    endif ()
    if (CMAKE_CXX_COMPILER_VERSION GREATER_EQUAL 14)
        add_definitions(
                -Wno-reserved-identifier
        )
    endif ()
    add_definitions(
            -Wno-nonportable-system-include-path
            -Wno-language-extension-token
            -Wno-undef
            -Wno-global-constructors
            -Wno-exit-time-destructors
            -Wno-vla
            -Wno-old-style-cast
            -Wno-covered-switch-default
            -Wno-weak-vtables
            -Wno-switch-enum
            -Wno-used-but-marked-unused
            -Wno-missing-noreturn
            -Wno-vla-extension
            -Wno-shift-sign-overflow
            -Wno-zero-as-null-pointer-constant
            -Wno-unused-member-function
    )
elseif (CMAKE_CXX_COMPILER_ID MATCHES "GNU")
    add_definitions(
            -Wno-vla
    )
    if (CMAKE_CXX_COMPILER_VERSION GREATER "9.0")
        add_definitions(
                -Wno-analyzer-null-dereference
        )
    endif ()
    if (CMAKE_CXX_COMPILER_VERSION GREATER "12.0")
        add_definitions(
                -Wno-analyzer-null-dereference
                -Wno-maybe-uninitialized
                -Wno-restrict
        )
    endif ()
endif ()

add_subdirectory(googletest)
