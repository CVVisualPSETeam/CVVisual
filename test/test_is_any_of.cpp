//#include <opencv2/ts/ts.hpp>
#include <gtest/gtest.h>

#include "../src/util/util.hpp"

class IsAnyOfTest: public testing::Test{};

using cvv::util::isAnyOf;

TEST_F(IsAnyOfTest, InitializerListIntInt)
{
	EXPECT_EQ(isAnyOf(3, {1,2,3,4}), true);
	EXPECT_EQ(isAnyOf(3, {1,2,4}), false);
}

TEST_F(IsAnyOfTest, InitializerListLongInt)
{
	EXPECT_EQ(isAnyOf(3, {1L,2L,3L,4L}), true);
	EXPECT_EQ(isAnyOf(3, {1L,2L,4L}), false);
}

TEST_F(IsAnyOfTest, VectorIntInt)
{
	EXPECT_EQ(isAnyOf(3, std::vector<int>{1,2,3,4}), true);
	EXPECT_EQ(isAnyOf(3, std::vector<int>{1,2,4}), false);
}

TEST_F(IsAnyOfTest, VectorLongInt)
{
	EXPECT_EQ(isAnyOf(3, std::vector<long>{1,2,3,4}), true);
	EXPECT_EQ(isAnyOf(3, std::vector<long>{1,2,4}), false);
}
