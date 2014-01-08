#include "Call.hpp"

namespace cvv {
namespace impl {

Call::Call() : callData{}, id{newCallId()}, calltype{} {}

Call::Call(impl::CallData callData, QString type):
		callData{std::move(callData)}, id{newCallId()}, calltype{type} {}

}} // namespaces cvv::impl
