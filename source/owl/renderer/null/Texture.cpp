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

Texture2D::Texture2D(const math::FrameSize iSize) : m_size{iSize} {}

Texture2D::Texture2D(const uint32_t iWidth, const uint32_t iHeight) : m_size{iWidth, iHeight} {}

Texture2D::Texture2D(std::filesystem::path iPath) : m_path{std::move(iPath)} {}

Texture2D::~Texture2D() = default;

void Texture2D::bind(uint32_t) const {}

void Texture2D::setData(void *, uint32_t) {}

}// namespace owl::renderer::null
