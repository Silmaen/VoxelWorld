/**
 * @file owl.h
 * @author Silmaen
 * @date 04/12/2022
 * Copyright © 2022 All rights reserved.
 * All modification must get authorization from the author.
 */

#pragma once


// ------- core ------------
#include "core/Application.h"
#include "core/Log.h"
// -------------------------

#include "core/Timestep.h"
#include "core/layer/Layer.h"
#include "gui/ImGuiLayer.h"
#include "input/CameraOrthoController.h"
#include "input/Input.h"

// ------ renderer ---------
#include "renderer/Buffer.h"
#include "renderer/CameraOrtho.h"
#include "renderer/Framebuffer.h"
#include "renderer/RenderCommand.h"
#include "renderer/Renderer.h"
#include "renderer/Renderer2D.h"
#include "renderer/ShaderLibrary.h"
#include "renderer/Texture.h"
#include "renderer/VertexArray.h"
// -------------------------

// ------ Debugging --------
#include "debug/Tracker.h"
#include "debug/Profiler.h"
// -------------------------
