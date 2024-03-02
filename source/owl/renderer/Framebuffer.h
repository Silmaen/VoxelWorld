/**
 * @file Framebuffer.h
 * @author Silmaen
 * @date 21/12/2022
 * Copyright © 2022 All rights reserved.
 * All modification must get authorization from the author.
 */

#pragma once

#include "core/Core.h"

namespace owl::renderer {
/// Format for the texture frame buffer.
enum class FramebufferTextureFormat {
	None = 0,
	/// Color.
	Rgba8,
	RedInteger,
	/// Depth/stencil.
	Depth24Stencil8,
	/// Defaults.
	Depth = Depth24Stencil8
};

/**
 * @brief Specification for the texture Framebuffer.
 */
struct FramebufferTextureSpecification {
	FramebufferTextureSpecification() = default;

	// NOLINTBEGIN(google-explicit-constructor)
	FramebufferTextureSpecification(const FramebufferTextureFormat& iFormat)
		: textureFormat(iFormat) {}

	// NOLINTEND(google-explicit-constructor)

	FramebufferTextureFormat textureFormat = FramebufferTextureFormat::None;
	// TODO: filtering/wrap
};

/**
 * @brief Specification for the attachment Framebuffer.
 */
struct FramebufferAttachmentSpecification {
	FramebufferAttachmentSpecification() = default;

	FramebufferAttachmentSpecification(const std::initializer_list<FramebufferTextureSpecification> iAttachments)
		: attachments(iAttachments) {}

	std::vector<FramebufferTextureSpecification> attachments;
};

/**
 * @brief Framebuffer specification.
 */
struct FramebufferSpecification {
	/// width.
	uint32_t width = 0;
	/// height.
	uint32_t height = 0;
	/// Attachment Specs.
	FramebufferAttachmentSpecification attachments;
	/// Amount of sample.
	uint32_t samples = 1;
	/// If chained target must be swap.
	bool swapChainTarget = false;
};

/**
 * @brief Class Framebuffer.
 */
class OWL_API Framebuffer {
public:
	Framebuffer() = default;
	Framebuffer(const Framebuffer&) = default;
	Framebuffer(Framebuffer&&) = default;
	Framebuffer& operator=(const Framebuffer&) = default;
	Framebuffer& operator=(Framebuffer&&) = default;

	/**
	 * @brief Destructor.
	 */
	virtual ~Framebuffer();

	/**
	 * @brief Activate the shader on the GPU.
	 */
	virtual void bind() = 0;

	/**
	 * @brief Deactivate the shader on the GPU.
	 */
	virtual void unbind() = 0;

	/**
	 * @brief Change the size of the frame buffer.
	 * @param[in] iWidth New width.
	 * @param[in] iHeight New height.
	 */
	virtual void resize(uint32_t iWidth, uint32_t iHeight) = 0;

	/**
	 * Get the Pixel information.
	 * @param[in] iAttachmentIndex Index in the attachment.
	 * @param[in] iX Horizontal coordinate.
	 * @param[in] iY Vertical coordinate.
	 * @return Pixel index.
	 */
	virtual int readPixel(uint32_t iAttachmentIndex, int iX, int iY) = 0;

	/**
	 * @brief Reset an attachment with the given value.
	 * @param[in] iAttachmentIndex Index of the attachment.
	 * @param[in] iValue The new value to affect.
	 */
	virtual void clearAttachment(uint32_t iAttachmentIndex, int iValue) = 0;

	/**
	 * @brief Get renderer id.
	 * @param[in] iIndex The color Index.
	 * @return The renderer ID.
	 */
	[[nodiscard]] virtual uint32_t getColorAttachmentRendererId(uint32_t iIndex) const = 0;

	/**
	 * @brief Get the specs.
	 * @return The specs.
	 */
	[[nodiscard]] virtual const FramebufferSpecification& getSpecification() const = 0;

	/**
	 * @brief Create the frame buffer.
	 * @param[in] iSpec Specifications.
	 * @return The Frame buffer.
	 */
	static shared<Framebuffer> create(const FramebufferSpecification& iSpec);

private:
};
} // namespace owl::renderer
