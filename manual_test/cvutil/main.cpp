#include "../../src/qtutil/cvutil.hpp"

#include <QApplication>
#include <QLabel>
#include <QPixmap>
#include <QImage>

#include "opencv2/core/core.hpp"
#include "opencv2/opencv.hpp"

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);

	cv::Mat m{100,200,CV_8UC3, cv::Scalar(127)};

	QLabel w{};

	QPixmap p{};
	p.convertFromImage(cvv::qtutil::convertMat2QImage(m));

	w.setPixmap(p);

	w.show();
	return a.exec();
}

