#include <iostream>

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

#include <QApplication>
#include <QImage>

#include "../../src/view/singlefilterview.hpp"
#include "../../src/impl/init.hpp"

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	
	if (argc < 2)
	{
		std::cerr << "Only execute this with filenames of images as "
		             "arguments!" << std::endl;
		return -1;
	}
	auto src = cv::imread(argv[1]);
	auto elem = cv::getStructuringElement(cv::MORPH_RECT, cv::Size(9, 9),
	                                      cv::Point(4, 4));
	cv::Mat dest;

	cv::dilate(src, dest, elem);

	cvv::impl::initializeFilterAndViews();
	std::vector<cv::Mat> vec{ src, dest };
	cvv::view::SingleFilterView view{ vec };
	view.setWindowTitle("Dual Filter View Test");
	view.show();
	/*
	std::vector<cv::Mat> imagelist;

	cv::Mat whitemat = cv::Mat{ 100, 100, CV_8UC3 };
	cv::Mat blackmat = cv::Mat{ 100, 100, CV_8UC3 };
	cv::Mat colormat1 = cv::Mat{ 100, 100, CV_8UC3 };
	cv::Mat colormat2 = cv::Mat{ 100, 100, CV_8UC3 };
	cv::Mat colormat3 = cv::Mat{ 100, 100, CV_8UC3 };

	whitemat = cv::Scalar(255, 255, 255);
	blackmat = cv::Scalar(0, 0, 0);
	colormat1 = cv::Scalar(255, 0, 0);
	colormat2 = cv::Scalar(0, 255, 0);
	colormat3 = cv::Scalar(0, 0, 255);

	imagelist.push_back(whitemat);
	imagelist.push_back(blackmat);
	imagelist.push_back(colormat1);
	imagelist.push_back(colormat2);
	imagelist.push_back(colormat3);
	*/
	
	cvv::view::SingleFilterView dfv{vec};
	dfv.setWindowTitle("Single Filter View Test");
	dfv.show();
	return a.exec();
}
