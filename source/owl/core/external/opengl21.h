/**
 * @file opengl21.h
 * @author Silmaen
 * @date 22/11/2023
 * Copyright (c) 2023 All rights reserved.
 * All modification must get authorization from the author.
 */
#pragma once

#ifdef OLD_GLAD
#include "glad21/glad.h"
#else
#if defined(__clang__)
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wreserved-identifier"
#endif
#include "glad21/gl.h"
#if defined(__clang__)
#pragma clang diagnostic pop
#endif
#endif
