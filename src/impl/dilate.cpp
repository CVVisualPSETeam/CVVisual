
#include "dilate.hpp"

#include "opencv2/core/core.hpp"

#include <QString>

#include "../util/util.hpp"

#include "call.hpp"
#include "call_data.hpp"
#include "data_controller.hpp"
#include "filter_call.hpp"

namespace cvv {
namespace impl {

void debugDilate(cv::InputArray in, cv::OutputArray out, const CallData& data) {
	dataController().addCall(util::make_unique<FilterCall>(in, out, data, QString{"dilate"}));
}

}} //namespaces
