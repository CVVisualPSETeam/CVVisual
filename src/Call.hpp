#ifndef CVVISUAL_CALL_HPP
#define CVVISUAL_CALL_HPP

#include <utility>

#include <QString>

#include "CallData.hpp"

namespace cvv {
namespace impl {

size_t newCallId() {
	thread_local size_t nextId = 0;
	return nextId++;
}

class Call {
public:
	virtual ~Call() = default;
	size_t getId() { return id; }
	
	const QString& type() const {return calltype;}
protected:
	Call();
	Call(impl::CallData callData, QString type);
	
	impl::CallData callData;
	size_t id;
	QString calltype;
};

}} //namespaces


#endif
