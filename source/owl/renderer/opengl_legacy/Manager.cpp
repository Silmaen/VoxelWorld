//
// Created by damien.lachouette on 06/07/2023.
//

#include "Manager.h"

#include "gl_21/glad.h"

namespace owl::renderer::opengl_legacy {

void Manager::init() {
	gl_21::glEnable(GL_BLEND);
	gl_21::glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	gl_21::glEnable(GL_DEPTH_TEST);
	gl_21::glEnable(GL_LINE_SMOOTH);
}

void Manager::drawIndexed([[maybe_unused]] const shared<renderer::VertexArray> &vertexArray, [[maybe_unused]] uint32_t indexCount) {
	OWL_PROFILE_FUNCTION()

	// TODO
}
void Manager::drawLines(const shared<renderer::VertexArray> &vertexArray, uint32_t vertexCount) {
	vertexArray->bind();
	gl_21::glDrawArrays(GL_LINES, 0, static_cast<gl_21::GLsizei>(vertexCount));
}


}// namespace owl::renderer::opengl_legacy
