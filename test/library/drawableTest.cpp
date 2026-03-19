#include "gtest/gtest.h"
#include <glm/glm.hpp>
#include "drawable.hpp"
#include "basetypes.hpp"

TEST(DrawableTest, testInit) {
    Drawable drawable;

    EXPECT_EQ(drawable.getPosition(), glm::vec3(0,0,0));
}

TEST(DrawableTest, testInitScale) {
    Drawable drawable;

    EXPECT_EQ(drawable.getScale(), glm::vec3(1,1,1));
}

TEST(DrawableTest, testInitModelMatrix) {
    Drawable drawable;

    EXPECT_EQ(drawable.getModelMatrix(), glm::mat4(1.0f));
}

TEST(DrawableTest, testInitType) {
    Drawable drawable;

    EXPECT_EQ(drawable.type(), ShaderType::SHADER_TYPE_DEFAULT);
}

TEST(DrawableTest, testSetType) {
    Drawable drawable;
    drawable.setType(ShaderType::SHADER_TYPE_TERRAIN);

    EXPECT_EQ(drawable.type(), ShaderType::SHADER_TYPE_TERRAIN);
}

TEST(DrawableTest, testSetPosition) {
    Drawable drawable;
    glm::vec3 pos(3.0f, 5.0f, 7.0f);
    drawable.setPosition(pos);

    EXPECT_EQ(drawable.getPosition(), pos);
}

TEST(DrawableTest, testTransformTranslation) {
    Drawable drawable;
    glm::vec3 translate(1.0f, 2.0f, 3.0f);
    drawable.transform(nullptr, &translate, nullptr);

    EXPECT_EQ(drawable.getPosition(), translate);
}

TEST(DrawableTest, testTransformScale) {
    Drawable drawable;
    glm::vec3 scale(2.0f, 3.0f, 4.0f);
    drawable.transform(&scale, nullptr, nullptr);

    EXPECT_EQ(drawable.getScale(), scale);
}

TEST(DrawableTest, testUpdateInstanceSize) {
    Drawable drawable;
    drawable.updateInstanceSize(3);

    EXPECT_EQ(drawable.getPosition(0), glm::vec3(0,0,0));
    EXPECT_EQ(drawable.getPosition(1), glm::vec3(0,0,0));
    EXPECT_EQ(drawable.getPosition(2), glm::vec3(0,0,0));
}

TEST(DrawableTest, testUpdateInstanceSizeMinOne) {
    Drawable drawable;
    drawable.updateInstanceSize(0);

    // size 0 is clamped to 1
    EXPECT_EQ(drawable.getPosition(0), glm::vec3(0,0,0));
}