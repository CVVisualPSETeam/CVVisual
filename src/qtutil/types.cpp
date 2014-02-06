
#include "types.hpp"

namespace cvv { namespace qtutil {

//type conversion
template<> uchar convertTo8U<CV_8U>(const DepthType<CV_8U> value)
	{return value;}

template<> uchar convertTo8U<CV_16U>(const DepthType<CV_16U> value)
	{return cv::saturate_cast<DepthType<CV_8U>>(value/256);}

template<> uchar convertTo8U<CV_16S>(const DepthType<CV_16S> value)
	{return convertTo8U<CV_8U>((value/256)+128);}

template<> uchar convertTo8U<CV_8S>(const DepthType<CV_8S> value)
	{return convertTo8U<CV_16S>(cv::saturate_cast<DepthType<CV_16S>>(value)*256);}

template<> uchar convertTo8U<CV_32S>(const DepthType<CV_32S> value)
	{return convertTo8U<CV_16S>(cv::saturate_cast<DepthType<CV_16S>>(value/(256*256)));}

template<> uchar convertTo8U<CV_32F>(const DepthType<CV_32F> value)
	{return cv::saturate_cast<DepthType<CV_8U>>(value*256.0);}

template<> uchar convertTo8U<CV_64F>(const DepthType<CV_64F> value)
	{return cv::saturate_cast<DepthType<CV_8U>>(value*256.0);}
}}//namespaces
