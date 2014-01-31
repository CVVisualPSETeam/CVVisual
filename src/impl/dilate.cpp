#include "dilate.hpp"

#include "call_meta_data.hpp"
#include "filter_call.hpp"

namespace cvv {
namespace impl {

void debugDilate(cv::InputArray original, cv::InputArray result, const CallMetaData& data,
		const char* description, const char* view)
{
	debugFilterCall(original, result, data, description, view, "dilate");
}

}} //namespaces
