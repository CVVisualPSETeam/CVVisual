#ifndef CVVISUAL_CALL_HPP
#define CVVISUAL_CALL_HPP

#include <utility>

#include <QString>

#include "opencv2/core/core.hpp"
#include "opencv2/features2d/features2d.hpp"

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
	virtual const cv::Mat& matrixAt(size_t index) const = 0;
	
	/**
	 * @brief provides a description of the call.
	 */
	const QString& description() const {return description_;}
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
	// TODO:
	QString description_ = "Not Yet Implemented";
};

}} //namespaces


#endif
