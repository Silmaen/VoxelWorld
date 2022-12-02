/**
 * @file VoxelTree.h
 * @author damien.lachouette
 * @date 24/11/2022
 * Copyright © 2022 All rights reserved.
 * All modification must get authorization from the author.
 */

#pragma once
#include "VoxelTreeNode.h"


namespace vw::geom {

/**
 * @brief Class VoxelTree
 */
class VoxelTree {
public:
  /**
   * @brief Default copy constructor
   */
  VoxelTree(const VoxelTree &) = default;
  /**
   * @brief Default move constructor
   */
  VoxelTree(VoxelTree &&) = default;
  /**
   * @brief Default copy assignation
   * @return this
   */
  VoxelTree &operator=(const VoxelTree &) = default;
  /**
   * @brief Default move assignation
   * @return this
   */
  VoxelTree &operator=(VoxelTree &&) = default;
  /**
   * @brief Default constructor.
   */
  VoxelTree() = default;
  /**
   * @brief Destructor.
   */
  virtual ~VoxelTree() = default; //---UNCOVER---

  void constructMap(uint8_t depth);

  void clearMap();

  [[nodiscard]] uint8_t getDepth()const{
    return depth;
  }

  enum class checkTreeResult {
    Good,
    IncompleteLevel,
    DepthTooLow,
    DepthTooHigh,
    OrphanNodes,
    BadNodeLink
  };
  [[nodiscard]] checkTreeResult checkTree()const;
private:

  void compactNodes();

  [[nodiscard]] uint8_t computeDepth()const;
  /// Current tree depth
  uint8_t depth = 0;
  /// Root node
  VoxelTreeNode::pNode rootNode = nullptr;
  /// Node pool
  std::vector<VoxelTreeNode::pNode> nodePool;
};

} // namespace vw::geom
