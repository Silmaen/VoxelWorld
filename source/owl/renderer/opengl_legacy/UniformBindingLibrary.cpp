/**
 * @file UniformBindingLibrary.cpp
 * @author Silmaen
 * @date 01/09/2023
 * Copyright © 2023 All rights reserved.
 * All modification must get authorization from the author.
 */
#include "owlpch.h"

#include "UniformBindingLibrary.h"

namespace owl::renderer::opengl_legacy {


const UniformBuffer *UniformBindingLibrary::getUniformBuffer(uint32_t iBinding) {
	const auto result = std::ranges::find_if(m_uniforms.begin(), m_uniforms.end(),
											 [&iBinding](auto &iItem) { return iItem->getBinding() == iBinding; });
	if (result == m_uniforms.end())
		return nullptr;
	return *result;
}

void UniformBindingLibrary::addUniformBuffer(const UniformBuffer *iBuffer) {
	removeUniformBuffer(iBuffer->getBinding());
	m_uniforms.push_back(iBuffer);
}

void UniformBindingLibrary::removeUniformBuffer(uint32_t iBinding) {
	if (const auto result = std::remove_if(m_uniforms.begin(), m_uniforms.end(),
										   [&iBinding](auto &iItem) {
											   return iItem->getBinding() == iBinding;
										   }); result == m_uniforms.end())
		OWL_CORE_TRACE("Nothing to remove.")
}

}// namespace owl::renderer::opengl_legacy
