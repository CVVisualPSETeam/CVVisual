#ifndef CVVISUAL_FINAL_SHOW_HPP
#define CVVISUAL_FINAL_SHOW_HPP

#include "debug_mode.hpp"

namespace cvv {

namespace impl {
	void finalShow();
}

inline void finalShow() {
#ifdef CVVISUAL_DEBUGMODE
	if(debugMode()) {
		impl::finalShow();
	}
#endif
}

}

#endif
