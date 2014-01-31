#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>

#include <QApplication>
/*
#include <QHBoxLayout>
#include <QLabel>
#include <QWidget>
*/

#include "../../src/view/dual_filter_view.hpp"

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);

	cv::Mat whitemat = cv::Mat{100,100,CV_8U};
	cv::Mat blackmat = cv::Mat{100,100,CV_8U};

	whitemat = cv::Scalar(255,255,255);
	blackmat = cv::Scalar(0,0,0);

	std::vector<cv::Mat> imgVector {whitemat, blackmat};

	cvv::view::DualFilterView view{imgVector};
	view.setWindowTitle("Dual Filter View Test");
	view.show();
	return a.exec();
}
