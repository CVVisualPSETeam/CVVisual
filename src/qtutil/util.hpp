#ifndef CVVISUAL_QTUTIL_HPP
#define CVVISUAL_QTUTIL_HPP

#include <QImage>
#include <QPixmap>
#include <QSet>

#include "opencv2/core/core.hpp"
#include "opencv2/opencv.hpp"

#include "../dbg/dbg.hpp"

namespace cvv { namespace qtutil {

/**
 * @brief Represents the staus of an image conversion.
 */
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

/**
 * @brief Converts a cv::Mat to a QImage.
 * @param mat The mat to convert.
 * @param skipFloatRangeTest If true Mats with floating types will be checked
 *  wheather all values are within [0,1].
 * @param threads Number of threads to use (0 will use 1 thread).
 * @return The status of the conversion and the converted mat.
 */
std::pair<ImageConversionResult,QImage>convertMatToQImage(const cv::Mat &mat,
						bool skipFloatRangeTest=0, unsigned int threads = 1);

/**
 * @brief Converts a cv::Mat to a QPixmap.
 * @param mat The mat to convert.
 * @param skipFloatRangeTest If true Mats with floating types will be checked
 *  wheather all values are within [0,1].
 * @param threads Number of threads to use (0 will use 1 thread).
 * @return The status of the conversion and the converted mat.
 */
std::pair<ImageConversionResult,QPixmap>  convertMatToQPixmap(const cv::Mat &mat,
						bool skipFloatRangeTest=0, unsigned int threads = 1);

/**
 * @brief Creates a QSet<QString> with the given string as an inherited value.
 */
QSet<QString> createStringSet(QString string);

/**
 * @brief Returns a string containing the type of the mat.
 * @param mat The mat.
 * @return A string containing the type of the mat.
 * (first = flase when the depth is unknown)
 */
std::pair<bool, QString> typeToQString(const cv::Mat& mat);

QString conversionResultToString(const ImageConversionResult& result);
}}

#endif
