#ifndef CVVISUAL_DEBUG_DILATE_HPP
#define CVVISUAL_DEBUG_DILATE_HPP

#include <string>

#include "opencv2/core/core.hpp"

#include "call_meta_data.hpp"
#include "debug_mode.hpp"

namespace cvv {


namespace impl {
	//implementation outside API
	void debugDilate(cv::InputArray original, cv::InputArray result, const CallMetaData& data, 
			const char* description, const char* view);
} // namespace impl

#ifdef CVVISUAL_DEBUGMODE
static inline void debugDilate(cv::InputArray original, cv::InputArray result,
		impl::CallMetaData data = impl::CallMetaData(), const char* description = nullptr,
		const char* view = nullptr)
{
	if(debugMode()) {
		impl::debugDilate(original, result, data, description, view);
	}
}
#else
static inline void debugDilate(cv::InputArray, cv::InputArray,
		impl::CallMetaData = impl::CallMetaData(), const char* = nullptr,
		const char* = nullptr) {}
#endif

} // namespace cvv


#endif
