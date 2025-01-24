/**
 * @file RunnerLayer.h
 * @author Silmaen
 * @date 21/12/2022
 * Copyright © 2022 All rights reserved.
 * All modification must get authorization from the author.
 */

#pragma once

#include <owl.h>

namespace owl::nest::runner {

struct RunnerConfig {
	std::string firstScene;
	void loadYaml(const std::filesystem::path& iPath);
	void saveYaml(const std::filesystem::path& iPath) const;
};

/**
 * @brief Class RunnerLayer
 */
class RunnerLayer final : public core::layer::Layer {
public:
	RunnerLayer(const RunnerLayer&) = delete;
	RunnerLayer(RunnerLayer&&) = delete;
	auto operator=(const RunnerLayer&) -> RunnerLayer& = delete;
	auto operator=(RunnerLayer&&) -> RunnerLayer& = delete;
	/**
	 * @brief Default constructor.
	 */
	RunnerLayer();
	/**
	 * @brief Destructor.
	 */
	~RunnerLayer() override = default;

	void onAttach() override;
	void onDetach() override;
	void onUpdate(const core::Timestep& iTimeStep) override;
	void onEvent(event::Event& ioEvent) override;
	void onImGuiRender(const core::Timestep& iTimeStep) override;

	auto getActiveScene() const -> const shared<scene::Scene>& { return m_activeScene; }

private:
	auto onKeyPressed(const event::KeyPressedEvent& ioEvent) -> bool;
	auto onMouseButtonPressed(const event::MouseButtonPressedEvent& ioEvent) -> bool;

	shared<scene::Scene> m_activeScene;
	math::vec2ui m_viewportSize = {0, 0};
	RunnerConfig m_config;
};
}// namespace owl::nest::runner
