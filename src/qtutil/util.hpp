#ifndef CVVISUAL_QTUTIL
#define CVVISUAL_QTUTIL

#include <QImage>
#include <QPixmap>
#include <QSet>

#include "opencv2/core/core.hpp"
#include "opencv2/opencv.hpp"

#include <iostream>

namespace cvv { namespace qtutil {

/*
 * @brief Converts an cv::Mat from OpenCV to a QImage.
 */
QImage convertOpenCVMatToQImage(const cv::Mat &mat);

/*
 * @brief Converts an cv::Mat from OpenCV to a QPixmap.
 */
QPixmap convertOpenCVMatToQPixmap(const cv::Mat &mat);

/**
 * @brief Creates a QSet<QString> with the given string as an inherited value.
 */
QSet<QString> createStringSet(QString string);

}}

#endif
