#ifndef CVVISUAL_QTUTIL
#define CVVISUAL_QTUTIL

#include <QImage>
#include <QPixmap>
#include <QSet>

#include "opencv2/core/core.hpp"
#include "opencv2/opencv.hpp"

namespace cvv { namespace qtutil {

enum class ImageConversionResult
{
	SUCCESS,
	MAT_EMPTY,
	MAT_NOT_2D,
	FLOAT_OUT_OF_0_TO_1,
	NUMBER_OF_CHANNELS_NOT_SUPPORTED,
	MAT_INVALID_SIZE,
	MAT_UNSUPPORTED_DEPTH
};

std::pair<ImageConversionResult,QImage>convertMatToQImage(const cv::Mat &mat,
							bool skipFloatRangeTest=0);


std::pair<ImageConversionResult,QPixmap>  convertMatToQPixmap(const cv::Mat &mat,
							bool skipFloatRangeTest=0);

/**
 * @brief Creates a QSet<QString> with the given string as an inherited value.
 */
QSet<QString> createStringSet(QString string);

}}

#endif
