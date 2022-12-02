
#include "geom/VoxelTreeNode.h"
#include "testHelper.h"

using namespace vw::geom;

TEST(VoxelTreeNode, base){
  VoxelTreeNode node;
  EXPECT_TRUE(node.isLeaf());
  EXPECT_TRUE(node.isOrphan());
  EXPECT_FALSE(node.hasAllChildren());
  EXPECT_TRUE(node.hasLinkedChildren());
  EXPECT_EQ(node.getDepth(), 0);
  EXPECT_EQ(node.getParent(), nullptr);
  EXPECT_EQ(node.getSize(), 1);
  EXPECT_EQ(node.getCenter(), Vect3f(0,0,0) );
  node.clearChilds();
}

TEST(VoxelTreeNode, badlyfilled){
  auto node = std::make_shared<VoxelTreeNode>();
  *(node->begin()) = std::make_shared<VoxelTreeNode>();
  *(node->begin() + 1) = std::make_shared<VoxelTreeNode>();
  (*node->begin())->setParent(node);
  EXPECT_FALSE(node->isLeaf());
  EXPECT_FALSE((*node->begin())->isOrphan());
  EXPECT_FALSE(node->hasAllChildren());
  EXPECT_FALSE(node->hasLinkedChildren());
  node->getChild(2) = std::make_shared<VoxelTreeNode>();
  node->getChild(3) = std::make_shared<VoxelTreeNode>();
  node->getChild(4) = std::make_shared<VoxelTreeNode>();
  node->getChild(5) = std::make_shared<VoxelTreeNode>();
  node->getChild(6) = std::make_shared<VoxelTreeNode>();
  node->getChild(7) = std::make_shared<VoxelTreeNode>();
  EXPECT_TRUE(node->hasAllChildren());
  EXPECT_FALSE(node->hasLinkedChildren());
  EXPECT_EQ(node->getSize(), 2);
  node->getChild(4)->setParent(node);
  EXPECT_EQ(node->getChild(4)->getCenter(), Vect3f(-0.5,-0.5,0.5));
  EXPECT_EQ(node->getChild(4)->getDepth(), 1);
  node->clearChilds();
}
