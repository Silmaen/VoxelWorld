/**
 * @file Texture.cpp
 * @author Silmaen
 * @date 07/01/2024
 * Copyright © 2024 All rights reserved.
 * All modification must get authorization from the author.
 */
#include "owlpch.h"

#include "Texture.h"

namespace owl::renderer::vulkan {

Texture2D::Texture2D(const math::FrameSize &size_, bool) : size{size_} {}

Texture2D::Texture2D(uint32_t width_, uint32_t height_, bool) : size{width_, height_} {}

Texture2D::Texture2D(std::filesystem::path path_) : path{std::move(path_)} {}

Texture2D::~Texture2D() = default;

void Texture2D::bind(uint32_t) const {}

void Texture2D::setData(void *, uint32_t) {}

}// namespace owl::renderer::vulkan
