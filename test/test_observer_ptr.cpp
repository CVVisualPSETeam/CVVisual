//#include <opencv2/ts/ts.hpp>
#include <gtest/gtest.h>

#include "../src/util/observer_ptr.hpp"

class ObserverPtrTest: public testing::Test{};

using cvv::util::ObserverPtr;

TEST_F(ObserverPtrTest, ConstructionAssignment)
{
	ObserverPtr<int> ptr = nullptr;
	EXPECT_EQ(true, ptr.isNull());
	int x = 3;
	ptr = x;
	EXPECT_EQ(false, ptr.isNull());
	EXPECT_EQ(&x, ptr.getPtr());

}
