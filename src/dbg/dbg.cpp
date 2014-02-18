#include <atomic>
#include <chrono>
#include <ctime>
#include <fstream>
#include <iomanip>
#include <ios>
#include <iostream>

#include "dbg.hpp"

namespace cvv {namespace dbg {

namespace {
	std::atomic_bool loggingState{true};
}

bool getLoggingState()
{
	return loggingState.load();
}

void setLoggingState(bool b) {
	loggingState.store(b);
}

void printLocation(std::ostream& stream, const Location& loc) {
	stream << "[“" << loc.file << "”, " << loc.line << ": " << loc.function << "]";
}

namespace impl {

void log(const Location& loc, const std::string msg) {
	std::stringstream stream;
	
	printLocation(stream, loc);
	
	stream << ": ";
	stream << msg;
	stream.put('\n');
	
	std::cout << stream.str();
}

} // namespace impl

}} // namespace cvv::dbg
