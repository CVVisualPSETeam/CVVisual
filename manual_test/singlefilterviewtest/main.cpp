#include <QApplication>
#include <QImage>

#include <iostream>

#include "../../src/view/singlefilterview.hpp"

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	//TODO Filter<1,1> einfügen
	
	std::vector<cv::Mat> imagelist;
	
	cv::Mat whitemat = cv::Mat{100,100,CV_8U};
	cv::Mat blackmat = cv::Mat{100,100,CV_8U};
	cv::Mat colormat1 =cv::Mat{100,100,CV_8U};
	cv::Mat colormat2 =cv::Mat{100,100,CV_8U};
	cv::Mat colormat3 =cv::Mat{100,100,CV_8U};

	whitemat = cv::Scalar(255,255,255);
	blackmat = cv::Scalar(0,0,0);
	colormat1 =cv::Scalar(255,0,0);
	colormat2 = cv::Scalar(0,255,0);
	colormat3 = cv::Scalar(0,0,255);

	imagelist.push_back(whitemat);
	imagelist.push_back(blackmat);
	imagelist.push_back(colormat1);
	imagelist.push_back(colormat2);
	imagelist.push_back(colormat3);
	
	cvv::view::SingleFilterView dfv{imagelist,nullptr};

	dfv.show();
	return a.exec();
}
