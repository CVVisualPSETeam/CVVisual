#ifndef CVVISUAL_FINAL_SHOW_HPP
#define CVVISUAL_FINAL_SHOW_HPP

#include "debug_mode.hpp"

namespace cvv {

namespace impl {
	void finalShow();
}

/**
 * @brief Passes the control to the debug-window for a last time.
 * 
 * This function must be called EXACTLY once if there was any prior debug-call.
 * 
 * If there was no prior call it may be called once in which case it does no harm.
 */
inline void finalShow() {
#ifdef CVVISUAL_DEBUGMODE
	if(debugMode()) {
		impl::finalShow();
	}
#endif
}

}

#endif
