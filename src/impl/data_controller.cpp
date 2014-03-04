#include "data_controller.hpp"

#include "../dbg/dbg.hpp"

#include <stdexcept>

namespace cvv
{
namespace impl
{

namespace
{
class CallEquality
{
      public:
	CallEquality(size_t Id) : Id{ Id }
	{
	}

	bool operator()(const std::unique_ptr<Call> &call) const
	{
		TRACEPOINT;
		return call->getId() == Id;
	}

      private:
	size_t Id;
};
}

void DataController::addCall(std::unique_ptr<Call> call)
{
	TRACEPOINT;
	auto ref = util::makeRef(*call);
	calls.push_back(std::move(call));
	viewController.addCall(ref);
	callUI();
	TRACEPOINT;
}

void DataController::removeCall(size_t Id)
{
	TRACEPOINT;
	auto it = std::find_if(calls.begin(), calls.end(), CallEquality{ Id });
	if (it == calls.end())
	{
		throw std::invalid_argument{ "there is no call with this id" };
	}
	calls.erase(it);
	TRACEPOINT;
}

const Call &DataController::getCall(size_t Id) const
{
	TRACEPOINT;
	auto it = std::find_if(calls.begin(), calls.end(), CallEquality{ Id });
	if (it == calls.end())
	{
		throw std::invalid_argument{ "there is no call with this id" };
	}
	TRACEPOINT;
	return **it;
}

Call &DataController::getCall(size_t Id)
{
	TRACEPOINT;
	auto it = std::find_if(calls.begin(), calls.end(), CallEquality{ Id });
	if (it == calls.end())
	{
		throw std::invalid_argument{ "there is no call with this id" };
	}
	TRACEPOINT;
	return **it;
}

bool DataController::hasCall(size_t Id)
{
	TRACEPOINT;
	auto it = std::find_if(calls.begin(), calls.end(), CallEquality{ Id });
	TRACEPOINT;
	return it != calls.end();
}

size_t DataController::numCalls() const
{
	TRACEPOINT;
	return calls.size();
}

void DataController::callUI()
{
	TRACEPOINT;
	viewController.exec();
	TRACEPOINT;
}

void DataController::lastCall()
{
	TRACEPOINT;
	viewController.showExitProgramButton();
	TRACEPOINT;
	callUI();
	TRACEPOINT;
}

/**
 * @brief Actual implementation of the global DataController.
 *
 * This is required to be able to delete it.
 */
static std::unique_ptr<DataController> &realSingleton()
{
	static std::unique_ptr<DataController> var = nullptr;
	return var;
}

void deleteDataController()
{
	realSingleton().reset();
}

DataController &dataController()
{
	auto& controller = realSingleton();
	if(!realSingleton().get())
	{
		controller = util::make_unique<DataController>();
	}
	return *controller;
}
}
} // namespaces cvv::impl
