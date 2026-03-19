#include "gtest/gtest.h"
#include <glm/glm.hpp>
#include "mathutils.hpp"

TEST(MathUtilsTest, testBoundingBoxMinDistance) {
    BoundingBox boundingBox(glm::vec3(0,0,0), glm::vec3(1,1,1));

    float dist = boundingBox.minDistanceFromPointSq(glm::vec3(0,0,0));

    EXPECT_EQ(dist, 0.0f);
}

TEST(MathUtilsTest, testBoundingBoxMinDistanceInsideBox) {
    BoundingBox boundingBox(glm::vec3(0,0,0), glm::vec3(4,4,4));

    float dist = boundingBox.minDistanceFromPointSq(glm::vec3(2,2,2));

    EXPECT_EQ(dist, 0.0f);
}

struct BoundingBoxDistanceCase {
    glm::vec3 point;
    float expectedDistSq;
};

class BoundingBoxMinDistanceOutsideTest : public ::testing::TestWithParam<BoundingBoxDistanceCase> {};

TEST_P(BoundingBoxMinDistanceOutsideTest, ReturnsCorrectSquaredDistance) {
    BoundingBox boundingBox(glm::vec3(0,0,0), glm::vec3(1,1,1));
    auto param = GetParam();

    float dist = boundingBox.minDistanceFromPointSq(param.point);

    EXPECT_FLOAT_EQ(dist, param.expectedDistSq);
}

INSTANTIATE_TEST_SUITE_P(OutsideCases, BoundingBoxMinDistanceOutsideTest, ::testing::Values(
    BoundingBoxDistanceCase{glm::vec3( 3, 0, 0), 4.0f},  // (3-1)^2
    BoundingBoxDistanceCase{glm::vec3(-2, 0, 0), 4.0f},  // (-2-0)^2
    BoundingBoxDistanceCase{glm::vec3( 0, 4, 0), 9.0f},  // (4-1)^2
    BoundingBoxDistanceCase{glm::vec3( 0, 0,-3), 9.0f},  // (-3-0)^2
    BoundingBoxDistanceCase{glm::vec3( 2, 2, 2), 3.0f}   // (2-1)^2 * 3 axes
));

TEST(MathUtilsTest, testIntersectSphereSqIntersects) {
    BoundingBox boundingBox(glm::vec3(0,0,0), glm::vec3(1,1,1));

    // Point at (3,0,0): minDistSq=4, radiusSq=4 -> on boundary, intersects
    EXPECT_TRUE(boundingBox.intersectSphereSq(glm::vec3(3,0,0), 4.0f));
}

TEST(MathUtilsTest, testIntersectSphereSqNoIntersect) {
    BoundingBox boundingBox(glm::vec3(0,0,0), glm::vec3(1,1,1));

    // Point at (3,0,0): minDistSq=4, radiusSq=1 -> no intersect
    EXPECT_FALSE(boundingBox.intersectSphereSq(glm::vec3(3,0,0), 1.0f));
}

TEST(MathUtilsTest, testIntersectSphereSqCenterInside) {
    BoundingBox boundingBox(glm::vec3(0,0,0), glm::vec3(4,4,4));

    // Center inside box: minDistSq=0, always intersects
    EXPECT_TRUE(boundingBox.intersectSphereSq(glm::vec3(2,2,2), 0.0f));
}