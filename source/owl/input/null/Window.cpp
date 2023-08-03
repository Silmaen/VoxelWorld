/**
 * @file Window.cpp
 * @author Silmaen
 * @date 04/12/2022
 * Copyright © 2022 All rights reserved.
 * All modification must get authorization from the author.
 */

#include "owlpch.h"

#include "Window.h"
#include "core/Log.h"
#include "debug/Profiler.h"

namespace owl::input::null {

static uint8_t s_WinCount = 0;

Window::Window(const Properties &props) : ::owl::input::Window() {
	OWL_PROFILE_FUNCTION()
	init(props);
}

Window::~Window() {
	OWL_PROFILE_FUNCTION()

	shutdown();
}

void Window::init(const Properties &props) {
	OWL_PROFILE_FUNCTION()
	windowData.title = props.title;
	windowData.width = props.width;
	windowData.height = props.height;

	OWL_CORE_INFO("Creating window {} ({}, {})", props.title, props.width,
				  props.height)
	++s_WinCount;
	setVSync(true);
}

void Window::shutdown() {
	OWL_PROFILE_FUNCTION()
	--s_WinCount;
}


void Window::onUpdate() {
	OWL_PROFILE_FUNCTION()
}

void Window::setVSync(bool enabled) {
	OWL_PROFILE_FUNCTION()
	windowData.VSync = enabled;
}

bool Window::isVSync() const { return windowData.VSync; }

}// namespace owl::input::null
