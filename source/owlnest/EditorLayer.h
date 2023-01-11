/**
 * @file EditorLayer.h
 * @author Silmaen
 * @date 21/12/2022
 * Copyright © 2022 All rights reserved.
 * All modification must get authorization from the author.
 */

#pragma once

#include "owl.h"

#include "event/KeyEvent.h"
#include "panel/SceneHierarchy.h"
#include "panel/ContentBrowser.h"
#include "renderer/CameraEditor.h"

namespace owl {

/**
 * @brief Class EditorLayer
 */
class EditorLayer : public core::layer::Layer {
public:
	EditorLayer(const EditorLayer &) = delete;
	EditorLayer(EditorLayer &&) = delete;
	EditorLayer &operator=(const EditorLayer &) = delete;
	EditorLayer &operator=(EditorLayer &&) = delete;
	/**
	 * @brief Default constructor.
	 */
	EditorLayer();
	/**
	 * @brief Destructor.
	 */
	~EditorLayer() override = default;

	void onAttach() override;
	void onDetach() override;
	void onUpdate(const core::Timestep &ts) override;
	void onEvent(event::Event &event) override;
	void onImGuiRender(const core::Timestep &ts) override;

private:
	void renderViewport();

	void renderStats(const core::Timestep &ts);
	void renderMenu();
	void renderGizmo();
	void renderToolbar();

	void newScene();
	void openScene();
	void openScene(const std::filesystem::path& scene);
	void saveSceneAs();
	void saveSceneAs(const std::filesystem::path& scene);
	void saveCurrentScene();

	bool onKeyPressed(event::KeyPressedEvent& e);
	bool onMouseButtonPressed(event::MouseButtonPressedEvent& e);
	void onScenePlay() {
		state = State::Play;
	}
	void onSceneStop(){
		state = State::Edit;
	}


	input::CameraOrthoController cameraController;

	enum class State {
		Edit,
		Play
	};
	State state = State::Edit;

	scene::Entity hoveredEntity;
	renderer::CameraEditor editorCamera;

	bool viewportFocused = false;
	bool viewportHovered = false;
	glm::vec2 viewportSize = {0.0f, 0.0f};
	glm::vec2 viewportBounds[2];
	shrd<renderer::Framebuffer> framebuffer;

	shrd<scene::Scene> activeScene;

	int gizmoType = -1;
	std::filesystem::path currentScenePath{};

	// Panels
	panel::SceneHierarchy sceneHierarchy;
	panel::ContentBrowser contentBrowser;

	// Editor resources
	shrd<renderer::Texture2D> iconPlay;
	shrd<renderer::Texture2D> iconStop;
};

}// namespace owl
