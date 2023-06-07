//
// Created by damien.lachouette on 06/07/2023.
//

#pragma once
#include "Buffer.h"
#include "Framebuffer.h"
#include "VertexArray.h"
#include "core/Core.h"

namespace owl::renderer::opengl_legacy {

class OWL_API Manager {
public:
	Manager(Manager const &) = delete;
	Manager(Manager &&) = delete;
	void operator=(const Manager &) = delete;
	void operator=(Manager &&) = delete;

	static Manager &get() {
		static Manager instance;
		return instance;
	}

	void init();

	void bindVertexBuffer(opengl_legacy::VertexBuffer *buf) {
		curVertexBuffer = buf;
	}
	void bindIndexBuffer(opengl_legacy::IndexBuffer *buf) {
		curIndexBuffer = buf;
	}
	void bindFrameBuffer(opengl_legacy::Framebuffer *buf) {
		curFrameBuffer = buf;
	}

	void drawIndexed(const shared<renderer::VertexArray> &vertexArray, uint32_t indexCount);

	void drawLines(const shared<renderer::VertexArray> &vertexArray, uint32_t vertexCount);

private:
	Manager() {}

	opengl_legacy::VertexBuffer *curVertexBuffer = nullptr;
	opengl_legacy::IndexBuffer *curIndexBuffer = nullptr;
	opengl_legacy::Framebuffer *curFrameBuffer = nullptr;
};

}// namespace owl::renderer::opengl_legacy
