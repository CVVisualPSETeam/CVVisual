#ifndef CVVISUAL_CALL_HPP
#define CVVISUAL_CALL_HPP

#include <utility>

#include <QString>

#include "call_meta_data.hpp"

namespace cvv {
namespace impl {

size_t newCallId();

class Call {
public:
	virtual ~Call() = default;
	size_t getId() const { return id; }
	
	const QString& type() const {return calltype;}
	
	/**
	 * @brief Returns the number of images that are part of the call.
	 */
	virtual size_t matrixCount() const = 0;
protected:
	Call();
	Call(impl::CallMetaData callData, QString type);
	
	Call(const Call&) = default;
	Call(Call&&) = default;
	
	Call& operator=(const Call&) = default;
	Call& operator=(Call&&) = default;
	
	impl::CallMetaData callData;
	size_t id;
	QString calltype;
};

}} //namespaces


#endif
