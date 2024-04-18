/* Copyright (C) Denys Valchuk - All Rights Reserved
 * ZHZhbGNodWtAZ21haWwuY29tCg==
 */

#include <pidcoll.h>
#include <gtest/gtest.h>

TEST(FactorialTest, Zero) {
  EXPECT_EQ(1, dv::pidcoll::factorial(0));
}

TEST(FactorialTest, Positive) {
  EXPECT_EQ(1, dv::pidcoll::factorial(1));
  EXPECT_EQ(2, dv::pidcoll::factorial(2));
  EXPECT_EQ(6, dv::pidcoll::factorial(3));
  EXPECT_EQ(40320, dv::pidcoll::factorial(8));
}

TEST(FactorialTest, Negative) {
  EXPECT_THROW(dv::pidcoll::factorial(-1), std::invalid_argument);
}
