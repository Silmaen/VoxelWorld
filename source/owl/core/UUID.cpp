/**
 * @file UUID.cpp
 * @author Silmaen
 * @date 14/01/2023
 * Copyright © 2023 All rights reserved.
 * All modification must get authorization from the author.
 */
#include "owlpch.h"

#include "UUID.h"

namespace owl::core {

static std::random_device randomDevice;
static std::mt19937_64 engine(randomDevice());
static std::uniform_int_distribution<uint64_t> uniformDistribution;

UUID::UUID() :uuid{uniformDistribution(engine)}{}

}// namespace core
