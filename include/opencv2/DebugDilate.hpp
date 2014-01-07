#ifndef CVVISUAL_DEBUG_DILATE_HPP
#define CVVISUAL_DEBUG_DILATE_HPP

#include <string>

#include "CallData.hpp"
#include "DebugMode.hpp"


// FIXME: replace this workaround
namespace ocv {
	class InputArray;
	class OutputArray;
}

namespace cvv {


namespace impl {
	//implementation outside API
	void debugDilate(const ocv::InputArray& in, const ocv::OutputArray& out, CallData data);
} // namespace impl

#ifdef CVVISUAL_DEBUGMODE
static inline void debugDilate(const ocv::InputArray& in, const ocv::OutputArray& out,
		impl::CallData data = impl::CallData()){
	if(debugMode()) {
		impl::debugDilate(in, out, data);
	}
}
#else
static inline void debugDilate(const ocv::InputArray&, const ocv::OutputArray&,
		impl::CallData = impl::CallData()){}
#endif

} // namespace cvv


#endif
