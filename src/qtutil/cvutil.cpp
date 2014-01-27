#include "cvutil.hpp"


#include <opencv2/highgui/highgui_c.h>

namespace cvv { namespace qtutil {

QImage convertMat2QImage(const cv::Mat& mat)
{
	// similar code used in:
	// window_QT.cpp::DefaultViewPort::updateImage(const CvArr* arr)
	// window_QT.cpp::DefaultViewPort::draw2D(QPainter *painter)

	// Mat::create() are always continuous. we need that!
	CvMat* canvas=cvCreateMat(mat.rows, mat.cols, CV_8UC3);


	//convert
	cvConvertImage(&mat, canvas, CV_CVTIMG_SWAP_RB);
	// maybe fliping is needed: (origin != 0 ? CV_CVTIMG_FLIP : 0) + CV_CVTIMG_SWAP_RB

	// mat.convertTo(transformed, int rtype);//has no swap rb.
	// dont know what will happen with 2 chanel images (qt wants 3)

	//shallow copy + referencecounter++ (data lives as long as copy of transformed lives)
	return {canvas->data.ptr, canvas->cols, canvas->rows,canvas->step,QImage::Format_RGB888,
		[](void* data){delete static_cast<CvMat*>(data);},
		static_cast<void*>(canvas)};
}


}}


