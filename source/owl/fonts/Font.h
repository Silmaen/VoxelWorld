/**
 * @file Font.h
 * @author Silmaen
 * @date 10/17/24
 * Copyright © 2024 All rights reserved.
 * All modification must get authorization from the author.
 */

#pragma once

#include "core/Core.h"
#include "renderer/Texture.h"

#include "math/box.h"

namespace owl::fonts {

struct MsdfData;

/**
 * @brief Class Font.
 */
class OWL_API Font {
public:
	/**
	 * @brief Default constructor.
	 */
	explicit Font(const std::filesystem::path& iPath);
	/**
	 * @brief Default destructor.
	 */
	virtual ~Font();
	/**
	 * @brief Default copy constructor.
	 */
	Font(const Font&) = default;
	/**
	 * @brief Default move constructor.
	 */
	Font(Font&&) = default;
	/**
	 * @brief Default copy affectation operator.
	 */
	auto operator=(const Font&) -> Font& = default;
	/**
	 * @brief Default move affectation operator.
	 */
	auto operator=(Font&&) -> Font& = default;

	/**
	 * @brief Get the atlas texture corresponding to the Font.
	 * @return The atlas texture.
	 */
	[[nodiscard]] auto getAtlasTexture() const -> shared<renderer::Texture2D> { return m_atlasTexture; }

	struct GlyphMetrics {
		math::box2f quad;
		math::box2f uv;
	};
	[[nodiscard]] auto getGlyphBox(const char& iChar) const -> GlyphMetrics;

	[[nodiscard]] auto getScaledLineHeight() const -> float;
	[[nodiscard]] auto getAdvance(const char& iChar, const char& iNextChar) const -> float;

private:
	/// pointer to the texture.
	shared<renderer::Texture2D> m_atlasTexture;
	/// The internal atlas data.
	shared<MsdfData> m_data;
};

}// namespace owl::fonts
