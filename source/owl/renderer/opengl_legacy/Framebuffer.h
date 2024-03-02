/**
 * @file Framebuffer.h
 * @author Silmaen
 * @date 03/08/2023
 * Copyright © 2023 All rights reserved.
 * All modification must get authorization from the author.
 */

#pragma once

#include "../Framebuffer.h"

namespace owl::renderer::opengl_legacy {
/**
 * @brief Class Framebuffer.
 */
class Framebuffer final : public renderer::Framebuffer {
public:
	Framebuffer(const Framebuffer&) = default;
	Framebuffer(Framebuffer&&) = default;
	Framebuffer& operator=(const Framebuffer&) = default;
	Framebuffer& operator=(Framebuffer&&) = default;

	/**
	 * @brief Default constructor.
	 * @param[in] iSpec The buffer specifications.
	 */
	explicit Framebuffer(FramebufferSpecification iSpec);

	/**
	 * @brief Destructor.
	 */
	~Framebuffer() override;

	/**
	 * @brief Invalidate this framebuffer.
	 */
	void invalidate();

	/**
	 * @brief Activate the shader on the GPU.
	 */
	void bind() override;

	/**
	 * @brief Deactivate the shader on the GPU.
	 */
	void unbind() override;

	/**
	 * @brief Change the size of the frame buffer.
	 * @param[in] iWidth New width.
	 * @param[in] iHeight New height.
	 */
	void resize(uint32_t iWidth, uint32_t iHeight) override;

	/**
	 * @brief Get the value of given pixel.
	 * @param[in] iAttachmentIndex Attachment's index.
	 * @param[in] iX X coordinate.
	 * @param[in] iY Y coordinate.
	 * @return Pixel value.
	 */
	int readPixel(uint32_t iAttachmentIndex, int iX, int iY) override;

	/**
	 * @brief Clear Attachment.
	 * @param[in] iAttachmentIndex Attachment's index.
	 * @param[in] iValue Clearing value.
	 */
	void clearAttachment(uint32_t iAttachmentIndex, int iValue) override;

	/**
	 * @brief Get renderer id.
	 * @param[in] iIndex The color index.
	 * @return The renderer ID.
	 */
	[[nodiscard]] uint32_t getColorAttachmentRendererId([[maybe_unused]] uint32_t iIndex) const override {
		return 0;
	}

	/**
	 * @brief Get the specs.
	 * @return The specs.
	 */
	[[nodiscard]] const FramebufferSpecification& getSpecification() const override { return m_specs; }

private:
	/// The renderer ID.
	uint32_t m_rendererId = 0;
	/// The specs.
	FramebufferSpecification m_specs;
};
} // namespace owl::renderer::opengl_legacy
