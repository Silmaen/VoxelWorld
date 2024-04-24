/**
 * @file LayerStack.h
 * @author Silmaen
 * @date 04/12/2022
 * Copyright © 2022 All rights reserved.
 * All modification must get authorization from the author.
 */

#pragma once
#include "Layer.h"
#include <vector>

namespace owl::core::layer {

/**
 * @brief Class LayerStack.
 */
class OWL_API LayerStack final {
public:
	/// Type for the list of Layer.
	using layerList = std::vector<shared<Layer>>;
	/// Iterator in the layer list.
	using iterator = layerList::iterator;
	/// Const iterator in the layer list.
	using const_iterator = layerList::const_iterator;
	/// Reverse iterator in the layer list.
	using reverse_iterator = layerList::reverse_iterator;
	/// Const reverse iterator in the layer list.
	using const_reverse_iterator = layerList::const_reverse_iterator;
	LayerStack(const LayerStack &) = delete;
	LayerStack(LayerStack &&) = delete;
	LayerStack &operator=(const LayerStack &) = delete;
	LayerStack &operator=(LayerStack &&) = delete;

	/**
	 * @brief Default constructor.
	 */
	LayerStack() = default;

	/**
	 * @brief Destructor.
	 */
	~LayerStack();

	/**
	 * @brief Adding a layer on top of the layers.
	 * @param[in] iLayer The new layer to add.
	 */
	void pushLayer(shared<Layer> &&iLayer);

	/**
	 * @brief Adding an overlay on top of everything.
	 * @param[in] iOverlay The new overlay.
	 */
	void pushOverlay(shared<Layer> &&iOverlay);

	/**
	 * @brief Remove the given layer.
	 * @param[in] iLayer the layer to remove.
	 */
	void popLayer(const shared<Layer> &iLayer);

	/**
	 * @brief Remove the given overlay.
	 * @param[in] iOverlay The overlay to remove.
	 */
	void popOverlay(const shared<Layer> &iOverlay);

	/**
	 * @brief Get the starting of the layer list.
	 * @return Iterator at the beginning of the layer list.
	 */
	[[nodiscard]] iterator begin() { return m_layers.begin(); }

	/**
	 * @brief Get the ending of the layer list.
	 * @return Iterator at the end of the layer list.
	 */
	[[nodiscard]] iterator end() { return m_layers.end(); }

	/**
	 * @brief Get the starting of the reversed layer list.
	 * @return Reverse iterator at the beginning of the layer list.
	 */
	[[nodiscard]] reverse_iterator rbegin() { return m_layers.rbegin(); }

	/**
	 * @brief Get the ending of the reversed layer list.
	 * @return Reverse iterator at the end of the layer list.
	 */
	[[nodiscard]] reverse_iterator rend() { return m_layers.rend(); }

	/**
	 * @brief Get the starting of the layer list.
	 * @return Const iterator at the beginning of the layer list.
	 */
	[[nodiscard]] const_iterator begin() const { return m_layers.begin(); }

	/**
	 * @brief Get the ending of the layer list.
	 * @return Const iterator at the end of the layer list.
	 */
	[[nodiscard]] const_iterator end() const { return m_layers.end(); }

	/**
	 * @brief Get the starting of the reversed layer list.
	 * @return Const reverse iterator at the beginning of the layer list.
	 */
	[[nodiscard]] const_reverse_iterator rbegin() const { return m_layers.rbegin(); }

	/**
	 * @brief Get the ending of the reversed layer list.
	 * @return Const reverse iterator at the end of the layer list.
	 */
	[[nodiscard]] const_reverse_iterator rend() const { return m_layers.rend(); }

	/**
	 * @brief Destroy all layers and overlays.
	 */
	void clear();

private:
	/// The layers.
	layerList m_layers;
	/// The inserting index.
	unsigned int m_layerInsertIndex = 0;
};

}// namespace owl::core::layer
