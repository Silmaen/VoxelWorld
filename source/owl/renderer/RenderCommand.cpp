/**
 * @file RenderCommand.cpp
 * @author Silmaen
 * @date 09/12/2022
 * Copyright © 2022 All rights reserved.
 * All modification must get authorization from the author.
 */
#include "owlpch.h"

#include "RenderCommand.h"

namespace owl::renderer {

uniq<RenderAPI> RenderCommand::renderAPI = nullptr;

void RenderCommand::create(const RenderAPI::Type &type) {
	renderAPI = RenderAPI::create(type);
}

RenderAPI::State RenderCommand::getState() {
	if (renderAPI)
		return renderAPI->getState();
	return RenderAPI::State::Error;
}

RenderCommand::~RenderCommand() = default;

}// namespace owl::renderer
