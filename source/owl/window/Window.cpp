/**
 * @file Window.cpp
 * @author Silmaen
 * @date 04/12/2022
 * Copyright © 2022 All rights reserved.
 * All modification must get authorization from the author.
 */

#include "Window.h"
#include "glfw/Window.h"
#include "null/Window.h"

namespace owl::window {

uniq<Window> Window::Create(const Properties &props) {
  if (props.winType == Type::GLFW)
    return mk_uniq<glfw::Window>(props);
  return mk_uniq<null::Window>(props);
}

} // namespace owl::window
