/**
 * @file Texture.cpp
 * @author Silmaen
 * @date 30/07/2023
 * Copyright © 2023 All rights reserved.
 * All modification must get authorization from the author.
 */
#include "owlpch.h"

#include "Texture.h"

#include <utility>

namespace owl::renderer::null {

Texture2D::Texture2D(uint32_t width_, uint32_t height_) : width{width_}, height{height_} {}

Texture2D::Texture2D(std::filesystem::path path_) : path{std::move(path_)} {}

Texture2D::~Texture2D() {}

void Texture2D::bind(uint32_t) const {}

void Texture2D::setData(void *, uint32_t) {}

}// namespace owl::renderer::null