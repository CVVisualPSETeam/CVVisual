//#include <opencv2/ts/ts.hpp>
#include <gtest/gtest.h>

#include <thread>


#include "debug_mode.hpp"

class DebugFlagTest: public testing::Test{};

TEST_F(DebugFlagTest, SetAndUnsetDebugMode) {
	EXPECT_EQ(cvv::debugMode(), true);
	cvv::setDebugFlag(false);
	EXPECT_EQ(cvv::debugMode(), false);
	cvv::setDebugFlag(true);
	EXPECT_EQ(cvv::debugMode(), true);
}

TEST_F(DebugFlagTest, ParallelDebugMode) {
	EXPECT_EQ(cvv::debugMode(), true);
	std::thread t{[]{
		EXPECT_EQ(cvv::debugMode(), true);
		cvv::setDebugFlag(false);
		EXPECT_EQ(cvv::debugMode(), false);
	}};
	t.join();
	EXPECT_EQ(cvv::debugMode(), true);
}
