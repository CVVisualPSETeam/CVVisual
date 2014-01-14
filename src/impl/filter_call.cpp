#include "filter_call.hpp"

namespace cvv { namespace impl {


FilterCall::FilterCall(cv::InputArray in, cv::InputArray out, impl::CallMetaData data, const QString& type):
	Call(data, type), input_(in.getMat()), output_(out.getMat()) {}

}} // namespaces cvv::impl
