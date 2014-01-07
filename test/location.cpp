#include <gtest/gtest.h>

#include "CallData.hpp"

class LocationTest: public testing::Test{};

TEST_F(LocationTest, FileLine) {
	auto locationMacroResult = CVVISUAL_LOCATION;
	auto line = __LINE__ - 1;
	auto file = __FILE__;
	EXPECT_EQ(locationMacroResult.isKnown, true);
	EXPECT_EQ(locationMacroResult.file, file);
	EXPECT_EQ(locationMacroResult.line, line);
}

TEST_F(LocationTest, EmptyLocation) {
	cvv::impl::CallData loc{};
	EXPECT_EQ(loc.isKnown, false);
}
