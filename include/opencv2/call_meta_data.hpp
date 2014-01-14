#ifndef CVVISUAL_CALL_DATA_HPP
#define CVVISUAL_CALL_DATA_HPP

#include <string>
#include <cstddef>
#include <utility>

namespace cvv {
namespace impl {

struct CallMetaData {
public:
	CallMetaData(): file(), line(0), function(), isKnown(false) {}
	CallMetaData(const std::string file, size_t line, const std::string function) :
		file(std::move(file)), line(line), function(std::move(function)), isKnown(true) {}
	operator bool() {
		return isKnown;
	}
	
	const std::string file;
	const size_t line;
	const std::string function;
	bool isKnown;
};

}} //namespaces

#ifdef __GNUC__
#define CVVISUAL_FUNCTION_NAME_MACRO __PRETTY_FUNCTION__
#else
#define CVVISUAL_FUNCTION_NAME_MACRO __func__
#endif

#define CVVISUAL_LOCATION ::cvv::impl::CallMetaData(__FILE__, __LINE__, CVVISUAL_FUNCTION_NAME_MACRO)

#endif
