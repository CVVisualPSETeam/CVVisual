#ifndef CVVISUAL_ZOOMABLEIMAGE
#define CVVISUAL_ZOOMABLEIMAGE

#include <QLabel>
#include <QWidget>

#include "opencv2/core/core.hpp"

#include "util.hpp"

namespace cvv{ namespace qtutil{

class ZoomableImage:public QWidget
{
Q_OBJECT
public:
	ZoomableImage(const cv::Mat& mat=cv::Mat{},QWidget* parent = nullptr);

signals:
	void conversionResult(ImageConversionResult);

public slots:
	void updateMat(cv::Mat mat);

private:
	cv::Mat mat_;
	QLabel *label_;

	};
}}
#endif
