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
class Framebuffer final : public ::owl::renderer::Framebuffer {
public:
	Framebuffer(const Framebuffer &) = default;
	Framebuffer(Framebuffer &&) = default;
	Framebuffer &operator=(const Framebuffer &) = default;
	Framebuffer &operator=(Framebuffer &&) = default;

	/**
	 * @brief Default constructor.
	 * @param spec The buffer specifications.
	 */
	explicit Framebuffer(FramebufferSpecification spec);

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
	 * @param width New width.
	 * @param height New height.
	 */
	void resize(uint32_t width, uint32_t height) override;

	int readPixel(uint32_t attachmentIndex, int x, int y) override;

	void clearAttachment(uint32_t attachmentIndex, int value) override;

	/**
	 * @brief Get renderer id.
	 * @param index The color index.
	 * @return The renderer ID.
	 */
	[[nodiscard]] uint32_t getColorAttachmentRendererID([[maybe_unused]] uint32_t index) const override {
		return 0;
	}

	/**
	 * @brief Get the specs.
	 * @return The specs.
	 */
	[[nodiscard]] const FramebufferSpecification &getSpecification() const override { return specs; }

private:
	/// The renderer ID.
	uint32_t rendererID = 0;
	/// The specs.
	FramebufferSpecification specs;
};

}// namespace owl::renderer::opengl_legacy
