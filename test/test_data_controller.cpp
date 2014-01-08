//#include <opencv2/ts/ts.hpp>
#include <gtest/gtest.h>

#include "../src/impl/data_controller.hpp"
#include "../src/impl/call.hpp"
#include "../src/util/util.hpp"

class DataControllerTest: public testing::Test{};

class CallMoc : public cvv::impl::Call {
public:
	CallMoc(size_t test_id) : Call{cvv::impl::CallMetaData{}, "test"}
	{
		id = test_id;
	}
};


TEST_F(DataControllerTest, AddCall) {
	cvv::impl::DataController controller;
	EXPECT_EQ(controller.numCalls(), 0);
	controller.addCall(cvv::util::make_unique<CallMoc>(0));
	EXPECT_EQ(controller.numCalls(), 1);
	EXPECT_EQ(controller.getCall(0).getId(), 0);
}

TEST_F(DataControllerTest, RemoveCall) {
	cvv::impl::DataController controller;
	controller.addCall(cvv::util::make_unique<CallMoc>(0));
	EXPECT_EQ(controller.numCalls(), 1);
	controller.removeCall(0);
	EXPECT_EQ(controller.numCalls(), 0);
}
