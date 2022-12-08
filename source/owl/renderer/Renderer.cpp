/**
 * @file Renderer.cpp
 * @author Silmen
 * @date 08/12/2022
 * Copyright © 2022 All rights reserved.
 * All modification must get authorization from the author.
 */
#include "owlpch.h"

#include "Renderer.h"

namespace owl::renderer {

APIType Renderer::getAPI() {
	return APIType::OpenGL;
}

}// namespace owl::renderer
