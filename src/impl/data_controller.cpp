#include "data_controller.hpp"

#include <stdexcept>

namespace cvv { namespace impl {

DataController::DataController()
{
	(void) viewController;
}


void DataController::addCall(std::unique_ptr<Call> call)
{
	calls.push_back(std::move(call));
}

void DataController::removeCall(size_t Id)
{
	auto it = std::find_if(calls.begin(), calls.end(),
			[=](const std::unique_ptr<Call>& call){return call->getId() == Id;});
	if(it == calls.end())
	{
		throw std::invalid_argument{"there is no call with this id"};
	}
	calls.erase(it);
	
}

const Call& DataController::getCall(size_t Id) const
{
	return *calls.at(Id);
}

Call& DataController::getCall(size_t Id)
{
	return *calls.at(Id);
}

size_t DataController::numCalls() const
{
	return calls.size();
}

/**
 * @todo implement
 */
void DataController::callUI()
{
	
}

DataController& dataController()
{
	thread_local static DataController controller{};
	return controller;
}
}} // namespaces cvv::impl
