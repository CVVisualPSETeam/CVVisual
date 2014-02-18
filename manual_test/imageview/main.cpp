#include <sstream>

#include <QWidget>
#include <QApplication>

#include <opencv2/core/core.hpp>

#include "../../src/view/image_view.hpp"

int main(int argc, char *argv[])
{

	QApplication vc{argc, argv};

	/* Create some data: */
	cv::Mat im{100,100,CV_8U};

	cvv::view::ImageView v{im};

	v.show();
	vc.exec();
	return 0;
}
