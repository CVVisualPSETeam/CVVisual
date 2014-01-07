
#include "DebugDilate.hpp"
#include "CallData.hpp"


namespace cvv {
namespace impl {

void debugDilate(const ocv::InputArray& in, const ocv::OutputArray& out, const CallData& data) {
	(void) in;
	(void) out;
	(void) data;
	//dataController().addCall(make_unique<FilterCall>(in, out, data, QString{ /* dilate */}));
}

}} //namespaces
