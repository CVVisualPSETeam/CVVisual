#ifndef CVVISUAL_CALL_DATA_HPP
#define CVVISUAL_CALL_DATA_HPP

#include <string>
#include <cstddef>
#include <utility>

namespace cvv {
namespace impl {

struct CallMetaData {
public:
	CallMetaData() = default;
	CallMetaData(const std::string file, size_t line, const std::string function) :
		file(std::move(file)), line(line), function(std::move(function)), isKnown(true) {}
	operator bool() {
		return isKnown;
	}
	
	const std::string file = "";
	const size_t line = 0;
	const std::string function ="";
	bool isKnown = false;
};

}} //namespaces

#ifdef __GNUC__
#define CVVISUAL_FUNCTION_NAME_MACRO __PRETTY_FUNCTION__
#else
#define CVVISUAL_FUNCTION_NAME_MACRO __func__
#endif

#define CVVISUAL_LOCATION ::cvv::impl::CallMetaData(__FILE__, __LINE__, CVVISUAL_FUNCTION_NAME_MACRO)

#endif
