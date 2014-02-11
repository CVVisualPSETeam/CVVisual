#include "util.hpp"

#include <algorithm>
#include <stdexcept>
#include <thread>
#include <functional>


#include <opencv/highgui.h>

#include "types.hpp"
#include "../../src/dbg/dbg.hpp"


namespace cvv {
namespace qtutil {


QSet<QString> createStringSet(QString string)
{
	TRACEPOINT;
	QSet<QString> set;
	set.insert(string);
	TRACEPOINT;
	return set;
}

std::pair<bool, QString> typeToQString(const cv::Mat& mat)
{
	TRACEPOINT;
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
	TRACEPOINT;
	return{b,s};
}

QString conversionResultToString(const ImageConversionResult& result)
{
	TRACEPOINT;
	switch(result)
	{
		case ImageConversionResult::SUCCESS :
			TRACEPOINT;
			return "SUCCESS";
		break;
		case ImageConversionResult::MAT_EMPTY:
			TRACEPOINT;
			return "Empty Mat";
		break;
		case ImageConversionResult::MAT_NOT_2D:
			TRACEPOINT;
			return "Unsupported Dimension";
		break;
		case ImageConversionResult::FLOAT_OUT_OF_0_TO_1:
			TRACEPOINT;
			return "Float values out of range [0,1]";
		break;
		case ImageConversionResult::NUMBER_OF_CHANNELS_NOT_SUPPORTED:
			TRACEPOINT;
			return "Unsupported number of channels";
		break;
		case ImageConversionResult::MAT_INVALID_SIZE:
			TRACEPOINT;
			return "Invalid Size";
		break;
		case ImageConversionResult::MAT_UNSUPPORTED_DEPTH:
			TRACEPOINT;
			return "Unsupported Depth ";
		break;
	}
	TRACEPOINT;
	return "Unknown result from convert function";
}

// ////////////////////////////////////////////////////////////////////////////////////////////////
//image conversion stuff
// ////////////////////////////////////////////////
//convert an image with known depth and channels (the number of chanels is the suffix (convertX)
/**
 * @brief Colortable for CV_XXC1
 */
struct ColorTable
{
	/**
	 * @brief Constructor
	 */
	ColorTable():
		table{}
	{
		TRACEPOINT;
		for(int i=0;i<265;i++){table.push_back(qRgb(i,i,i));}
		TRACEPOINT;
	}
	/**
	 * @brief The colortable
	 */
	QVector<QRgb> table;
};
/**
 * @brief Static colortable for CV_XXC1
 */
const static ColorTable colorTable{};

//helper
/**
 * @brief Provides the parts of the conversion fuction that differ depending on the type.
 */
template<int depth, int channels> struct ConvertHelper
{
	static_assert(channels>=1&&channels<=4,"Illegal number of channels");
	QImage image(const cv::Mat& mat);
	void pixelOperation(int i,int j, const cv::Mat& mat, uchar* row);
};

template<int depth> struct ConvertHelper<depth,1>
{
	static QImage image(const cv::Mat& mat)
	{
		TRACEPOINT;
		QImage img{mat.cols,mat.rows,QImage::Format_Indexed8};
		img.setColorTable(colorTable.table);
		TRACEPOINT;
		return img;
	}

	static void pixelOperation(int i,int j, const cv::Mat& mat, uchar* row)
	{
		row[j] = convertTo8U<depth>(mat.at<PixelType<depth,1>>(i,j)[0]);
	}
};

/**
 * @brief Provides the parts of the conversion fuction that differ depending on the type.
 */
template<int depth> struct ConvertHelper<depth,2>
{
	static QImage image(const cv::Mat& mat)
		{TRACEPOINT;return QImage{mat.cols,mat.rows,QImage::Format_RGB888};}

	static void pixelOperation(int i,int j, const cv::Mat& mat, uchar* row)
	{
		row[j*3]   = 0;//r
		row[j*3+1] = convertTo8U<depth>(mat.at<PixelType<depth,2>>(i,j)[1]);//g
		row[j*3+2] = convertTo8U<depth>(mat.at<PixelType<depth,2>>(i,j)[0]);//b
	}
};

/**
 * @brief Provides the parts of the conversion fuction that differ depending on the type.
 */
template<int depth> struct ConvertHelper<depth,3>
{
	static QImage image(const cv::Mat& mat)
		{TRACEPOINT;return QImage{mat.cols,mat.rows,QImage::Format_RGB888};}

	static void pixelOperation(int i,int j, const cv::Mat& mat, uchar* row)
	{
		row[3*j]   = convertTo8U<depth>(mat.at<PixelType<depth,3>>(i,j)[2]);//r
		row[3*j+1] = convertTo8U<depth>(mat.at<PixelType<depth,3>>(i,j)[1]);//g
		row[3*j+2] = convertTo8U<depth>(mat.at<PixelType<depth,3>>(i,j)[0]);//b
	}
};

/**
 * @brief Provides the parts of the conversion fuction that differ depending on the type.
 */
template<int depth> struct ConvertHelper<depth,4>
{
	static QImage image(const cv::Mat& mat)
		{TRACEPOINT;return QImage{mat.cols,mat.rows,QImage::Format_ARGB32};}

	static void pixelOperation(int i,int j, const cv::Mat& mat, uchar* row)
	{
		row[4*j]   = convertTo8U<depth>(mat.at<PixelType<depth,4>>(i,j)[3]);//a
		row[4*j+1] = convertTo8U<depth>(mat.at<PixelType<depth,4>>(i,j)[2]);//r
		row[4*j+2] = convertTo8U<depth>(mat.at<PixelType<depth,4>>(i,j)[1]);//g
		row[4*j+3] = convertTo8U<depth>(mat.at<PixelType<depth,4>>(i,j)[0]);//b
	}
};

/**
 * @brief Converts parts of a cv Mat. [minRow,maxRow)
 * @param mat The mat.
 * @param img The result image.
 * @param minRow Row to start.
 * @param maxRow Last row.
 */
template<int depth, int channels>
void convertPart(const cv::Mat& mat, QImage& img, int minRow, int maxRow)
{
	TRACEPOINT;
	if(minRow==maxRow){return;}
	if(maxRow<minRow) {throw std::invalid_argument{"maxRow<minRow"};}
	if(maxRow>mat.rows) {throw std::invalid_argument{"maxRow>mat.rows"};}
	uchar* row;
	for(int i=minRow; i<maxRow; i++)
	{
		row = img.scanLine(i);
		for(int j=0; j<mat.cols; j++)
		{
			ConvertHelper<depth,channels>::pixelOperation(i,j,mat,row);
		}
	}
	TRACEPOINT;
}

/**
 * @brief Converts a cv Mat.
 * @param mat The mat.
 * @param threads The number of threads to use.
 * @return The converted QImage.
 */
template<int depth, int channels>
QImage convert(const cv::Mat& mat, unsigned int threads)
{
	TRACEPOINT;
	QImage img=ConvertHelper<depth,channels>::image(mat);

	if(threads>1)
	{
		TRACEPOINT;
		//multithreadding
		int nThreads=std::min(threads,std::thread::hardware_concurrency());
		std::vector<std::thread> workerThreads;
		workerThreads.reserve(nThreads);
		int nperthread=mat.rows/nThreads;
		for(std::size_t i=0;i<nThreads;i++)
		{
			workerThreads.emplace_back(
						convertPart<depth,channels>,mat,std::ref(img),
						i*nperthread,
						i*nperthread+nperthread);
		}
		//there may be some rows left
		convertPart<depth,channels>(mat,img,nperthread*nThreads,mat.rows);

		TRACEPOINT;
		//join
		for(auto& t:workerThreads)
		{
			t.join();
		}

	}else{
		TRACEPOINT;
		convertPart<depth,channels>(mat,img,0,mat.rows);
	}
	TRACEPOINT;
	return img;
}



// ////////////////////////////////////////////////
/**
 * @brief Checks wheather all channels of each pixel are in the given range.
 * @param mat The Mat.
 * @param min Minimal value
 * @param max Maximal value
 * @return Wheather all channels of each pixel are in the given range.
 */
template<int depth>
bool checkValueRange(const cv::Mat& mat, DepthType<depth> min, DepthType<depth> max)
{
	TRACEPOINT;
	std::pair<cv::MatConstIterator_<DepthType<depth>>,
			cv::MatConstIterator_<DepthType<depth>> >
		mm{std::minmax_element(mat.begin<DepthType<depth>>(),
					mat.end<DepthType<depth>>())};

	TRACEPOINT;
	return  cv::saturate_cast<DepthType<CV_8UC1>>(*(mm.first )) >= min &&
		cv::saturate_cast<DepthType<CV_8UC1>>(*(mm.second)) <= max;
}
// ////////////////////////////////////////////////
//error result
//the error could be printed on an image
//second parameter: maybe more informations are useful
/**
 * @brief Creates the error result for a given error.
 * @param res The error code.
 * @return The result.
 */
std::pair<ImageConversionResult,QImage> errorResult(ImageConversionResult res, const cv::Mat&)
{
	TRACEPOINT;
	return {res, QImage{0,0,QImage::Format_Invalid}};
}

//split depth
/**
 * @brief Converts a given image. (this step splits according to the depth)
 * @param mat The Mat.
 * @param skipFloatRangeTest Wheather a rangecheck for float images will be performed.
 * @param threads The number of threads to use.
 * @return The converted QImage.
 */
template<int channels>
std::pair<ImageConversionResult,QImage> convert(const cv::Mat& mat,bool skipFloatRangeTest,
						unsigned int threads)
{
	TRACEPOINT;
	//depth ok?
	switch(mat.depth())
	{
		case CV_8U:
		TRACEPOINT;
			return {ImageConversionResult::SUCCESS,
				convert<CV_8U  ,channels>(mat,threads)};
		break;
		case CV_8S:
		TRACEPOINT;
			return {ImageConversionResult::SUCCESS,
				convert<CV_8S  ,channels>(mat,threads)};
		break;
		case CV_16U:
		TRACEPOINT;
			return {ImageConversionResult::SUCCESS,
				convert<CV_16U ,channels>(mat,threads)};
		break;
		case CV_16S:
		TRACEPOINT;
			return {ImageConversionResult::SUCCESS,
				convert<CV_16S ,channels>(mat,threads)};
		break;
		case CV_32S:
		TRACEPOINT;
			return {ImageConversionResult::SUCCESS,
				convert<CV_32S ,channels>(mat,threads)};
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
			TRACEPOINT;
			return {ImageConversionResult::SUCCESS,
				convert<CV_32F ,channels>(mat,threads)};
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
			TRACEPOINT;
			return {ImageConversionResult::SUCCESS,
				convert<CV_64F ,channels>(mat,threads)};
		break;
		default:
			TRACEPOINT;
			return errorResult(ImageConversionResult::MAT_UNSUPPORTED_DEPTH, mat);
	}
	TRACEPOINT;
}


//convert
/*
 * @brief Converts a given image. (this step splits according to the channels)
 * @param mat The Mat.
 * @param skipFloatRangeTest Wheather a rangecheck for float images will be performed.
 * @param threads The number of threads to use.
 * @return The converted QImage.
 */
std::pair<ImageConversionResult,QImage> convertMatToQImage(const cv::Mat &mat,
						bool skipFloatRangeTest, unsigned int threads)
{
	TRACEPOINT;
	//empty?
	if(mat.empty())
	{
		TRACEPOINT;
		return errorResult(ImageConversionResult::MAT_EMPTY, mat);
	};

	//2d?
	if(mat.dims != 2)
	{
		TRACEPOINT;
		return errorResult(ImageConversionResult::MAT_NOT_2D, mat);
	};

	//size ok
	if(mat.rows < 1 || mat.cols < 1)
	{
		TRACEPOINT;
		return errorResult(ImageConversionResult::MAT_INVALID_SIZE, mat);
	}

	//check channels 1-4
	//now convert
	switch(mat.channels())
	{
		case 1:
			TRACEPOINT;
			return convert<1>(mat,skipFloatRangeTest,threads);
		break;
		case 2:
			TRACEPOINT;
			return convert<2>(mat,skipFloatRangeTest,threads);
		break;
		case 3:
			TRACEPOINT;
			return convert<3>(mat,skipFloatRangeTest,threads);
		break;
		case 4:
			TRACEPOINT;
			return convert<4>(mat,skipFloatRangeTest,threads);
		break;
		default:
			TRACEPOINT;
			return errorResult(ImageConversionResult::NUMBER_OF_CHANNELS_NOT_SUPPORTED,
						mat);
	}
	//floating depth + in range [0,1]  (in function convert<T>)
	//depth ok?						(in function convert<T>)
	TRACEPOINT;
}


std::pair<ImageConversionResult,QPixmap>  convertMatToQPixmap(const cv::Mat &mat,
						bool skipFloatRangeTest, unsigned int threads)
{
	TRACEPOINT;
	auto converted=convertMatToQImage(mat, skipFloatRangeTest,threads);
	TRACEPOINT;
	return {converted.first, QPixmap::fromImage(converted.second)};
}
}}
