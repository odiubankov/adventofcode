#include "2021/day_18/snailfish.h"

#include <gtest/gtest.h>

TEST(Snailfish, explode1) {
    SnailfishNumber snailfish_number{"[[[[[9,8],1],2],3],4]"};

    const auto explode_res = snailfish_number.explode();

    ASSERT_TRUE(explode_res);
    ASSERT_EQ(snailfish_number.get_number(), "[[[[0,9],2],3],4]");
}

TEST(Snailfish, explode2) {
    SnailfishNumber snailfish_number{"[7,[6,[5,[4,[3,2]]]]]"};

    const auto explode_res = snailfish_number.explode();

    ASSERT_TRUE(explode_res);
    ASSERT_EQ(snailfish_number.get_number(), "[7,[6,[5,[7,0]]]]");
}

TEST(Snailfish, explode3) {
    SnailfishNumber snailfish_number{"[[6,[5,[4,[3,2]]]],1]"};

    const auto explode_res = snailfish_number.explode();

    ASSERT_TRUE(explode_res);
    ASSERT_EQ(snailfish_number.get_number(), "[[6,[5,[7,0]]],3]");
}

TEST(Snailfish, explode4) {
    SnailfishNumber snailfish_number{"[[3,[2,[1,[7,3]]]],[6,[5,[4,[3,2]]]]]"};

    const auto explode_res = snailfish_number.explode();

    ASSERT_TRUE(explode_res);
    ASSERT_EQ(snailfish_number.get_number(), "[[3,[2,[8,0]]],[9,[5,[4,[3,2]]]]]");
}

TEST(Snailfish, explode5) {
    SnailfishNumber snailfish_number{"[[3,[2,[8,0]]],[9,[5,[4,[3,2]]]]]"};

    const auto explode_res = snailfish_number.explode();

    ASSERT_TRUE(explode_res);
    ASSERT_EQ(snailfish_number.get_number(), "[[3,[2,[8,0]]],[9,[5,[7,0]]]]");
}

TEST(Snailfish, addition) {
    SnailfishNumber snailfish_number_a{"[[[[4,3],4],4],[7,[[8,4],9]]]"};
    SnailfishNumber snailfish_number_b{"[1,1]"};

    SnailfishNumber result = snailfish_number_a + snailfish_number_b;

    ASSERT_EQ(result.get_number(), "[[[[[4,3],4],4],[7,[[8,4],9]]],[1,1]]");
}

TEST(Snailfish, explode6) {
    SnailfishNumber snailfish_number{"[[[[[4,3],4],4],[7,[[8,4],9]]],[1,1]]"};

    const auto explode_res = snailfish_number.explode();

    ASSERT_TRUE(explode_res);
    ASSERT_EQ(snailfish_number.get_number(), "[[[[0,7],4],[7,[[8,4],9]]],[1,1]]");
}

TEST(Snailfish, explode7) {
    SnailfishNumber snailfish_number{"[[[[0,7],4],[7,[[8,4],9]]],[1,1]]"};

    const auto explode_res = snailfish_number.explode();

    ASSERT_TRUE(explode_res);
    ASSERT_EQ(snailfish_number.get_number(), "[[[[0,7],4],[15,[0,13]]],[1,1]]");
}

TEST(Snailfish, explode8) {
    SnailfishNumber snailfish_number{"[[[[0,7],4],[15,[0,13]]],[1,1]]"};

    const auto explode_res = snailfish_number.explode();

    ASSERT_FALSE(explode_res);
}

TEST(Snailfish, split1) {
    SnailfishNumber snailfish_number{"[[[[0,7],4],[15,[0,13]]],[1,1]]"};

    const auto split_res = snailfish_number.split();

    ASSERT_TRUE(split_res);
    ASSERT_EQ(snailfish_number.get_number(), "[[[[0,7],4],[[7,8],[0,13]]],[1,1]]");
}

TEST(Snailfish, reduce) {
    SnailfishNumber snailfish_number{"[[[[[4,3],4],4],[7,[[8,4],9]]],[1,1]]"};

    snailfish_number.reduce();

    ASSERT_EQ(snailfish_number.get_number(), "[[[[0,7],4],[[7,8],[6,0]]],[8,1]]");
}

TEST(Snailfish, magnitude1) {
    SnailfishNumber snailfish_number{"[9,1]"};

    const auto magnitude = snailfish_number.get_magnitude();

    ASSERT_EQ(magnitude, 29ull);
}

TEST(Snailfish, magnitude2) {
    SnailfishNumber snailfish_number{"[[9,1],[1,9]]"};

    const auto magnitude = snailfish_number.get_magnitude();

    ASSERT_EQ(magnitude, 129ull);
}
