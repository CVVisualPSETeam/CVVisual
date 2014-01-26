#include "call.hpp"

namespace cvv {
namespace impl {

size_t newCallId()
{
	thread_local size_t nextId = 0;
	return nextId++;
}

Call::Call() : metaData_{}, id{newCallId()}, calltype{} {}

Call::Call(impl::CallMetaData callData, QString type):
		metaData_{std::move(callData)}, id{newCallId()}, calltype{type} {}

}} // namespaces cvv::impl
