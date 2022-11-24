
#include "math/geometry/Angle.h"
#include "testHelper.h"

using namespace rc::math::geometry;

TEST(Angle, base){
    EXPECT_DOUBLE_EQ(Angle::getPi(), 3.1415926535897931);
    Angle angle;
    EXPECT_DOUBLE_EQ(angle.get(), 0);
    EXPECT_DOUBLE_EQ(angle.getUnit(Angle::Unit::Degree), 0);
    EXPECT_DOUBLE_EQ(angle.getUnit(Angle::Unit::Radian), 0);
}

TEST(Angle, conversions){
    {
        const Angle angle{Angle::getPi() / 2.0, Angle::Unit::Radian};
        EXPECT_DOUBLE_EQ(angle.get(), 1.5707963267948968);
        EXPECT_DOUBLE_EQ(angle.getUnit(Angle::Unit::Degree), 90);
        EXPECT_DOUBLE_EQ(angle.getUnit(Angle::Unit::Radian), 1.5707963267948968);
    }
    {
        Angle angle{60, Angle::Unit::Degree};
        EXPECT_DOUBLE_EQ(angle.get(), 1.0471975511965976);
        EXPECT_DOUBLE_EQ(angle.getUnit(Angle::Unit::Degree), 60);
        EXPECT_DOUBLE_EQ(angle.getUnit(Angle::Unit::Radian), 1.0471975511965976);
    }
}

TEST(Angle, clamps){
    {
        Angle angle{710, Angle::Unit::Degree};
        angle.clamp();
        EXPECT_NEAR(angle.getUnit(Angle::Unit::Degree), 350, 1e-13);
    }
    {
        Angle angle{710, Angle::Unit::Degree};
        angle.clampSigned();
        EXPECT_NEAR(angle.getUnit(Angle::Unit::Degree), -10, 1e-13);
    }
    {
        Angle angle{-710, Angle::Unit::Degree};
        angle.clamp();
        EXPECT_NEAR(angle.getUnit(Angle::Unit::Degree), 10, 1e-13);
    }
    {
        Angle angle{-710, Angle::Unit::Degree};
        angle.clampSigned();
        EXPECT_NEAR(angle.getUnit(Angle::Unit::Degree), 10, 1e-13);
    }
}
