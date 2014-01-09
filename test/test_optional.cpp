//#include <opencv2/ts/ts.hpp>
#include <gtest/gtest.h>

#include "../src/util/optional.hpp"

class OptionalTest: public testing::Test{};

using cvv::util::Optional;

TEST_F(OptionalTest, Construction)
{
	Optional<int> unsetOptional1;
	EXPECT_EQ(unsetOptional1.isSet(), false);
	
	Optional<int> setOptional1{3};
	EXPECT_EQ(setOptional1.isSet(), true);
	EXPECT_EQ(setOptional1.get(), 3);
}

TEST_F(OptionalTest, Operators)
{
	Optional<int> unsetOptional;
	EXPECT_EQ(static_cast<bool>(unsetOptional), false);
	
	const Optional<int> constUnsetOptional;
	EXPECT_EQ(static_cast<bool>(constUnsetOptional), false);
	
	Optional<int> setOptional{3};
	EXPECT_EQ(static_cast<bool>(setOptional), true);
	EXPECT_EQ(*setOptional, 3);
	
	const Optional<int> constSetOptional{3};
	EXPECT_EQ(static_cast<bool>(constSetOptional), true);
	EXPECT_EQ(*constSetOptional, 3);
}
