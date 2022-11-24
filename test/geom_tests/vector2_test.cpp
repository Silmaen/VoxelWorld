
#include "math/geometry/Vector2.h"
#include "testHelper.h"
using namespace rc::math::geometry;

TEST(VectorInt, base) {
    {
        Vecti voidVect;
        EXPECT_EQ(voidVect.lengthSQ(), 0);
        EXPECT_EQ(voidVect[0], 0);
        EXPECT_EQ(voidVect[1], 0);
    }
    {
        const Vecti dummyVect{1,2};
        EXPECT_EQ(dummyVect.length(), 2);
        EXPECT_EQ(dummyVect[0], 1);
        EXPECT_EQ(dummyVect[1], 2);
    }
}

TEST(VectorInt, copy){
    Vecti vect1{3, 67};
    Vecti vect2{7, 76};
    EXPECT_EQ(vect2[0], 7);
    vect2 = vect1;
    EXPECT_EQ(vect2[0], 3);
    EXPECT_EQ(vect1[0], 3);
    vect1[0] = 78;
    vect2 = std::move(vect1);
    EXPECT_EQ(vect2[0], 78);
    Vecti vect3{std::move(vect2)};
    EXPECT_EQ(vect3[0], 78);
}

TEST(VectorInt, rotation) {
    {
        Vecti dummyVect{4,2};
        auto angle = dummyVect.getAngle();
        angle.get() *= -1;
        Vecti rot = dummyVect.rotated(angle);
        EXPECT_EQ(rot[1], 0);
    }
}

TEST(VectorInt, operation) {
    Vecti vect1{3, 67};
    Vecti vect2{7, 76};
    auto vect3 = vect1+vect2;
    EXPECT_EQ(vect3[1], 143);
    auto vect4 = (vect3 - Vecti{10,10}) / 2;
    EXPECT_EQ(vect4[1], 66);
    vect1 = vect4 * 4;
    EXPECT_EQ(vect1[0], 0);
    EXPECT_EQ(vect2.dot(vect3), 10938);
}

TEST(Vectors, typeChanges){
    Vecti vect{56,-45};
    Vectf vectf{vect};
    vect = vectf;
    EXPECT_EQ(vect,Vecti(56,-45));
}

TEST(VectorFloat, rotation90) {
    {
        Vectf dummyVect{4,2};
        Vectf rot = dummyVect.rotated90();
        EXPECT_NEAR(rot.dot(dummyVect), 0, 0.001);
        EXPECT_NEAR(rot[0], 2, 0.001);
        EXPECT_NEAR(rot[1], -4, 0.001);
    }
}
