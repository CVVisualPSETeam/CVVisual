#include <QApplication>
#include <QImage>

#include <iostream>

#include "../../src/view/defaultfilterview.hpp"

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	
	std::vector<cv::Mat> imagelist;
	
	cv::Mat whitemat = cv::Mat{100,100,CV_8U};
	cv::Mat blackmat = cv::Mat{100,100,CV_8U};

	whitemat = cv::Scalar(255,255,255);
	blackmat=cv::Scalar(0,0,0);

	imagelist.push_back(whitemat);
	imagelist.push_back(blackmat);
	
	cvv::view::DefaultFilterView dfv{imagelist,nullptr};

	dfv.show();
	return a.exec();
}
