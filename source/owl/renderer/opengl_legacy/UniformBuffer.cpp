/**
 * @file UniformBuffer.cpp
 * @author Silmaen
 * @date 03/08/2023
 * Copyright © 2023 All rights reserved.
 * All modification must get authorization from the author.
 */
#include "owlpch.h"

#include "UniformBindingLibrary.h"
#include "UniformBuffer.h"
#include "core/external/opengl21.h"

namespace owl::renderer::opengl_legacy {

UniformBuffer::UniformBuffer(const uint32_t iSize, const uint32_t iBinding) {
	m_internalBinding = iBinding;
	m_internalData.resize(iSize);
	UniformBindingLibrary::get().addUniformBuffer(this);
}

UniformBuffer::~UniformBuffer() { UniformBindingLibrary::get().removeUniformBuffer(m_internalBinding); }

OWL_DIAG_PUSH
OWL_DIAG_DISABLE_CLANG("-Wunsafe-buffer-usage")
void UniformBuffer::setData(const void *iData, const uint32_t iSize, const uint32_t iOffset) {
	memcpy(m_internalData.data() + iOffset, iData, iSize);
}
OWL_DIAG_POP

}// namespace owl::renderer::opengl_legacy
