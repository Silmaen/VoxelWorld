/**
 * @file owlpch.h
 * @author Silmaen
 * @date 06/12/2022
 * Copyright © 2022 All rights reserved.
 * All modification must get authorization from the author.
 */

#pragma once
#include "core/Macros.h"
OWL_DIAG_PUSH
OWL_DIAG_DISABLE_CLANG("-Wundefined-func-template")

#include <algorithm>
#include <array>
#include <filesystem>
#include <fstream>
#include <functional>
#include <list>
#include <memory>
#include <random>
#include <set>
#include <streambuf>
#include <string>
#include <string_view>
#include <unordered_map>
#include <unordered_set>
#include <utility>
#include <vector>

#include "core/Log.h"
#include "debug/Profiler.h"

// third party
#include <magic_enum.hpp>

OWL_DIAG_POP
