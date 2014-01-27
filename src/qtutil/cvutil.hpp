#ifndef CVVISUAL_QTUTIL
#define CVVISUAL_QTUTIL

#include <QImage>

#include "opencv2/core/core.hpp"
#include "opencv2/opencv.hpp"

#include <iostream>

namespace cvv { namespace qtutil {

/**
 * @brief Converts a Mat into a QImage.
 * @param mat The Mat to convert.
 * @return The converted QImage
 *
 * @todo fix linker error
 */
QImage convertMat2QImage(const cv::Mat& mat);

/*
libcvvisual.a(cvutil.cpp.o): In function `cvv::qtutil::convertMat2QImage(cv::Mat const&)':
/home/user/Schreibtisch/pse/CVVisual/src/qtutil/cvutil.cpp:19: undefined reference to `cvConvertImage'
collect2: Fehler: ld gab 1 als Ende-Status zurück
make[2]: *** [test_manual_cvutil] Fehler 1

//opencv2/highgui/highgui_c.h
zeile 252:
// utility function: convert one image to another with optional vertical flip
CVAPI(void) cvConvertImage( const CvArr* src, CvArr* dst, int flags CV_DEFAULT(0));
*/

}}

#endif
