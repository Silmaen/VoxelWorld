/**
 * @file VoxelTree.cpp
 * @author damien.lachouette
 * @date 24/11/2022
 * Copyright © 2022 All rights reserved.
 * All modification must get authorization from the author.
 */

#include "VoxelTree.h"
#include <algorithm>
#include <magic_enum.hpp>
#include <spdlog/spdlog.h>

namespace vw::geom {

void VoxelTree::constructMap(uint8_t depth_) {
  spdlog::info("Construct Voxel tree with a depth of {}", depth_);
  clearMap();
  if (depth_ == 0) {
    return;
  }
  // reserve the full memory
  size_t theoreticalNumber = 0;
  size_t levelNumber = 8;
  for (uint8_t i = 0; i < depth_; ++i) {
    theoreticalNumber += levelNumber;
    levelNumber *= 8;
  }
  nodePool.resize(theoreticalNumber);
  auto it = nodePool.begin();
  // populate the root node
  rootNode = std::make_shared<VoxelTreeNode>();
  for (auto &child : *rootNode) {
    if (*it == nullptr)
      *it = std::make_shared<VoxelTreeNode>();
    child = *it;
    child->setParent(rootNode);
    ++it;
  }
  // populate each level
  for (uint8_t lvl = 1; lvl < depth_; ++lvl) {
    for (auto &levelNode : nodePool) {
      if (levelNode == nullptr)
        continue;
      if (levelNode->getDepth() != lvl)
        continue;
      if (levelNode->hasAllChildren())
        continue;
      for (auto &child : *levelNode) {
        if (*it == nullptr)
          *it = std::make_shared<VoxelTreeNode>();
        child = *it;
        child->setParent(levelNode);
        ++it;
      }
    }
  }
  depth = computeDepth();
  // verifications
  if (it != nodePool.end()) {
    spdlog::warn("Voxel Tree creation: iterator issue");
  }
  auto result = checkTree();
  if (result != checkTreeResult ::Good) {
    spdlog::warn("Voxel Tree creation: bad check: {}",
                 magic_enum::enum_name(result));
  }
}

void VoxelTree::clearMap() {
  if (rootNode)
    rootNode->clearChilds();
  for(auto& node:nodePool){
    node->clearChilds();
    node->setParent();
  }
  nodePool.clear();
  depth = 0;
}

void VoxelTree::compactNodes() {
  std::remove_if(nodePool.begin(), nodePool.end(), [](const auto &it) {
    return ((it == nullptr) || (it->isOrphan()));
  });
}

uint8_t VoxelTree::computeDepth() const {
  VoxelTreeNode::pNode node = rootNode;
  uint8_t iDepth = 0;
  while (!node->isLeaf()) {
    ++iDepth;
    node = node->getChild(0);
  }
  return iDepth;
}

VoxelTree::checkTreeResult VoxelTree::checkTree() const {
  if (rootNode == nullptr) {
    if (nodePool.empty())
      return checkTreeResult::Good;
    return checkTreeResult::OrphanNodes;
  }
  // check for orphan nodes
  auto firstOrphan =
      std::find_if(nodePool.begin(), nodePool.end(),
                   [](const auto &it) { return it->isOrphan(); });
  if (firstOrphan != nodePool.end())
    return checkTreeResult::OrphanNodes;
  // check depths of oll nodes
  bool tooLow = true;
  for (const auto &node : nodePool) {
    auto nDepth = node->getDepth();
    if (nDepth > depth)
      return checkTreeResult::DepthTooHigh;
    if (nDepth == depth)
      tooLow = false;
  }
  if (tooLow)
    return checkTreeResult::DepthTooLow;
  if (!rootNode->hasLinkedChildren())
    return checkTreeResult::BadNodeLink;
  // check for incomplete levels: if previous check is Ok so the number of nodes
  // in the pool is sufficient to know if there are missings
  size_t theoreticalNumber = 0;
  size_t levelNumber = 8;
  for (uint8_t i = 0; i < depth; ++i) {
    theoreticalNumber += levelNumber;
    levelNumber *= 8;
  }
  if (nodePool.size() != theoreticalNumber)
    return checkTreeResult::IncompleteLevel;
  return checkTreeResult::Good;
}

} // namespace vw::geom
