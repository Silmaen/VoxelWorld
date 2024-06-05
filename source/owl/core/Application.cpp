/**
 * @file Application.cpp
 * @author Silmaen
 * @date 04/12/2022
 * Copyright © 2022 All rights reserved.
 * All modification must get authorization from the author.
 */

#include "owlpch.h"

#include "Application.h"

#include "Environment.h"
#include "core/external/yaml.h"
#include "input/Input.h"
#include "renderer/Renderer.h"

namespace owl::core {

Application *Application::s_instance = nullptr;

Application::Application(AppParams iAppParams) : m_initParams{std::move(iAppParams)} {
    OWL_PROFILE_FUNCTION()

    OWL_CORE_ASSERT(!s_instance, "Application already exists!")
    s_instance = this;

    // Look for things on the storages
    {
        // Set up a working directory
        // Assuming present of a folder 'res' containing the data
        m_workingDirectory = absolute(std::filesystem::current_path());
        OWL_CORE_INFO("Working directory: {}", m_workingDirectory.string())

        // load config file if any
        if (!m_initParams.isDummy) {
            const auto configPath = m_workingDirectory / "config.yml";
            if (exists(configPath))
                m_initParams.loadFromFile(configPath);
            // save config
            m_initParams.saveToFile(configPath);
        }

        if (m_initParams.useDebugging) {
            appendEnv("VK_ADD_LAYER_PATH", m_workingDirectory.string());
#ifdef OWL_VULKAN_LAYER_PATH
            appendEnv("VK_ADD_LAYER_PATH", OWL_VULKAN_LAYER_PATH);
#endif
        }

        Log::setFrameFrequency(m_initParams.frameLogFrequency);
        [[maybe_unused]] const bool assetFound = searchAssets(m_initParams.assetsPattern);
        OWL_CORE_ASSERT(assetFound, "Unable to find assets")
    }

    // Create the renderer
    {
        renderer::RenderCommand::create(m_initParams.renderer);
        // check renderer creation
        if (renderer::RenderCommand::getState() != renderer::RenderAPI::State::Created) {
            OWL_CORE_ERROR("ERROR while Creating Renderer")
            m_state = State::Error;
            return;
        }
    }

    // create main window
    {
        mu_appWindow = window::Window::create({
                .winType = m_initParams.isDummy ? window::Type::Null : window::Type::GLFW,
                .title = m_initParams.name,
                .iconPath = m_initParams.icon.empty() ? "" : (m_assetDirectory / m_initParams.icon).string(),
                .width = m_initParams.width,
                .height = m_initParams.height,
        });
        input::Input::init();
        OWL_CORE_INFO("Window Created.")
    }

    // initialize the renderer
    {
        renderer::Renderer::init();
        // check renderer initialization
        if (renderer::RenderCommand::getState() != renderer::RenderAPI::State::Ready) {
            OWL_CORE_ERROR("ERROR while Initializing Renderer")
            m_state = State::Error;
            return;
        }
        OWL_CORE_INFO("Renderer initiated.")
    }

    // set up the callbacks
    mu_appWindow->setEventCallback([this](auto &&PH1) { onEvent(std::forward<decltype(PH1)>(PH1)); });

    // create the GUI layer
    if (m_initParams.hasGui) {
        mp_imGuiLayer = mkShared<gui::UiLayer>();
        pushOverlay(mp_imGuiLayer);
        OWL_CORE_TRACE("GUI Layer created.")
    }

    OWL_CORE_TRACE("Application creation done.")
}

void Application::enableDocking() {
    if (mp_imGuiLayer)
        mp_imGuiLayer->enableDocking();
}

void Application::disableDocking() {
    if (mp_imGuiLayer)
        mp_imGuiLayer->disableDocking();
}

Application::~Application() {
    OWL_PROFILE_FUNCTION()

    if (renderer::RenderCommand::getState() != renderer::RenderAPI::State::Error) {
        m_layerStack.clear();
        input::Input::invalidate();
        mu_appWindow->shutdown();
        OWL_CORE_TRACE("Application window shut down.")
        renderer::Renderer::shutdown();
        renderer::RenderCommand::invalidate();
        OWL_CORE_TRACE("Renderer shut down and invalidated.")
        mu_appWindow.reset();
    }
    invalidate();
}

void Application::close() { m_state = State::Stopped; }

void Application::invalidate() { s_instance = nullptr; }

void Application::run() {
    OWL_PROFILE_FUNCTION()

#if OWL_TRACKER_VERBOSITY >= 3
    uint64_t frameCount = 0;
#endif
    while (m_state == State::Running) {
        OWL_PROFILE_SCOPE("RunLoop")
        OWL_CORE_FRAME_ADVANCE

        m_stepper.update();
        if (!m_minimized) {
            renderer::RenderCommand::beginFrame();
            if (renderer::RenderCommand::getState() != renderer::RenderAPI::State::Ready) {
                m_state = State::Error;
                continue;
            }
            {
                OWL_PROFILE_SCOPE("LayerStack onUpdate")

                for (const auto &layer: m_layerStack) layer->onUpdate(m_stepper);
            }
            if (mp_imGuiLayer) {
                OWL_PROFILE_SCOPE("LayerStack onImUpdate")
                mp_imGuiLayer->begin();
                for (const auto &layer: m_layerStack) layer->onImGuiRender(m_stepper);
                mp_imGuiLayer->end();
            }
            renderer::RenderCommand::endFrame();
        }
        mu_appWindow->onUpdate();

#if OWL_TRACKER_VERBOSITY >= 3
        {
            const auto &memState = debug::Tracker::get().checkState();
            if (memState.allocationCalls > memState.deallocationCalls && frameCount > 0) {
                OWL_CORE_TRACE("----------------------------------")
                OWL_CORE_TRACE("Frame Leak Detected")
                OWL_CORE_TRACE("-----------------------------------")
                OWL_CORE_TRACE("")
                OWL_CORE_TRACE(" LEAK Amount: {} in {} Unallocated chunks", memState.allocatedMemory,
                               memState.allocs.size())
                for (const auto &chunk: memState.allocs) { OWL_CORE_TRACE(" ** {}", chunk.toStr()) }
                OWL_CORE_TRACE("----------------------------------")
                OWL_CORE_TRACE("")
            }
        }
        ++frameCount;
#endif
    }
}

void Application::onEvent(event::Event &ioEvent) {
    OWL_PROFILE_FUNCTION()

    event::EventDispatcher dispatcher(ioEvent);
    dispatcher.dispatch<event::WindowCloseEvent>(
            [this](auto &&PH1) { return onWindowClosed(std::forward<decltype(PH1)>(PH1)); });
    dispatcher.dispatch<event::WindowResizeEvent>(
            [this](auto &&PH1) { return onWindowResized(std::forward<decltype(PH1)>(PH1)); });

#if defined(__clang__) and __clang_major__ < 16
    for (auto it2 = m_layerStack.rbegin(); it2 != m_layerStack.rend(); ++it2) {
        auto it = (*it2);
#else
    for (const auto &it: std::ranges::reverse_view(m_layerStack)) {
#endif
        if (ioEvent.handled)
            break;
        it->onEvent(ioEvent);
    }
}

bool Application::onWindowClosed(event::WindowCloseEvent &) {
    OWL_PROFILE_FUNCTION()

    close();
    return true;
}

bool Application::onWindowResized(const event::WindowResizeEvent &iEvent) {
    OWL_PROFILE_FUNCTION()

    if (iEvent.getWidth() == 0 || iEvent.getHeight() == 0) {
        m_minimized = true;
        return false;
    }
    m_minimized = false;
    renderer::Renderer::onWindowResized(iEvent.getWidth(), iEvent.getHeight());
    return false;
}

void Application::pushLayer(shared<layer::Layer> &&iLayer) {
    OWL_PROFILE_FUNCTION()
    if (renderer::RenderCommand::getState() == renderer::RenderAPI::State::Error)
        return;
    m_layerStack.pushLayer(std::move(iLayer));
}

void Application::pushOverlay(shared<layer::Layer> &&iOverlay) {
    OWL_PROFILE_FUNCTION()
    if (renderer::RenderCommand::getState() == renderer::RenderAPI::State::Error)
        return;
    m_layerStack.pushOverlay(std::move(iOverlay));
}

bool Application::searchAssets(const std::string &iPattern) {
    OWL_PROFILE_FUNCTION()

    std::filesystem::path parent = m_workingDirectory;
    std::filesystem::path assets = parent / iPattern;
    while (parent != parent.root_path()) {
        if (exists(assets)) {
            m_assetDirectory = std::move(assets);
            return true;
        }
        assets.clear();
        parent = parent.parent_path();
        assets = parent;
        assets /= iPattern;
    }
    m_assetDirectory = m_workingDirectory;
    return false;
}

void AppParams::loadFromFile(const std::filesystem::path &iFile) {
    YAML::Node data = YAML::LoadFile(iFile.string());
    if (auto appConfig = data["AppConfig"]; appConfig) {
        if (appConfig["width"])
            width = appConfig["width"].as<uint32_t>();
        if (appConfig["height"])
            height = appConfig["height"].as<uint32_t>();
        if (appConfig["renderer"]) {
            if (const auto dRenderer =
                        magic_enum::enum_cast<renderer::RenderAPI::Type>(appConfig["renderer"].as<std::string>());
                dRenderer.has_value())
                renderer = dRenderer.value();
        }
        if (appConfig["hasGui"])
            hasGui = appConfig["hasGui"].as<bool>();
        if (appConfig["useDebugging"])
            useDebugging = appConfig["useDebugging"].as<bool>();
        if (appConfig["frameLogFrequency"])
            frameLogFrequency = appConfig["frameLogFrequency"].as<uint64_t>();
    }
}

void AppParams::saveToFile(const std::filesystem::path &iFile) const {
    YAML::Emitter out;
    out << YAML::BeginMap;
    out << YAML::Key << "AppConfig" << YAML::Value << YAML::BeginMap;

    out << YAML::Key << "width" << YAML::Value << width;
    out << YAML::Key << "height" << YAML::Value << height;
    out << YAML::Key << "renderer" << YAML::Value << std::string(magic_enum::enum_name(renderer));
    out << YAML::Key << "hasGui" << YAML::Value << hasGui;
    out << YAML::Key << "useDebugging" << YAML::Value << useDebugging;
    out << YAML::Key << "frameLogFrequency" << YAML::Value << frameLogFrequency;

    out << YAML::EndMap;
    out << YAML::EndMap;
    std::ofstream fileOut(iFile);
    fileOut << out.c_str();
    fileOut.close();
}

}// namespace owl::core
