/**
 * @file Framebuffer.h
 * @author Silmaen
 * @date 07/01/2024
 * Copyright © 2024 All rights reserved.
 * All modification must get authorization from the author.
 */

#pragma once

#include "../Framebuffer.h"

namespace owl::renderer::vulkan {

/**
 * @brief Class Framebuffer.
 */
class Framebuffer : public ::owl::renderer::Framebuffer {
public:
	Framebuffer(const Framebuffer &) = default;
	Framebuffer(Framebuffer &&) = default;

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
	/// The specs.
	FramebufferSpecification specs;
};

}// namespace owl::renderer::vulkan
