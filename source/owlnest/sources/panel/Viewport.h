/**
 * @file Viewport.h
 * @author Silmaen
 * @date 10/16/24
 * Copyright © 2024 All rights reserved.
 * All modification must get authorization from the author.
 */

#pragma once

#include <owl.h>

namespace owl::nest {
class EditorLayer;
}

namespace owl::nest::panel {
/**
 * @brief Class Viewport.
 */
class Viewport final : public gui::BasePanel {
public:
	/**
	 * @brief Default constructor.
	 */
	Viewport();
	/**
	 * @brief Default destructor.
	 */
	~Viewport() override;
	/**
	 * @brief Default copy constructor.
	 */
	Viewport(const Viewport&) = default;
	/**
	 * @brief Default move constructor.
	 */
	Viewport(Viewport&&) = default;
	/**
	 * @brief Default copy affectation operator.
	 */
	auto operator=(const Viewport&) -> Viewport& = default;
	/**
	 * @brief Default move affectation operator.
	 */
	auto operator=(Viewport&&) -> Viewport& = default;

	/**
	 * @brief Initialization when attached.
	 */
	void attach() override;
	/**
	 * @brief Cleaning after detached.
	 */
	void detach() override;
	/**
	 * @brief Update panel Status.
	 * @param iTimeStep The Time delta of the frame.
	 */
	void onUpdate(const core::Timestep& iTimeStep) override;
	/**
	 * @brief Action on event.
	 * @param[in,out] ioEvent The Event to react.
	 */
	void onEvent(event::Event& ioEvent) override;
	/**
	 * @brief Do the rendering.
	 */
	void onRenderInternal() override;

	/**
	 * @brief Defines the camera.
	 */
	void setCamera(const shared<renderer::Camera>& iCamera) { m_camera = iCamera; }

	/**
	 * @brief Define the parent layer.
	 */
	void attachParent(EditorLayer* iParent) { m_parent = iParent; }

	/**
	 * @brief Get the hovered entity.
	 * @return The Hovered entity, empty entity if not hovered.
	 */
	[[nodiscard]] auto getHoveredEntity() const -> scene::Entity { return m_hoveredEntity; }

	/**
	 * @brief Types of guizmo.
	 */
	enum struct GuizmoType : uint16_t {
		None = 0,///< Nothing displayed.
		Translation = 0b00000000000111,///< Translation control.
		Rotation = 0b00000001111000,///< Rotation control.
		Scale = 0b00001110000000,///< Scale control.
		All = 0b11100001111111,///< All at once.
	};

	void setGuizmoType(const GuizmoType& iType);
	[[nodiscard]] auto getGuizmoType() const -> GuizmoType;
	[[nodiscard]] auto getGuizmoTypeI() const -> uint16_t;

private:
	void renderGizmo();
	void renderOverlay() const;
	auto onKeyPressed(const event::KeyPressedEvent& ioEvent) -> bool;
	auto onMouseButtonPressed(const event::MouseButtonPressedEvent& ioEvent) -> bool;
	/// The camera.
	shared<renderer::Camera> m_camera;
	/// The frame buffer where to do the render.
	shared<renderer::Framebuffer> m_framebuffer;
	/// Pointer to the parent layer.
	EditorLayer* m_parent = nullptr;
	/// The hovered entity
	scene::Entity m_hoveredEntity;
	/// The Gizmo type
	GuizmoType m_gizmoType = GuizmoType::None;
	/// The editor camera
	renderer::CameraEditor m_editorCamera;
};

}// namespace owl::nest::panel
