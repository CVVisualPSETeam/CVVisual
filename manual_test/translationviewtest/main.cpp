#include <iostream>

#include <QApplication>

#include "opencv2/core/core.hpp"
#include "opencv2/features2d/features2d.hpp"
#include <opencv2/highgui/highgui.hpp>

#include "../../src/view/translationsmatchview.hpp"

int main(int argc, char** argv)
{
	QApplication a(argc, argv);

	cvv::dbg::setLoggingState(false);
	if(argc != 3)
	{
		std::cerr << "Only execute this test with filenames of two images as arguments! \n";
		return -1;
	}
	auto src 	= cv::imread(argv[1]);
	auto train 	= cv::imread(argv[2]);
	std::vector<cv::KeyPoint> key1;

	for(int i=std::min(src.rows,src.cols);i>0;i-=30)
	{
		cv::Point2f pt{static_cast<float>(std::min(src.rows,src.cols)-i),static_cast<float>(i)};
		key1.emplace_back(pt,0.0f);
	}
	std::vector<cv::KeyPoint> key2;
	for(int i=0;i<std::min(train.rows,train.cols);i+=30)
	{
		cv::Point2f pt{static_cast<float>(i),static_cast<float>(i)};
		key2.emplace_back(pt,0.0f);
	}
	std::vector<cv::DMatch> match;
	for(size_t i=0;i<std::min(key1.size()-1,key2.size()-1);i++)
	{
		match.emplace_back(i,i+1,1.0f);
	}

	cvv::view::TranslationMatchView view{key1, key2,match,src,train};
	view.setWindowTitle("TranslationMatchView Test");
	view.show();
	return a.exec();

}
