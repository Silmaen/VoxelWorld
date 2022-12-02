/**
 * @file VoxelTreeNode.cpp
 * @author damien.lachouette
 * @date 24/11/2022
 * Copyright © 2022 All rights reserved.
 * All modification must get authorization from the author.
 */

#include "VoxelTreeNode.h"
#include <algorithm>

namespace vw::geom {

constexpr uint8_t maxIndex{7};

Voxel::SizeType VoxelTreeNode::getSize() const {
  if (isLeaf())
    return 1;
  return 2 * children[0]->getSize();
}

Vect3f VoxelTreeNode::getCenter() const {
  if (parent == nullptr)
    return {0, 0, 0};
  auto off = parent->getChildOffset(this);
  return parent->getCenter() +
         Vect3f{static_cast<double>(off.x()), static_cast<double>(off.y()), static_cast<double>(off.z())} *
             parent->getSize() * .25;
}

Voxel::Coord VoxelTreeNode::getChildOffset(const VoxelTreeNode *who) const {
  for (Voxel::SizeType i = 0; i < 8; ++i) {
    if (who == children[i].get()) {
      return {static_cast<Voxel::CoordType>(i & 1 ? 1 : -1),
              static_cast<Voxel::CoordType>(i & 2 ? 1 : -1),
              static_cast<Voxel::CoordType>(i & 4 ? 1 : -1)};
    }
  }
  return {};
}

void VoxelTreeNode::clearChilds() {
  if (isLeaf())
    return;
  for (const auto &child : children) {
    child->clearChilds();
    child->setParent();
  }
  children = {nullptr, nullptr, nullptr, nullptr,
              nullptr, nullptr, nullptr, nullptr};
}

VoxelTreeNode::pNode &VoxelTreeNode::getChild(uint8_t index) {
  index = std::min(maxIndex, index);
  return children[index];
}

VoxelTreeNode::const_pNode &VoxelTreeNode::getChild(uint8_t index) const {
  index = std::min(maxIndex, index);
  return children[index];
}

bool VoxelTreeNode::hasAllChildren() const {
  return std::find_if(children.begin(), children.end(), [](const auto &it) {
           return it == nullptr;
         }) == children.end();
}

uint8_t VoxelTreeNode::getDepth() const {
  if (isOrphan())
    return 0;
  return parent->getDepth() + 1;
}

bool VoxelTreeNode::hasLinkedChildren() const {
  if (isLeaf())
    return true;
  for (const auto &child : children) {
    if (child->getParent().get() != this)
      return false;
    if (!child->hasLinkedChildren())
      return false;
  }
  return true;
}

} // namespace vw::geom