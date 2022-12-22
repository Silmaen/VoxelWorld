/**
 * @file owlpch.h
 * @author Silmaen
 * @date 06/12/2022
 * Copyright © 2022 All rights reserved.
 * All modification must get authorization from the author.
 */

#pragma once

#ifdef __clang__
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wundefined-func-template"
#endif
#include <algorithm>
#include <filesystem>
#include <fstream>
#include <functional>
#include <memory>
#include <streambuf>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <utility>
#include <vector>

#include "core/Log.h"
#include "debug/Profiler.h"

#ifdef __clang__
#pragma clang diagnostic pop
#endif
