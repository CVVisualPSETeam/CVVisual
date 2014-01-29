
#include "dilate.hpp"

#include "opencv2/core/core.hpp"

#include <QString>

#include "call_meta_data.hpp"

#include "../util/util.hpp"

#include "call.hpp"
#include "data_controller.hpp"
#include "filter_call.hpp"

namespace cvv {
namespace impl {

void debugDilate(cv::InputArray original, cv::InputArray result, const CallMetaData& data,
		const char* description, const char* view)
{
	dataController().addCall(util::make_unique<FilterCall>(original, result, data, QString{"dilate"},
			description ? QString::fromLocal8Bit(description) : QString{"<no description>"},
			view ? QString::fromLocal8Bit(view) : QString{}));
}

}} //namespaces
