#include "gtest/gtest.h"
#include <glm/glm.hpp>
#include "colorgenerator.hpp"

TEST(ColorGeneratorTest, testGenColorsReturnsCorrectSize) {
    ColorGenerator gen;
    std::vector<float> heights = {0.0f, 0.5f, -0.5f, 1.0f};

    auto colors = gen.genColors(heights, 2);

    EXPECT_EQ(colors.size(), heights.size());
}

TEST(ColorGeneratorTest, testGenColorsAlphaIsOne) {
    ColorGenerator gen;
    std::vector<float> heights = {0.0f, 0.5f, -0.5f, 1.0f};

    auto colors = gen.genColors(heights, 2);

    for (const auto &c : colors) {
        EXPECT_FLOAT_EQ(c.a, 1.0f);
    }
}

TEST(ColorGeneratorTest, testGenColorsRGBInNormalizedRange) {
    ColorGenerator gen;
    std::vector<float> heights = {0.0f, 0.5f, -0.5f, 1.0f};

    auto colors = gen.genColors(heights, 2, 1.0f);

    for (const auto &c : colors) {
        EXPECT_GE(c.r, 0.0f);
        EXPECT_LE(c.r, 1.0f);
        EXPECT_GE(c.g, 0.0f);
        EXPECT_LE(c.g, 1.0f);
        EXPECT_GE(c.b, 0.0f);
        EXPECT_LE(c.b, 1.0f);
    }
}

TEST(ColorGeneratorTest, testUniformPaletteProducesConstantColor) {
    glm::vec4 uniformColor(128, 128, 128, 1);
    std::vector<glm::vec4> palette = {uniformColor, uniformColor};
    ColorGenerator gen(palette, 0.5f);
    std::vector<float> heights = {0.0f, 0.5f, -0.5f, 0.3f};

    auto colors = gen.genColors(heights, 2, 1.0f);

    float expected = 128.0f / 255.0f;
    for (const auto &c : colors) {
        EXPECT_NEAR(c.r, expected, 0.001f);
        EXPECT_NEAR(c.g, expected, 0.001f);
        EXPECT_NEAR(c.b, expected, 0.001f);
        EXPECT_FLOAT_EQ(c.a, 1.0f);
    }
}

TEST(ColorGeneratorTest, testCustomPaletteInterpolation) {
    // Black to white palette
    glm::vec4 black(0, 0, 0, 1);
    glm::vec4 white(255, 255, 255, 1);
    std::vector<glm::vec4> palette = {black, white};
    ColorGenerator gen(palette, 1.0f);

    // Height=0 with amplitude=1: value maps to (0+1)/2=0.5
    // After clamping: (0.5 - 0.5)/1.0 = 0 -> blend=0 -> returns black
    std::vector<float> heights = {0.0f, 0.0f, 0.0f, 0.0f};
    auto colors = gen.genColors(heights, 2, 1.0f);

    for (const auto &c : colors) {
        EXPECT_FLOAT_EQ(c.a, 1.0f);
        EXPECT_GE(c.r, 0.0f);
        EXPECT_LE(c.r, 1.0f);
    }
}

TEST(ColorGeneratorTest, testGenColorsDeterministic) {
    ColorGenerator gen;
    std::vector<float> heights = {0.0f, 0.25f, 0.5f, 0.75f};

    auto colors1 = gen.genColors(heights, 2, 1.0f);
    auto colors2 = gen.genColors(heights, 2, 1.0f);

    ASSERT_EQ(colors1.size(), colors2.size());
    for (size_t i = 0; i < colors1.size(); i++) {
        EXPECT_FLOAT_EQ(colors1[i].r, colors2[i].r);
        EXPECT_FLOAT_EQ(colors1[i].g, colors2[i].g);
        EXPECT_FLOAT_EQ(colors1[i].b, colors2[i].b);
    }
}
