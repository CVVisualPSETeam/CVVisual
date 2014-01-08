
#include "dilate.hpp"
#include "call_data.hpp"


namespace cvv {
namespace impl {

void debugDilate(cv::InputArray in, cv::OutputArray out, const CallData& data) {
	(void) in;
	(void) out;
	(void) data;
	//dataController().addCall(make_unique<FilterCall>(in, out, data, QString{ /* dilate */}));
}

}} //namespaces
