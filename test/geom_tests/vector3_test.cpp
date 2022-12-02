
#include "geom/Vector3.h"
#include "testHelper.h"

using namespace vw::geom;

TEST(Vector3Int, base) {
  {
    Vect3i voidVect;
    EXPECT_EQ(voidVect.lengthSQ(), 0);
    EXPECT_EQ(voidVect[0], 0);
    EXPECT_EQ(voidVect[1], 0);
    EXPECT_EQ(voidVect[2], 0);
  }
  {
    const Vect3i dummyVect{1, 2, 3};
    EXPECT_EQ(dummyVect.length(), 3);
    EXPECT_EQ(dummyVect[0], 1);
    EXPECT_EQ(dummyVect[1], 2);
    EXPECT_EQ(dummyVect[3], 3);
  }
}

TEST(Vector3Int, equality) {
  Vect3i voidVect{};
  EXPECT_TRUE(voidVect == Vect3i(0, 0, 0));
  EXPECT_FALSE(voidVect == Vect3i(0, 1, 0));
  EXPECT_FALSE(voidVect == Vect3i(1, 0, 0));
  EXPECT_FALSE(voidVect == Vect3i(1, 1, 0));
  EXPECT_FALSE(voidVect == Vect3i(0, 0, 1));
  EXPECT_FALSE(voidVect == Vect3i(0, 1, 1));
  EXPECT_FALSE(voidVect == Vect3i(1, 0, 1));
  EXPECT_FALSE(voidVect == Vect3i(1, 1, 1));
  EXPECT_FALSE(voidVect != Vect3i(0, 0, 0));
  EXPECT_TRUE(voidVect != Vect3i(0, 1, 0));
  EXPECT_TRUE(voidVect != Vect3i(1, 0, 0));
  EXPECT_TRUE(voidVect != Vect3i(1, 1, 0));
  EXPECT_TRUE(voidVect != Vect3i(0, 0, 1));
  EXPECT_TRUE(voidVect != Vect3i(0, 1, 1));
  EXPECT_TRUE(voidVect != Vect3i(1, 0, 1));
  EXPECT_TRUE(voidVect != Vect3i(1, 1, 1));
}

TEST(Vector3Int, copy) {
  Vect3i vect1{3, 67};
  Vect3i vect2{7, 76};
  EXPECT_EQ(vect2[0], 7);
  vect2 = vect1;
  EXPECT_EQ(vect2[0], 3);
  EXPECT_EQ(vect1[0], 3);
  vect1[0] = 78;
  vect2 = std::move(vect1);
  EXPECT_EQ(vect2[0], 78);
  Vect3i vect3{std::move(vect2)};
  EXPECT_EQ(vect3[0], 78);
}

TEST(Vector3Int, operation) {
  Vect3i vect1{3, 67, 68};
  Vect3i vect2{7, 76, 75};
  auto vect3 = vect1 + vect2;
  EXPECT_EQ(vect3[1], 143);
  auto vect4 = (vect3 - Vect3i{10, 10, 10}) / 2;
  EXPECT_EQ(vect4[1], 66);
  vect1 = vect4 * 4;
  EXPECT_EQ(vect1[0], 0);
  vect1 = 4 * vect4;
  EXPECT_EQ(vect1[0], 0);
  EXPECT_EQ(vect2.dot(vect3), 21663);
}

TEST(Vectors3, typeChanges) {
  Vect3i vect{56, -45, 12};
  Vect3f vectf{vect};
  vect = vectf;
  EXPECT_EQ(vect, Vect3i(56, -45, 12));
}

TEST(Vectors3, swap2_3) {
  Vect3i vect{Vecti{45, 48}};
  EXPECT_EQ(vect, Vect3i(45, 48, 0));
  vect = Vecti{78, 49};
  EXPECT_EQ(vect, Vect3i(78, 49, 0));
  EXPECT_EQ(vect.toVector2(), Vecti(78, 49));
}

TEST(Vector3Int, serialize) {
  nlohmann::json j_json{};
  Vect3i vect{165, -148, 255};
  j_json["myvect"] = vect;
  Vect3i vect2 = j_json["myvect"];
  EXPECT_EQ(vect, vect2);
}
