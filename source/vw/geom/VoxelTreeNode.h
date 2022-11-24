/**
 * @file VoxelTreeNode.h
 * @author damien.lachouette
 * @date 24/11/2022
 * Copyright © 2022 All rights reserved.
 * All modification must get authorization from the author.
 */

#pragma once

#include <memory>
#include <array>
#include "Voxel.h"

namespace vw::geom {

/**
 * @brief Class VoxelTreeNode
 */
class VoxelTreeNode {
public:

  /// Pointer to node
  using pnode = std::shared_ptr<VoxelTreeNode>;

  /**
   * @brief Default copy constructor
   */
  VoxelTreeNode(const VoxelTreeNode &) = default;
  /**
   * @brief Default move constructor
   */
  VoxelTreeNode(VoxelTreeNode &&) = default;
  /**
   * @brief Default copy assignation
   * @return this
   */
  VoxelTreeNode &operator=(const VoxelTreeNode &) = default;
  /**
   * @brief Default move assignation
   * @return this
   */
  VoxelTreeNode &operator=(VoxelTreeNode &&) = default;
  /**
   * @brief Default constructor.
   */
  VoxelTreeNode() = default;
  /**
   * @brief Destructor.
   */
  virtual ~VoxelTreeNode() = default; //---UNCOVER---

  /**
   * @brief Check if the node is a leaf (no child)
   * @return True is there is no child
   */
  [[nodiscard]] bool isLeaf()const {return children[0] == nullptr;}

  /**
   * @brief Get the size (in voxel number)
   * @return The size
   */
  [[nodiscard]] Voxel::SizeType getSize()const;

  [[nodiscard]] Voxel::Coord getCenter() const;
private:
  /**
   * @brief Pointer to parent node
   */
  pnode parent = nullptr;
  /**
   * @brief List of Children Node
   */
  std::array<pnode,8> children = {nullptr,nullptr,nullptr,nullptr,nullptr,nullptr,nullptr,nullptr};

  [[nodiscard]] Voxel::Coord getChildOffset(const VoxelTreeNode* who)const;
};

} // namespace vw::geom
