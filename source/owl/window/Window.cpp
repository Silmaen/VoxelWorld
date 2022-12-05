/**
 * @file Window.cpp
 * @author argawaen
 * @date 04/12/2022
 * Copyright © 2022 All rights reserved.
 * All modification must get authorization from the author.
 */

#include "Window.h"
#include "glfw/Window.h"

namespace owl::window {

uniq<Window> Window::Create(const Properties &props) {
  return mk_uniq<glfw::Window>(props);
}

}
