#include "util.hpp"

#include <algorithm>

#include <opencv/highgui.h>

namespace cvv {
namespace qtutil {


QSet<QString> createStringSet(QString string)
{
	QSet<QString> set;
	set.insert(string);
	return set;
}

// ////////////////////////////////////////////////////////////////////////////////////////////////
//image conversion stuff
// ////////////////////////////////////////////////
//depth to type
template<int depth> struct CvvDepthTypeConverter
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
template<> struct CvvDepthTypeConverter<CV_8U >{using type = uint8_t;};
template<> struct CvvDepthTypeConverter<CV_8S >{using type =  int8_t;};
template<> struct CvvDepthTypeConverter<CV_16U>{using type = uint16_t;};
template<> struct CvvDepthTypeConverter<CV_16S>{using type =  int16_t;};
template<> struct CvvDepthTypeConverter<CV_32S>{using type =  int32_t;};
template<> struct CvvDepthTypeConverter<CV_32F>{using type = float;};
template<> struct CvvDepthTypeConverter<CV_64F>{using type = double;};
template<int depth> using CvvDepthType = typename CvvDepthTypeConverter<depth>::type;

// ////////////////////////////////////////////////
//depth and channels to type
template<int depth, int channels> struct CvvPixelTypeConverter
{
	static_assert(channels>=1&&channels<5,"Illegal number of channels");
	using type = cv::Vec<CvvDepthType<depth>,channels>;
};
template<int depth, int channels>
using CvvPixelType = typename CvvPixelTypeConverter<depth,channels>::type;

// ////////////////////////////////////////////////
//convert a depth value to uchar
template<int depth> uchar convertTo8U(const CvvDepthType<depth> value) = delete;

template<> uchar convertTo8U<CV_8UC1>(const CvvDepthType<CV_8UC1> value)
	{return value;}

template<> uchar convertTo8U<CV_16UC1>(const CvvDepthType<CV_16UC1> value)
	{return cv::saturate_cast<CvvDepthType<CV_8UC1>>(value/256);}

template<> uchar convertTo8U<CV_8SC1>(const CvvDepthType<CV_8SC1> value)
{
	CvvDepthType<CV_8UC1> neg{cv::saturate_cast<CvvDepthType<CV_8UC1>>(value < 0? 128:0)}; //if value is negative add 128 bevore cast
	CvvDepthType<CV_8UC1> pos{cv::saturate_cast<CvvDepthType<CV_8UC1>>(value < 0? 0:128)}; //if value is positive add 128 after cast
	return cv::saturate_cast<CvvDepthType<CV_8UC1>>(value + neg) + pos;
}

template<> uchar convertTo8U<CV_16SC1>(const CvvDepthType<CV_16SC1> value)
	{return convertTo8U<CV_8SC1>(cv::saturate_cast<CvvDepthType<CV_8SC1>>(value/256));}

template<> uchar convertTo8U<CV_32SC1>(const CvvDepthType<CV_32SC1> value)
	{return convertTo8U<CV_8SC1>(cv::saturate_cast<CvvDepthType<CV_16SC1>>((value/256)/256));}

template<> uchar convertTo8U<CV_32FC1>(const CvvDepthType<CV_32FC1> value)
	{return cv::saturate_cast<CvvDepthType<CV_8UC1>>(value*256.0);}

template<> uchar convertTo8U<CV_64FC1>(const CvvDepthType<CV_64FC1> value)
	{return cv::saturate_cast<CvvDepthType<CV_8UC1>>(value*256.0);}


// ////////////////////////////////////////////////
//convert an image with known depth and channels (the number of chanels is the suffix (convertX)
struct ColorTable
{
	ColorTable(): table{}
		{for(int i=0;i<265;i++){table.push_back(qRgb(i,i,i));}}
	QVector<QRgb> table;
};
const static ColorTable colorTable{};

template<int depth, int channels> struct ImageConverter
{
	static_assert(channels>=1&&channels<5,"Illegal number of channels");
	QImage static convert(const cv::Mat& mat);
};

template<int depth> struct ImageConverter<depth,1>
{
	QImage static convert(const cv::Mat& mat){
		QImage img{mat.rows,mat.cols,QImage::Format_Indexed8};
		img.setColorTable(colorTable.table);
		uchar* row;
		for(int i=0; i<mat.rows; i++)
		{
			row = img.scanLine(i);
			for(int j=0; j<mat.cols; j++)
			{
				row[j] = convertTo8U<depth>(mat.at<CvvPixelType<depth,1>>(i,j)[0]);
			}
		}
		return img;
	}
};

template<int depth> struct ImageConverter<depth,2>
{
	QImage static convert(const cv::Mat& mat)
	{
		QImage img{mat.rows,mat.cols,QImage::Format_RGB888};
		uchar* row;
		int qimIndex;
		for(int i=0; i<mat.rows; i++)
		{
			row = img.scanLine(i);
			qimIndex=0;
			for(int j=0; j<mat.cols; j++)
			{
				row[qimIndex]   = 0;//r
				row[qimIndex+1] = convertTo8U<depth>(mat.at<CvvPixelType<depth,2>>(i,j)[1]);//g
				row[qimIndex+2] = convertTo8U<depth>(mat.at<CvvPixelType<depth,2>>(i,j)[0]);//b
				qimIndex+=3;
			}
		}
		return img;
	}
};

template<int depth> struct ImageConverter<depth,3>
{
	QImage static convert(const cv::Mat& mat)
	{
		QImage img{mat.rows,mat.cols,QImage::Format_RGB888};
		uchar* row;
		int qimIndex;
		for(int i=0; i<mat.rows; i++)
		{
			row = img.scanLine(i);
			qimIndex=0;
			for(int j=0; j<mat.cols; j++)
			{
				row[qimIndex]   = convertTo8U<depth>(mat.at<CvvPixelType<depth,3>>(i,j)[2]);//r
				row[qimIndex+1] = convertTo8U<depth>(mat.at<CvvPixelType<depth,3>>(i,j)[1]);//g
				row[qimIndex+2] = convertTo8U<depth>(mat.at<CvvPixelType<depth,3>>(i,j)[0]);//b
				qimIndex+=3;
			}
		}
		return img;
	}
};

template<int depth> struct ImageConverter<depth,4>
{
	QImage static convert(const cv::Mat& mat)
	{
		QImage img{mat.rows,mat.cols,QImage::Format_ARGB32};
		uchar* row;
		int qimIndex;
		for(int i=0; i<mat.rows; i++)
		{
			row = img.scanLine(i);
			qimIndex=0;
			for(int j=0; j<mat.cols; j++)
			{
				row[qimIndex]   = convertTo8U<depth>(mat.at<CvvPixelType<depth,4>>(i,j)[3]);//a
				row[qimIndex+1] = convertTo8U<depth>(mat.at<CvvPixelType<depth,4>>(i,j)[2]);//r
				row[qimIndex+2] = convertTo8U<depth>(mat.at<CvvPixelType<depth,4>>(i,j)[1]);//g
				row[qimIndex+3] = convertTo8U<depth>(mat.at<CvvPixelType<depth,4>>(i,j)[0]);//b
				qimIndex+=4;
			}
		}
		return img;
	}
};

// ////////////////////////////////////////////////
// checks wheather all pixels are in a given range
template<int depth>
bool checkValueRange(const cv::Mat& mat, CvvDepthType<depth> min, CvvDepthType<depth> max)
{
	std::pair<cv::MatConstIterator_<CvvDepthType<depth>>,
			cv::MatConstIterator_<CvvDepthType<depth>> >
		mm{std::minmax_element(mat.begin<CvvDepthType<depth>>(),
					mat.end<CvvDepthType<depth>>())};

	return  cv::saturate_cast<CvvDepthType<CV_8UC1>>(*(mm.first )) >= min &&
		cv::saturate_cast<CvvDepthType<CV_8UC1>>(*(mm.second)) <= max;
}
// ////////////////////////////////////////////////
//error result
//the error could be printed on an image
//second parameter: maybe more informations are useful
std::pair<ImageConversionResult,QImage> errorResult(ImageConversionResult res, const cv::Mat&)
	{return {res, QImage{0,0,QImage::Format_Invalid}};}

//split depth
template<int channels> std::pair<ImageConversionResult,QImage> convert(const cv::Mat& mat)
{
	//depth ok?
	switch(mat.depth())
	{
		case CV_8U:
			return {ImageConversionResult::SUCCESS,
				ImageConverter<CV_8U  ,channels>::convert(mat)};
		break;
		case CV_8S:
			return {ImageConversionResult::SUCCESS,
				ImageConverter<CV_8S  ,channels>::convert(mat)};
		break;
		case CV_16U:
			return {ImageConversionResult::SUCCESS,
				ImageConverter<CV_16U ,channels>::convert(mat)};
		break;
		case CV_16S:
			return {ImageConversionResult::SUCCESS,
				ImageConverter<CV_16S ,channels>::convert(mat)};
		break;
		case CV_32S:
			return {ImageConversionResult::SUCCESS,
				ImageConverter<CV_32S ,channels>::convert(mat)};
		break;
		case CV_32F:
			if(!checkValueRange<CV_32F>(mat,
					cv::saturate_cast<CvvDepthType<CV_32F>>(0),
					cv::saturate_cast<CvvDepthType<CV_32F>>(1))
			) //floating depth + in range [0,1]
				{return errorResult(ImageConversionResult::FLOAT_OUT_OF_0_TO_1, mat);};
			return {ImageConversionResult::SUCCESS,
				ImageConverter<CV_32F ,channels>::convert(mat)};
		break;
		case CV_64F:
		if(!checkValueRange<CV_64F>(mat,
				cv::saturate_cast<CvvDepthType<CV_64F>>(0),
				cv::saturate_cast<CvvDepthType<CV_64F>>(1))
		) //floating depth + in range [0,1]
				{return errorResult(ImageConversionResult::FLOAT_OUT_OF_0_TO_1, mat);};
			return {ImageConversionResult::SUCCESS,
				ImageConverter<CV_64F ,channels>::convert(mat)};
		break;
		default: return errorResult(ImageConversionResult::MAT_UNSUPPORTED_DEPTH, mat);
	}
}


//convert
std::pair<ImageConversionResult,QImage> convertMatToQImage(const cv::Mat &mat)
{
	//empty?
	if(mat.empty())
		{return errorResult(ImageConversionResult::MAT_EMPTY, mat);};

	//2d?
	if(mat.dims != 2)
		{return errorResult(ImageConversionResult::MAT_NOT_2D, mat);};

	//size ok
	if(mat.rows < 1 || mat.cols < 1)
		{return errorResult(ImageConversionResult::MAT_INVALID_SIZE, mat);}

	//check channels 1-4
	//now convert
	switch(mat.channels())
	{
		case 1: return convert<1>(mat); break;
		case 2: return convert<2>(mat); break;
		case 3: return convert<3>(mat); break;
		case 4: return convert<4>(mat); break;
		default:
			return errorResult(
				ImageConversionResult::NUMBER_OF_CHANNELS_NOT_SUPPORTED, mat);
	}
	//floating depth + in range [0,1]  (in function convert<T>)
	//depth ok?                        (in function convert<T>)
}


std::pair<ImageConversionResult,QPixmap>  convertMatToQPixmap(const cv::Mat &mat)
{
	auto converted=convertMatToQImage(mat);
	return {converted.first, QPixmap::fromImage(converted.second)};
}
}}
