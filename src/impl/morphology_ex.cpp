#include "morphology_ex.hpp"

#include "call_meta_data.hpp"
#include "filter_call.hpp"

namespace cvv {
namespace impl {

void debugMorphologyEx(cv::InputArray original, cv::InputArray result, const CallMetaData& data,
		const char* description, const char* view)
{
	debugFilterCall(original, result, data, description, view, "morphologyEx");
}

}} //namespaces
