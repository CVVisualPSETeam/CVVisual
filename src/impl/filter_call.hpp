#ifndef CVVISUAL_FILTER_CALL_HPP
#define CVVISUAL_FILTER_CALL_HPP

#include "call.hpp"

#include "opencv2/core/core.hpp"

namespace cvv { namespace impl {

/**
 * All data of a filter-call: Location, original image and result.
 */
class FilterCall: public Call {
public:
	FilterCall(cv::InputArray in, cv::InputArray out, impl::CallMetaData data, const QString& type);
	
	/**
	 * @returns the original image
	 */
	cv::InputArray original() const {return input_;}
	/**
	 * @returns the filtered image
	 */
	cv::InputArray result() const {return output_;}
	
private:
	//TODO: in case we REALLY want to support several input-images: make this a std::vector
	//TODO: those are typedefs for references, make it clean:
	cv::InputArray input_;
	cv::InputArray output_;
};


}} //namespaces

#endif
