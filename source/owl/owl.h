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
#include "core/utils/FileDialog.h"
#include "core/utils/FileUtils.h"
#include "gui/Theme.h"
#include "gui/UILayer.h"
#include "input/CameraOrthoController.h"
#include "input/Input.h"
#include "input/Window.h"

// ------- scene -----------
#include "scene/Entity.h"
#include "scene/Scene.h"
//--------------------------

// ------ renderer ---------
#include "renderer/Buffer.h"
#include "renderer/CameraOrtho.h"
#include "renderer/Framebuffer.h"
#include "renderer/RenderCommand.h"
#include "renderer/Renderer.h"
#include "renderer/Renderer2D.h"
#include "renderer/ShaderLibrary.h"
#include "renderer/Texture.h"
// -------------------------

// ------ Debugging --------
#include "debug/Profiler.h"
#include "debug/Tracker.h"
// -------------------------
