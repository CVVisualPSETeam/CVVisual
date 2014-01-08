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
protected:
	Call();
	Call(impl::CallMetaData callData, QString type);
	
	impl::CallMetaData callData;
	size_t id;
	QString calltype;
};

}} //namespaces


#endif
