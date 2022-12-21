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

/**
 * @brief Framebuffer specification
 */
struct FramebufferSpecification {
	/// width
	uint32_t width=0;
	/// height
	uint32_t height=0;
	/// Amount of sample
	uint32_t samples = 1;
	/// If chained target must be swap
	bool swapChainTarget = false;
};

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
	 * @brief Get renderer id
	 * @return The renderer ID
	 */
	virtual uint32_t getColorAttachmentRendererID() const = 0;
	/**
	 * @brief Get the specs
	 * @return The specs
	 */
	virtual const FramebufferSpecification& getSpecification() const = 0;
	/**
	 * @brief Create the frame buffer
	 * @param spec Specifications
	 * @return The Frame buffer
	 */
	static shrd<Framebuffer> create(const FramebufferSpecification& spec);
private:
};

}// namespace owl::renderer
