#include "data_controller.hpp"


namespace cvv { namespace impl {

DataController::DataController()
{
	(void) windowController;
}



void DataController::removeCall(size_t Id) {
	// TODO: implement
	(void) Id;
}

const Call& DataController::getCall(size_t Id) const {
	return *calls.at(Id);
}

Call& DataController::getCall(size_t Id)
{
	return *calls.at(Id);
}

void DataController::callUI()
{
	// TODO: implement
}

}} // namespaces cvv::impl
