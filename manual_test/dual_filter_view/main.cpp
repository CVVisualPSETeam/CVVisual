#include <iostream>

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>

#include <QApplication>
#include <QHBoxLayout>
#include <QLabel>
#include <QWidget>

#include "../../src/impl/init.hpp"
#include "../../src/view/dual_filter_view.hpp"
#include "../../src/view/singlefilterview.hpp"


int main(int argc, char *argv[])
{
	QApplication a(argc, argv);

	if(argc < 2)
	{
		std::cerr << "Only execute this with filenames of images as arguments!" << std::endl;
		return -1;
	}
	auto src = cv::imread(argv[1]);
	auto elem = cv::getStructuringElement(cv::MORPH_RECT, cv::Size(9, 9), cv::Point(4, 4));
	cv::Mat dest;

	cv::dilate(src, dest, elem);

	cvv::impl::initializeFilterAndViews();
	std::array<cv::Mat, 2> inArray {src, dest};
	cvv::view::DualFilterView view{inArray};
	view.setWindowTitle("Dual Filter View Test");
	view.show();

	return a.exec();
}
