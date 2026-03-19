#include "gtest/gtest.h"
#include "perlinnoise.hpp"

TEST(PerlinNoiseTest, testDefaultAmplitude) {
    PerlinNoise noise;

    EXPECT_FLOAT_EQ(noise.getAmplitude(), (float)DEFAULT_AMPLITUDE);
}

TEST(PerlinNoiseTest, testDefaultLowerBound) {
    PerlinNoise noise;

    float expected = 0 - DEFAULT_AMPLITUDE + (DEFAULT_AMPLITUDE / DEFAULT_HEIGHT_OFFSET_FACTOR);
    EXPECT_FLOAT_EQ(noise.getLowerBound(), expected);
}

TEST(PerlinNoiseTest, testDefaultUpperBound) {
    PerlinNoise noise;

    float expected = DEFAULT_AMPLITUDE + (DEFAULT_AMPLITUDE / DEFAULT_HEIGHT_OFFSET_FACTOR);
    EXPECT_FLOAT_EQ(noise.getUpperBound(), expected);
}

TEST(PerlinNoiseTest, testCustomAmplitude) {
    PerlinNoise noise(4, 20.0f, 0.5f, 2);

    EXPECT_FLOAT_EQ(noise.getAmplitude(), 20.0f);
}

TEST(PerlinNoiseTest, testCustomBoundsWithOffset) {
    PerlinNoise noise(4, 20.0f, 0.5f, 2);

    EXPECT_FLOAT_EQ(noise.getLowerBound(), -10.0f);  // -20 + 20/2
    EXPECT_FLOAT_EQ(noise.getUpperBound(), 30.0f);   // 20 + 20/2
}

TEST(PerlinNoiseTest, testCustomBoundsNoOffset) {
    PerlinNoise noise(4, 15.0f, 0.5f, 0);

    EXPECT_FLOAT_EQ(noise.getLowerBound(), -15.0f);
    EXPECT_FLOAT_EQ(noise.getUpperBound(), 15.0f);
}

TEST(PerlinNoiseTest, testNoise2dDeterministic) {
    PerlinNoise noise;

    float v1 = noise.getNoise2d(10, 20);
    float v2 = noise.getNoise2d(10, 20);

    EXPECT_FLOAT_EQ(v1, v2);
}

TEST(PerlinNoiseTest, testNoise3dDeterministic) {
    PerlinNoise noise;

    float v1 = noise.getNoise3d(10, 20, 30);
    float v2 = noise.getNoise3d(10, 20, 30);

    EXPECT_FLOAT_EQ(v1, v2);
}

TEST(PerlinNoiseTest, testNoise2dVariesWithInput) {
    PerlinNoise noise;

    float v1 = noise.getNoise2d(0, 0);
    float v2 = noise.getNoise2d(50, 50);

    EXPECT_NE(v1, v2);
}

TEST(PerlinNoiseTest, testNoise3dVariesWithInput) {
    PerlinNoise noise;

    float v1 = noise.getNoise3d(0, 0, 0);
    float v2 = noise.getNoise3d(50, 50, 50);

    EXPECT_NE(v1, v2);
}

TEST(PerlinNoiseTest, testDifferentSeedsProduceDifferentNoise) {
    PerlinNoise noise1(6, 10.0f, 0.35f, 2, 42);
    PerlinNoise noise2(6, 10.0f, 0.35f, 2, 99);

    float v1 = noise1.getNoise2d(10, 20);
    float v2 = noise2.getNoise2d(10, 20);

    EXPECT_NE(v1, v2);
}

TEST(PerlinNoiseTest, testSameSeedProducesSameNoise) {
    PerlinNoise noise1(6, 10.0f, 0.35f, 2, 42);
    PerlinNoise noise2(6, 10.0f, 0.35f, 2, 42);

    EXPECT_FLOAT_EQ(noise1.getNoise2d(10, 20), noise2.getNoise2d(10, 20));
    EXPECT_FLOAT_EQ(noise1.getNoise3d(5, 15, 25), noise2.getNoise3d(5, 15, 25));
}
