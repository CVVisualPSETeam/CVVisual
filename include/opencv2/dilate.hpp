#ifndef CVVISUAL_DEBUG_DILATE_HPP
#define CVVISUAL_DEBUG_DILATE_HPP

#include <string>

#include "opencv2/core/core.hpp"

#include "call_data.hpp"
#include "debug_mode.hpp"

namespace cvv {


namespace impl {
	//implementation outside API
	void debugDilate(cv::InputArray in, cv::OutputArray out, CallData data);
} // namespace impl

#ifdef CVVISUAL_DEBUGMODE
static inline void debugDilate(cv::InputArrayin, cv::OutputArrayout,
		impl::CallData data = impl::CallData()){
	if(debugMode()) {
		impl::debugDilate(in, out, data);
	}
}
#else
static inline void debugDilate(cv::InputArray, cv::OutputArray,
		impl::CallData = impl::CallData()){}
#endif

} // namespace cvv


#endif
