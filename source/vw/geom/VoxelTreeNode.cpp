/**
 * @file VoxelTreeNode.cpp
 * @author damien.lachouette
 * @date 24/11/2022
 * Copyright © 2022 All rights reserved.
 * All modification must get authorization from the author.
 */

#include "VoxelTreeNode.h"

namespace vw::geom {


Voxel::SizeType VoxelTreeNode::getSize() const {
  if (isLeaf())
    return 1;
  return children[0]->getSize();
}
Voxel::Coord VoxelTreeNode::getCenter() const {
  if (parent == nullptr)
    return {0,0,0};
  return parent->getCenter() +
         parent->getChildOffset(this) * static_cast<Voxel::CoordType>(parent->getSize() >> 2U);
}
Voxel::Coord VoxelTreeNode::getChildOffset(const VoxelTreeNode* who) const {
  for(Voxel::SizeType i=0;i<8;++i){
    if (who == children[i].get()){
      return {static_cast<Voxel::CoordType>(i&1?1:-1),static_cast<Voxel::CoordType>(i&2?1:-1),static_cast<Voxel::CoordType>(i&4?1:-1)};
    }
  }
  return {};
}

} // namespace vw