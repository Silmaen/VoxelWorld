/**
 * @file LayerStack.cpp
 * @author Silmaen
 * @date 04/12/2022
 * Copyright © 2022 All rights reserved.
 * All modification must get authorization from the author.
 */

#include "owlpch.h"

#include "LayerStack.h"

namespace owl::core::layer {

Layer::~Layer() = default;

LayerStack::~LayerStack() {
	for (auto &layer: m_layers) {
		layer->onDetach();
		layer.reset();
	}
}

void LayerStack::pushLayer(shared<Layer> &&layer) {
	layer->onAttach();
	m_layers.emplace(m_layers.begin() + m_layerInsertIndex, std::move(layer));
	m_layerInsertIndex++;
}

void LayerStack::pushOverlay(shared<Layer> &&overlay) {
	m_layers.emplace_back(std::move(overlay));
	m_layers.back()->onAttach();
}

void LayerStack::popLayer(const shared<Layer> &layer) {
	if (const auto it = std::find(m_layers.begin(), m_layers.begin() + m_layerInsertIndex, layer);
		it != m_layers.begin() + m_layerInsertIndex) {
		layer->onDetach();
		m_layers.erase(it);
		m_layerInsertIndex--;
	}
}

void LayerStack::popOverlay(const shared<Layer> &overlay) {
	if (const auto it = std::find(m_layers.begin() + m_layerInsertIndex, m_layers.end(), overlay);
		it != m_layers.end()) {
		overlay->onDetach();
		m_layers.erase(it);
	}
}

}// namespace owl::core::layer
