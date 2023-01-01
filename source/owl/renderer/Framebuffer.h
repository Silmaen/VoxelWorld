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

enum class FramebufferTextureFormat {
	None = 0,
	// Color
	RGBA8,
	RED_INTEGER,
	// Depth/stencil
	DEPTH24STENCIL8,
	// Defaults
	Depth = DEPTH24STENCIL8
};

struct FramebufferTextureSpecification {
	FramebufferTextureSpecification() = default;
	FramebufferTextureSpecification(FramebufferTextureFormat format)
		: textureFormat(format) {}
	FramebufferTextureFormat textureFormat = FramebufferTextureFormat::None;
	// TODO: filtering/wrap
};

struct FramebufferAttachmentSpecification {
	FramebufferAttachmentSpecification() = default;
	FramebufferAttachmentSpecification(std::initializer_list<FramebufferTextureSpecification> attachments_)
		: attachments(attachments_) {}
	std::vector<FramebufferTextureSpecification> attachments;
};

/**
 * @brief Framebuffer specification
 */
struct FramebufferSpecification {
	/// width
	uint32_t width = 0;
	/// height
	uint32_t height = 0;
	FramebufferAttachmentSpecification attachments;
	/// Amount of sample
	uint32_t samples = 1;
	/// If chained target must be swap
	bool swapChainTarget = false;
};

#ifdef __clang__
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wweak-vtables"
#endif
/**
 * @brief Class Framebuffer
 */
class OWL_API Framebuffer {
public:
	/**
	 * @brief Default copy constructor
	 */
	Framebuffer(const Framebuffer &) = default;
	/**
	 * @brief Default move constructor
	 */
	Framebuffer(Framebuffer &&) = default;
	/**
	 * @brief Default copy assignation
	 * @return this
	 */
	Framebuffer &operator=(const Framebuffer &) = default;
	/**
	 * @brief Default move assignation
	 * @return this
	 */
	Framebuffer &operator=(Framebuffer &&) = default;
	/**
	 * @brief Default constructor.
	 */
	Framebuffer() = default;
	/**
	 * @brief Destructor.
	 */
	virtual ~Framebuffer() = default;
	/**
	 * @brief Activate the shader on the GPU
	 */
	virtual void bind() = 0;
	/**
	 * @brief Deactivate the shader on the GPU
	 */
	virtual void unbind() = 0;
	/**
	 * @brief Change the size of the frame buffer
	 * @param width New width
	 * @param height New height
	 */
	virtual void resize(uint32_t width, uint32_t height) = 0;
	virtual int readPixel(uint32_t attachmentIndex, int x, int y) = 0;
	virtual void clearAttachment(uint32_t attachmentIndex, int value) = 0;
	/**
	 * @brief Get renderer id
	 * @param index The color Index
	 * @return The renderer ID
	 */
	[[nodiscard]] virtual uint32_t getColorAttachmentRendererID(uint32_t index = 0) const = 0;
	/**
	 * @brief Get the specs
	 * @return The specs
	 */
	virtual const FramebufferSpecification &getSpecification() const = 0;
	/**
	 * @brief Create the frame buffer
	 * @param spec Specifications
	 * @return The Frame buffer
	 */
	static shrd<Framebuffer> create(const FramebufferSpecification &spec);

private:
};
#ifdef __clang__
#pragma clang diagnostic pop
#endif

}// namespace owl::renderer
