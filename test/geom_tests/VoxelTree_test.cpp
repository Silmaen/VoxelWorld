
#include "geom/VoxelTree.h"
#include "testHelper.h"

using namespace vw::geom;

TEST(VoxelTree, base){
  VoxelTree tree;
  EXPECT_EQ(tree.getDepth(), 0);
  EXPECT_EQ(tree.checkTree(), VoxelTree::checkTreeResult::Good);
}

TEST(VoxelTree, Create){
  VoxelTree tree;
  tree.constructMap(5);
  EXPECT_EQ(tree.getDepth(), 5);
  EXPECT_EQ(tree.checkTree(), VoxelTree::checkTreeResult::Good);
  tree.clearMap();
}
