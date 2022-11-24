/**
 * @file VoxelTree.h
 * @author damien.lachouette
 * @date 24/11/2022
 * Copyright © 2022 All rights reserved.
 * All modification must get authorization from the author.
 */

#pragma once

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
private:
};

} // namespace vw::geom
