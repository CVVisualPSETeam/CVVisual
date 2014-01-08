#include "call.hpp"

namespace cvv {
namespace impl {

size_t newCallId()
{
	thread_local size_t nextId = 0;
	return nextId++;
}

Call::Call() : callData{}, id{newCallId()}, calltype{} {}

Call::Call(impl::CallMetaData callData, QString type):
		callData{std::move(callData)}, id{newCallId()}, calltype{type} {}

}} // namespaces cvv::impl
