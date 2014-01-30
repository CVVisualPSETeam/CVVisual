#ifndef CVVISUAL_ZOOMABLEIMAGE
#define CVVISUAL_ZOOMABLEIMAGE

#include <QLabel>
#include <QWidget>

#include "opencv2/core/core.hpp"

namespace cvv{ namespace qtutil{
	
	class ZoomableImage:public QWidget
	{

	Q_OBJECT

	public:

		ZoomableImage(const cv::Mat& mat,QWidget* parent = nullptr);

	signals:
		void updateInfo(const QString& str);

	public slots:

		void updateMat(const cv::Mat& mat); 

	private:

		cv::Mat mat_;
		QLabel *label_;

	};
}}
#endif
