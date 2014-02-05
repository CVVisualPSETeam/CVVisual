#ifndef CVVISUAL_TYPES_HPP
#define CVVISUAL_TYPES_HPP

#include "opencv2/core/core.hpp"

#include "../dbg/dbg.hpp"

namespace cvv { namespace qtutil {

namespace structures{
	//depth to type
	template<int depth> struct DepthTypeConverter
	{
		static_assert(!(
					depth==CV_8U ||
					depth==CV_8S ||
					depth==CV_16U ||
					depth==CV_16S ||
					depth==CV_32S ||
					depth==CV_32F ||
					depth==CV_64F
				),"Conversion of unknown type");
		//using type;
	};
	template<> struct DepthTypeConverter<CV_8U >{using type = uint8_t;};
	template<> struct DepthTypeConverter<CV_8S >{using type = int8_t;};
	template<> struct DepthTypeConverter<CV_16U>{using type = uint16_t;};
	template<> struct DepthTypeConverter<CV_16S>{using type = int16_t;};
	template<> struct DepthTypeConverter<CV_32S>{using type = int32_t;};
	template<> struct DepthTypeConverter<CV_32F>{using type = float;};
	template<> struct DepthTypeConverter<CV_64F>{using type = double;};

	//depth and channels to type
	template<class depthtype, int channels> struct PixelTypeConverter
	{
		static_assert(channels>=1&&channels<=4,"Illegal number of channels");
		using type = cv::Vec<depthtype,channels>;
	};
}

//type conversion
template<int depth> using DepthType = typename structures::DepthTypeConverter<depth>::type;

template<int depth, int channels>
using PixelType = typename structures::PixelTypeConverter<DepthType<depth>,channels>::type;

//convert a depth value to uchar
template<int depth> uchar convertTo8U(const DepthType<depth> value) = delete;

template<> uchar convertTo8U<CV_8U >(const DepthType<CV_8U > value);
template<> uchar convertTo8U<CV_16U>(const DepthType<CV_16U> value);
template<> uchar convertTo8U<CV_16S>(const DepthType<CV_16S> value);
template<> uchar convertTo8U<CV_8S >(const DepthType<CV_8S > value);
template<> uchar convertTo8U<CV_32S>(const DepthType<CV_32S> value);
template<> uchar convertTo8U<CV_32F>(const DepthType<CV_32F> value);
template<> uchar convertTo8U<CV_64F>(const DepthType<CV_64F> value);
}}//namespaces

#endif // TYPES_HPP
