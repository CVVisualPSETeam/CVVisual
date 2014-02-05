#include "erode.hpp"

#include "call_meta_data.hpp"
#include "filter_call.hpp"

namespace cvv {
namespace impl {

void debugErode(cv::InputArray original, cv::InputArray result, const CallMetaData& data,
		const char* description, const char* view)
{
	debugFilterCall(original, result, data, description, view, "erode");
}

}} //namespaces
