#ifndef CVVISUAL_LOG_H
#define CVVISUAL_LOG_H

#include <sstream>
#include <string>

#include "string_builder.hpp"

#ifdef NDEBUG
#	define DEBUG(...) do{}while(false)
#	define DEBUGF(...) do{}while(false)
#	define TRACEPOINT do{}while(false)
#else
#	define DEBUG(...) ::cvv::dbg::log({__FILE__, __PRETTY_FUNCTION__, __LINE__}, __VA_ARGS__)
#	define DEBUGF(...) ::cvv::dbg::logF({__FILE__, __PRETTY_FUNCTION__, __LINE__},__VA_ARGS__)
#	define TRACEPOINT ::cvv::dbg::log({__FILE__, __PRETTY_FUNCTION__, __LINE__}, "tracepoint")
#endif

namespace cvv{ namespace dbg {

struct Location {
	const char* file;
	const char* function;
	const int line;
};

bool getLoggingState();
void setLoggingState(bool b);


namespace impl {
void log(const Location& loc, const std::string msg);
} //namespace impl

template<typename...T>
void log(const Location& loc, const T&...args) {
	if (getLoggingState()) {
		std::stringstream stream;
		printToStream(stream, args...);
		impl::log(loc, stream.str());
	}
}

template<typename...T>
void logF(const Location& loc, const std::string& format, const T&...args) {
	if (getLoggingState()) {
		std::stringstream stream;
		printToStreamF(stream, format, args...);
		impl::log(loc, stream.str());
	}
}


}} // namespace cvv::dbg

#endif
