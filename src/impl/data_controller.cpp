#include "data_controller.hpp"

#include "../dbg/dbg.hpp"

#include <stdexcept>

namespace cvv { namespace impl {

namespace {
	class CallEquality {
	public:
		CallEquality(size_t Id): Id{Id} {}
		
		bool operator()(const std::unique_ptr<Call>& call) const
		{
			return call->getId() == Id;
		}
	private:
		size_t Id;
	};
}

void DataController::addCall(std::unique_ptr<Call> call)
{
	auto ref = util::makeRef(*call);
	calls.push_back(std::move(call));
	viewController.addCall(ref);
	callUI();
}

void DataController::removeCall(size_t Id)
{
	auto it = std::find_if(calls.begin(), calls.end(), CallEquality{Id});
	if(it == calls.end())
	{
		throw std::invalid_argument{"there is no call with this id"};
	}
	calls.erase(it);
}

const Call& DataController::getCall(size_t Id) const
{
	auto it = std::find_if(calls.begin(), calls.end(), CallEquality{Id});
	if(it == calls.end())
	{
		throw std::invalid_argument{"there is no call with this id"};
	}
	return **it;
}

Call& DataController::getCall(size_t Id)
{
	auto it = std::find_if(calls.begin(), calls.end(), CallEquality{Id});
	if(it == calls.end())
	{
		throw std::invalid_argument{"there is no call with this id"};
	}
	return **it;
}

bool DataController::hasCall(size_t Id)
{
    auto it = std::find_if(calls.begin(), calls.end(), CallEquality{Id});
	return it != calls.end();
}

size_t DataController::numCalls() const
{
	return calls.size();
}

void DataController::callUI()
{
	viewController.exec();
}

void DataController::lastCall()
{
	viewController.showExitProgramButton();
	callUI();
}


DataController& dataController()
{
	static DataController controller{};
	return controller;
}
}} // namespaces cvv::impl
