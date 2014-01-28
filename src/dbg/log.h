#ifndef CVVISUAL_LOG_H
#define CVVISUAL_LOG_H

#include <sstream>
#include <string>

#include "string_builder.h"

#ifdef NDEBUG
#	define DEBUG(...) do{}while(false)
#	define DEBUGF(...) do{}while(false)
#	define TRACEPOINT do{}while(false)
#else
#	define DEBUG(...) ::cvv::dbg::log({__FILE__, __PRETTY_FUNCTION__, __LINE__}, __VA_ARGS__)
#	define DEBUGF(...) ::cvv::dbg::logF({__FILE__, __PRETTY_FUNCTION__, __LINE__},__VA_ARGS__)
#	define TRACEPOINT ::cvv::dbg::log({__FILE__, __PRETTY_FUNCTION__, __LINE__}, ::cvv::dbg::TRACE, "tracepoint")
#endif

namespace cvv{ namespace dbg {

struct Location {
	const char* file;
	const char* function;
	const int line;
};

typedef size_t Priority;
static const Priority DEBUG = 0;
static const Priority TRACE = 100;

Priority getPriority();
void setPriority(Priority p);


namespace impl {
void log(const Location& loc, Priority p, const std::string msg);
} //namespace impl

template<typename...T>
void log(const Location& loc, Priority p, const T&...args) {
	if(p <= getPriority()) {
		std::stringstream stream;
		printToStream(stream, args...);
		impl::log(loc, p, stream.str());
	}
}

template<typename...T>
void logF(const Location& loc, Priority p, const std::string& format, const T&...args) {
	if(p <= getPriority()) {
		std::stringstream stream;
		printToStreamF(stream, format, args...);
		impl::log(loc, p, stream.str());
	}
}


}} // namespace cvv::dbg

#endif
