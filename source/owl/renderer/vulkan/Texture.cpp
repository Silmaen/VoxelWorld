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

Texture2D::Texture2D(const math::FrameSize &iSize, bool) : m_size{iSize} {}

Texture2D::Texture2D(const uint32_t iWidth, const uint32_t iHeight, bool) : m_size{iWidth, iHeight} {}

Texture2D::Texture2D(std::filesystem::path iPath) : m_path{std::move(iPath)} {}

Texture2D::~Texture2D() = default;

void Texture2D::bind(uint32_t) const {}

void Texture2D::setData(void *, uint32_t) {}

}// namespace owl::renderer::vulkan
