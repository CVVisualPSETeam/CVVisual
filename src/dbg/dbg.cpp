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
Priority priority = TRACE - 1;
}

Priority getPriority() {return priority;}
void setPriority(Priority p) {priority = p;}


void printLocation(std::ostream& stream, const Location& loc) {
	stream << "[“" << loc.file << "”, " << loc.line << ": " << loc.function << "]";
}

void printPriority(std::ostream& stream, Priority p) {
	if(p >= TRACE) {
		stream << "[Trace(";
	}
	else {
		stream << "[Debug(";
	}
	stream << std::setw(4) << std::setfill(' ') << p << ")]";
}

namespace impl {

void log(const Location& loc, Priority p, const std::string msg) {
	std::stringstream stream;
	
	printPriority(stream, p);
	printLocation(stream, loc);
	
	stream << ": ";
	stream << msg;
	stream.put('\n');
	
	std::cout << stream.str();
}

} // namespace impl

}} // namespace cvv::dbg
