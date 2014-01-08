#ifndef CVVISUAL_DEBUG_MODE_HPP
#define CVVISUAL_DEBUG_MODE_HPP

#if __cplusplus >= 201103L
// great, we have C++11-support
#include <atomic>

namespace cvv{
namespace impl {

static inline std::atomic<bool>& getDebugFlag() {
	thread_local static std::atomic<bool> flag{true};
	return flag;
}

} //namespace impl

static inline bool debugMode() {
	return impl::getDebugFlag().load();
}

static inline void setDebugFlag(bool active) {
	impl::getDebugFlag().store(active);
}

} //namespace cvv

#else
// less great: we have to support the stone-age

namespace cvv{
namespace impl {

static inline bool& getDebugFlag() {
	static bool flag;
	return flag;
}

} //namespace impl

static inline bool debugMode() {
	return impl::getDebugFlag();
}

static inline void setDebugFlag(bool active) {
	impl::getDebugFlag() = active;
}

} //namespace cvv

#endif

#endif
