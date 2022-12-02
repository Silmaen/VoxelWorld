/**
 * @file VoxelTreeNode.h
 * @author damien.lachouette
 * @date 24/11/2022
 * Copyright © 2022 All rights reserved.
 * All modification must get authorization from the author.
 */

#pragma once

#include "Voxel.h"
#include <array>
#include <memory>

namespace vw::geom {

/**
 * @brief Class VoxelTreeNode
 */
class VoxelTreeNode {
public:
  /// Pointer to node
  using pNode = std::shared_ptr<VoxelTreeNode>;
  /// Pointer to node
  using const_pNode = const std::shared_ptr<VoxelTreeNode>;
  /// Children array
  using childrenType = std::array<pNode, 8>;
  /// Children iterator
  using childIterator = childrenType::iterator;
  /// Children iterator
  using const_childIterator = childrenType::const_iterator;
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
  [[nodiscard]] bool isLeaf() const { return children[0] == nullptr; }
  /**
   * @brief Check if the node has a parent
   * @return True if no parent found.
   */
  [[nodiscard]] bool isOrphan() const { return parent == nullptr; }

  /**
   * @brief Check if all children are defined
   * @return True if all are defined
   */
  [[nodiscard]] bool hasAllChildren()const;
  /**
   * @brief Check the link of all children
   * @return True if all link's good
   */
  [[nodiscard]] bool hasLinkedChildren()const;

    /**
   * @brief Get the size (in voxel number)
   * @return The size
   */
  [[nodiscard]] Voxel::SizeType getSize() const;

  /**
   * @brief Get node's center coordinates.
   * @return The world's coordinate of the voxel.
   */
  [[nodiscard]] Vect3f getCenter() const;

  /**
   * @brief Remove all childs
   */
  void clearChilds();

  /**
   * @brief Compute the current level of the node
   * @return the current level
   */
  [[nodiscard]] uint8_t getDepth()const;

  /**
   * @brief Define the parent none
   * @param p The new parent
   */
  void setParent(const pNode &p = nullptr) { parent = p; }
  /**
   * @brief Get the parent none
   * @return The parent
   */
  [[nodiscard]] const_pNode& getParent()const { return parent; }

  /**
   * @brief Access to the given child
   * @param index The child index
   * @return The child
   */
  pNode& getChild(uint8_t index);
  /**
   * @brief Access to the given child
   * @param index The child index
   * @return The child
   */
  [[nodiscard]] const_pNode& getChild(uint8_t index) const;

  childIterator begin(){return children.begin();}
  childIterator end(){return children.end();}
  [[nodiscard]] const_childIterator begin()const{return children.begin();}
  [[nodiscard]] const_childIterator end()const{return children.end();}
private:
  /**
   * @brief Pointer to parent node
   */
  pNode parent = nullptr;
  /**
   * @brief List of Children Node
   */
  childrenType children = {nullptr, nullptr, nullptr, nullptr,
                                   nullptr, nullptr, nullptr, nullptr};
  /**
   * @brief Get the child offsets
   * @param who
   * @return
   */
  [[nodiscard]] Voxel::Coord getChildOffset(const VoxelTreeNode *who) const;
};

} // namespace vw::geom
