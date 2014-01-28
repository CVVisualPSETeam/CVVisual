#include "filter_call.hpp"

namespace cvv { namespace impl {


FilterCall::FilterCall(cv::InputArray in, cv::InputArray out, impl::CallMetaData data, const QString& type):
	Call(data, type), input_(in.getMat()), output_(out.getMat()) {}


const cv::Mat& FilterCall::matrixAt(size_t index) const {
	switch(index) {
		case 0:
			return original();
		case 1:
			return result();
		default:
			throw std::out_of_range{""};
	}
}

}} // namespaces cvv::impl
