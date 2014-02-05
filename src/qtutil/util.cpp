#include "util.hpp"

#include <algorithm>

#include <opencv/highgui.h>

#include "types.hpp"

namespace cvv {
namespace qtutil {


QSet<QString> createStringSet(QString string)
{
	QSet<QString> set;
	set.insert(string);
	return set;
}

std::pair<bool, QString> typeToQString(const cv::Mat& mat)
{
	QString s{};
	bool b=true;
	switch(mat.depth())
	{
		case CV_8U:  s.append("CV_8U" ); break;
		case CV_8S:  s.append("CV_8S" ); break;
		case CV_16U: s.append("CV_16U"); break;
		case CV_16S: s.append("CV_16S"); break;
		case CV_32S: s.append("CV_32S"); break;
		case CV_32F: s.append("CV_32F"); break;
		case CV_64F: s.append("CV_64F"); break;
		default:
			s.append("DEPTH").append(QString::number(mat.depth()));
			b=false;
	}
	s.append("C").append(QString::number(mat.channels()));
	return{b,s};
}

QString conversionResultToString(const ImageConversionResult& result)
{
	switch(result)
	{
		case ImageConversionResult::SUCCESS :
			return "SUCCESS";
		break;
		case ImageConversionResult::MAT_EMPTY:
			return "Empty Mat";
		break;
		case ImageConversionResult::MAT_NOT_2D:
			return "Unsupported Dimension";
		break;
		case ImageConversionResult::FLOAT_OUT_OF_0_TO_1:
			return "Float values out of range [0,1]";
		break;
		case ImageConversionResult::NUMBER_OF_CHANNELS_NOT_SUPPORTED:
			return "Unsupported number of channels";
		break;
		case ImageConversionResult::MAT_INVALID_SIZE:
			return "Invalid Size";
		break;
		case ImageConversionResult::MAT_UNSUPPORTED_DEPTH:
			return "Unsupported Depth ";
		break;
	}
	return "Unknown result from convert function";
}

// ////////////////////////////////////////////////////////////////////////////////////////////////
//image conversion stuff
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
	static_assert(channels>=1&&channels<=4,"Illegal number of channels");
	QImage static convert(const cv::Mat& mat);
};

template<int depth> struct ImageConverter<depth,1>
{
	QImage static convert(const cv::Mat& mat){
		QImage img{mat.cols,mat.rows,QImage::Format_Indexed8};
		img.setColorTable(colorTable.table);
		uchar* row;
		for(int i=0; i<mat.rows; i++)
		{
			row = img.scanLine(i);
			for(int j=0; j<mat.cols; j++)
			{
				row[j] = convertTo8U<depth>(mat.at<PixelType<depth,1>>(i,j)[0]);
			}
		}
		return img;
	}
};

template<int depth> struct ImageConverter<depth,2>
{
	QImage static convert(const cv::Mat& mat)
	{
		QImage img{mat.cols,mat.rows,QImage::Format_RGB888};
		uchar* row;
		int qimIndex;
		for(int i=0; i<mat.rows; i++)
		{
			row = img.scanLine(i);
			qimIndex=0;
			for(int j=0; j<mat.cols; j++)
			{
				row[qimIndex]   = 0;//r
				row[qimIndex+1] = convertTo8U<depth>(mat.at<PixelType<depth,2>>(i,j)[1]);//g
				row[qimIndex+2] = convertTo8U<depth>(mat.at<PixelType<depth,2>>(i,j)[0]);//b
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
		QImage img{mat.cols,mat.rows,QImage::Format_RGB888};
		uchar* row;
		int qimIndex;
		for(int i=0; i<mat.rows; i++)
		{
			row = img.scanLine(i);
			qimIndex=0;
			for(int j=0; j<mat.cols; j++)
			{
				row[qimIndex]   = convertTo8U<depth>(mat.at<PixelType<depth,3>>(i,j)[2]);//r
				row[qimIndex+1] = convertTo8U<depth>(mat.at<PixelType<depth,3>>(i,j)[1]);//g
				row[qimIndex+2] = convertTo8U<depth>(mat.at<PixelType<depth,3>>(i,j)[0]);//b
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
		QImage img{mat.cols,mat.rows,QImage::Format_ARGB32};
		uchar* row;
		int qimIndex;
		for(int i=0; i<mat.rows; i++)
		{
			row = img.scanLine(i);
			qimIndex=0;
			for(int j=0; j<mat.cols; j++)
			{
				row[qimIndex]   = convertTo8U<depth>(mat.at<PixelType<depth,4>>(i,j)[3]);//a
				row[qimIndex+1] = convertTo8U<depth>(mat.at<PixelType<depth,4>>(i,j)[2]);//r
				row[qimIndex+2] = convertTo8U<depth>(mat.at<PixelType<depth,4>>(i,j)[1]);//g
				row[qimIndex+3] = convertTo8U<depth>(mat.at<PixelType<depth,4>>(i,j)[0]);//b
				qimIndex+=4;
			}
		}
		return img;
	}
};

// ////////////////////////////////////////////////
// checks wheather all pixels are in a given range
template<int depth>
bool checkValueRange(const cv::Mat& mat, DepthType<depth> min, DepthType<depth> max)
{
	std::pair<cv::MatConstIterator_<DepthType<depth>>,
			cv::MatConstIterator_<DepthType<depth>> >
		mm{std::minmax_element(mat.begin<DepthType<depth>>(),
					mat.end<DepthType<depth>>())};

	return  cv::saturate_cast<DepthType<CV_8UC1>>(*(mm.first )) >= min &&
		cv::saturate_cast<DepthType<CV_8UC1>>(*(mm.second)) <= max;
}
// ////////////////////////////////////////////////
//error result
//the error could be printed on an image
//second parameter: maybe more informations are useful
std::pair<ImageConversionResult,QImage> errorResult(ImageConversionResult res, const cv::Mat&)
	{return {res, QImage{0,0,QImage::Format_Invalid}};}

//split depth
template<int channels> std::pair<ImageConversionResult,QImage> convert(const cv::Mat& mat,
								bool skipFloatRangeTest)
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
			if(!skipFloatRangeTest)
			{
				if(!checkValueRange<CV_32F>(mat,
						cv::saturate_cast<DepthType<CV_32F>>(0),
						cv::saturate_cast<DepthType<CV_32F>>(1))
				) //floating depth + in range [0,1]
				{
					return errorResult(
						ImageConversionResult::FLOAT_OUT_OF_0_TO_1, mat);
				}
			}
			return {ImageConversionResult::SUCCESS,
				ImageConverter<CV_32F ,channels>::convert(mat)};
		break;
		case CV_64F:
			if(!skipFloatRangeTest)
			{
				if(!checkValueRange<CV_64F>(mat,
						cv::saturate_cast<DepthType<CV_64F>>(0),
						cv::saturate_cast<DepthType<CV_64F>>(1))
				) //floating depth + in range [0,1]
				{
					return errorResult(
						ImageConversionResult::FLOAT_OUT_OF_0_TO_1, mat);
				}
			}
			return {ImageConversionResult::SUCCESS,
				ImageConverter<CV_64F ,channels>::convert(mat)};
		break;
		default: return errorResult(ImageConversionResult::MAT_UNSUPPORTED_DEPTH, mat);
	}
}


//convert
std::pair<ImageConversionResult,QImage> convertMatToQImage(const cv::Mat &mat,
							bool skipFloatRangeTest)
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
		case 1: return convert<1>(mat,skipFloatRangeTest); break;
		case 2: return convert<2>(mat,skipFloatRangeTest); break;
		case 3: return convert<3>(mat,skipFloatRangeTest); break;
		case 4: return convert<4>(mat,skipFloatRangeTest); break;
		default:
			return errorResult(
				ImageConversionResult::NUMBER_OF_CHANNELS_NOT_SUPPORTED, mat);
	}
	//floating depth + in range [0,1]  (in function convert<T>)
	//depth ok?                        (in function convert<T>)
}


std::pair<ImageConversionResult,QPixmap>  convertMatToQPixmap(const cv::Mat &mat,
							bool skipFloatRangeTest)
{
	auto converted=convertMatToQImage(mat, skipFloatRangeTest);
	return {converted.first, QPixmap::fromImage(converted.second)};
}
}}
