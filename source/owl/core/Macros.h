/**
 * @file Macros.h
 * @author Silmaen
 * @date 31/01/2024
 * Copyright © 2024 All rights reserved.
 * All modification must get authorization from the author.
 */

#pragma once

#if defined(OWL_PLATFORM_WINDOWS)
#ifndef _WIN64
#error 32-bit windows platform is not supported
#endif
#ifdef OWL_BUILD_SHARED
#ifdef OWL_BUILD_DLL
#define OWL_API __declspec(dllexport)
#else
#define OWL_API __declspec(dllimport)
#endif
#else
#define OWL_API
#endif
#elif defined(OWL_PLATFORM_LINUX)
#define OWL_API
#else
#define OWL_API
#if defined(__APPLE__) || defined(__MACH__)
#include <TargetConditionals.h>
#if TARGET_IPHONE_SIMULATOR == 1
#error "IOS simulator is not supported!"
#elif TARGET_OS_IPHONE == 1
#error "IOS is not supported!"
#elif TARGET_OS_MAC == 1
#error "MacOS is not supported!"
#else
#error "Unknown Apple platform!"
#endif
#elif defined(__ANDROID__)
#error "Android is not supported!"
#else
#error "Unknown compile platform so: not supported!"
#endif
#endif

// clang-format off
// Get which compiler...
#if defined(__clang__) && defined(_MSC_VER)
#define OWL_COMPILER clang_cl
#define OWL_COMPILER_CLANG_CL
#elif defined(__clang__)
#define OWL_COMPILER clang
#define OWL_COMPILER_CLANG
#elif defined(_MSC_VER)
#define OWL_COMPILER msvc
#define OWL_COMPILER_MSVC
#elif defined(__GNUC__)
#define OWL_COMPILER gcc
#define OWL_COMPILER_GCC
#else
#define OWL_COMPILER unknown
#endif

// Check supported compiler.
#if !defined(OWL_COMPILER_GCC) && !defined(OWL_COMPILER_CLANG)
#error("unsupported compiler: OWL_COMPILER")
#endif

#if defined(OWL_COMPILER_MSVC) || defined(OWL_COMPILER_CLANG_CL)
#define OWL_DO_PRAGMA(arg) __Pragma(#arg)
#elif defined(OWL_COMPILER_GCC) || defined(OWL_COMPILER_CLANG)
#define OWL_DO_PRAGMA(arg) _Pragma(#arg)
#else
#define OWL_DO_PRAGMA(arg)
#endif

#if defined(OWL_COMPILER_CLANG)
#define OWL_DIAG_POP OWL_DO_PRAGMA(clang diagnostic pop)
#define OWL_DIAG_PUSH OWL_DO_PRAGMA(clang diagnostic push)
#define OWL_DIAG_DISABLE_CLANG(diag) OWL_DO_PRAGMA(clang diagnostic ignored diag)
#if __clang_major__ > 15
#define OWL_DIAG_DISABLE_CLANG16(diag) OWL_DO_PRAGMA(clang diagnostic ignored diag)
#else
#define OWL_DIAG_DISABLE_CLANG16(diag)
#endif
#if __clang_major__ > 16
#define OWL_DIAG_DISABLE_CLANG17(diag) OWL_DO_PRAGMA(clang diagnostic ignored diag)
#else
#define OWL_DIAG_DISABLE_CLANG17(diag)
#endif
#if __clang_major__ > 17
#define OWL_DIAG_DISABLE_CLANG18(diag) OWL_DO_PRAGMA(clang diagnostic ignored diag)
#else
#define OWL_DIAG_DISABLE_CLANG18(diag)
#endif
#define OWL_DIAG_DISABLE_GCC(diag)
#define OWL_DIAG_DISABLE_CLANG_CL(diag)
#define OWL_DIAG_DISABLE_MSVC(diag)
#elif defined(OWL_COMPILER_GCC)
#define OWL_DIAG_POP OWL_DO_PRAGMA(GCC diagnostic pop)
#define OWL_DIAG_PUSH OWL_DO_PRAGMA(GCC diagnostic push)
#define OWL_DIAG_DISABLE_CLANG(diag)
#define OWL_DIAG_DISABLE_CLANG16(diag)
#define OWL_DIAG_DISABLE_CLANG17(diag)
#define OWL_DIAG_DISABLE_CLANG18(diag)
#define OWL_DIAG_DISABLE_GCC(diag) OWL_DO_PRAGMA(GCC diagnostic ignored diag)
#define OWL_DIAG_DISABLE_CLANG_CL(diag)
#define OWL_DIAG_DISABLE_MSVC(diag)
#elif defined(OWL_COMPILER_CLANG_CL)
#define OWL_DIAG_POP
#define OWL_DIAG_PUSH
#define OWL_DIAG_DISABLE_CLANG(diag)
#define OWL_DIAG_DISABLE_GCC(diag)
#define OWL_DIAG_DISABLE_CLANG_CL(diag)
#define OWL_DIAG_DISABLE_CLANG16(diag)
#define OWL_DIAG_DISABLE_CLANG17(diag)
#define OWL_DIAG_DISABLE_CLANG18(diag)
#define OWL_DIAG_DISABLE_MSVC(diag)
#elif defined(OWL_COMPILER_MSVC)
#define OWL_DIAG_POP OWL_DO_PRAGMA(warning(pop))
#define OWL_DIAG_PUSH OWL_DO_PRAGMA(warning(push))
#define OWL_DIAG_DISABLE_CLANG(diag)
#define OWL_DIAG_DISABLE_CLANG16(diag)
#define OWL_DIAG_DISABLE_CLANG17(diag)
#define OWL_DIAG_DISABLE_CLANG18(diag)
#define OWL_DIAG_DISABLE_GCC(diag)
#define OWL_DIAG_DISABLE_CLANG_CL(diag)
#define OWL_DIAG_DISABLE_MSVC(diag) OWL_DO_PRAGMA(warning(disable : diag))
#else
#define OWL_DIAG_POP
#define OWL_DIAG_PUSH
#define OWL_DIAG_DISABLE_CLANG(diag)
#define OWL_DIAG_DISABLE_CLANG16(diag)
#define OWL_DIAG_DISABLE_CLANG17(diag)
#define OWL_DIAG_DISABLE_CLANG18(diag)
#define OWL_DIAG_DISABLE_GCC(diag)
#define OWL_DIAG_DISABLE_CLANG_CL(diag)
#define OWL_DIAG_DISABLE_MSVC(diag)
#endif

// clang-format on
