#include "call.hpp"

namespace cvv {
namespace impl {

size_t newCallId()
{
	thread_local size_t nextId = 0;
	return nextId++;
}

Call::Call() : metaData_{}, id{newCallId()}, calltype{} {}

Call::Call(impl::CallMetaData callData, QString type, QString description, QString requestedView):
		metaData_{std::move(callData)}, id{newCallId()}, calltype{std::move(type)},
		description_{std::move(description)}, requestedView{std::move(requestedView)} {}

}} // namespaces cvv::impl
