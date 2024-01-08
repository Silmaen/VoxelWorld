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
class OWL_API LayerStack {
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
	virtual ~LayerStack();

	/**
	 * @brief Adding a layer on top of the layers.
	 * @param layer The new layer to add.
	 */
	void pushLayer(shared<Layer> &&layer);

	/**
	 * @brief Adding an overlay on top of everything.
	 * @param overlay The new overlay.
	 */
	void pushOverlay(shared<Layer> &&overlay);

	/**
	 * @brief Remove the given layer.
	 * @param layer the layer to remove.
	 */
	void popLayer(const shared<Layer> &layer);

	/**
	 * @brief Remove the given overlay.
	 * @param overlay The overlay to remove.
	 */
	void popOverlay(const shared<Layer> &overlay);

	/**
	 * @brief Get the starting of the layer list.
	 * @return Iterator at the beginning of the layer list.
	 */
	[[nodiscard]] iterator begin() { return layers.begin(); }

	/**
	 * @brief Get the ending of the layer list.
	 * @return Iterator at the end of the layer list.
	 */
	[[nodiscard]] iterator end() { return layers.end(); }

	/**
	 * @brief Get the starting of the reversed layer list.
	 * @return Reverse iterator at the beginning of the layer list.
	 */
	[[nodiscard]] reverse_iterator rbegin() { return layers.rbegin(); }

	/**
	 * @brief Get the ending of the reversed layer list.
	 * @return Reverse iterator at the end of the layer list.
	 */
	[[nodiscard]] reverse_iterator rend() { return layers.rend(); }

	/**
	 * @brief Get the starting of the layer list.
	 * @return Const iterator at the beginning of the layer list.
	 */
	[[nodiscard]] const_iterator begin() const { return layers.begin(); }

	/**
	 * @brief Get the ending of the layer list.
	 * @return Const iterator at the end of the layer list.
	 */
	[[nodiscard]] const_iterator end() const { return layers.end(); }

	/**
	 * @brief Get the starting of the reversed layer list.
	 * @return Const reverse iterator at the beginning of the layer list.
	 */
	[[nodiscard]] const_reverse_iterator rbegin() const {
		return layers.rbegin();
	}

	/**
	 * @brief Get the ending of the reversed layer list.
	 * @return Const reverse iterator at the end of the layer list.
	 */
	[[nodiscard]] const_reverse_iterator rend() const { return layers.rend(); }

private:
	/// The layers.
	layerList layers;
	/// The inserting index.
	unsigned int layerInsertIndex = 0;
};

}// namespace owl::core::layer
