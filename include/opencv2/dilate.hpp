#ifndef CVVISUAL_DEBUG_DILATE_HPP
#define CVVISUAL_DEBUG_DILATE_HPP

#include <string>

#include "opencv2/core/core.hpp"

#include "call_meta_data.hpp"
#include "debug_mode.hpp"

namespace cvv {


namespace impl {
	//implementation outside API
	void debugDilate(cv::InputArray in, cv::OutputArray out, CallMetaData data);
} // namespace impl

#ifdef CVVISUAL_DEBUGMODE
static inline void debugDilate(cv::InputArray in, cv::OutputArray out,
		impl::CallMetaData data = impl::CallMetaData()){
	if(debugMode()) {
		impl::debugDilate(in, out, data);
	}
}
#else
static inline void debugDilate(cv::InputArray, cv::OutputArray,
		impl::CallMetaData = impl::CallMetaData()){}
#endif

} // namespace cvv


#endif
