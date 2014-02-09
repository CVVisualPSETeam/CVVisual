#include <iostream>

#include <QApplication>

#include "opencv2/core/core.hpp"
#include "opencv2/features2d/features2d.hpp"
#include <opencv2/highgui/highgui.hpp>

#include "../../src/qtutil/matchscene.hpp"

int main(int argc, char** argv)
{
	QApplication a(argc, argv);

	if(argc != 3)
	{
		std::cerr << "Only execute this test with filenames of two images as arguments! \n";
		return -1;
	}
	auto src 	= cv::imread(argv[1]);
	auto train 	= cv::imread(argv[2]);
	std::vector<cv::KeyPoint> key1;

	for(int i=0;i<std::min(src.rows,src.cols);i+=30)
	{
			cv::Point2f pt{static_cast<float>(i),static_cast<float>(i)};
			key1.emplace_back(pt,0.0f);
	}
	std::vector<cv::KeyPoint> key2;
	for(int i=0;i<std::min(train.rows,train.cols);i+=30)
	{
		cv::Point2f pt{static_cast<float>(i),static_cast<float>(i)};
		key2.emplace_back(pt,0.0f);
	}
	std::vector<cv::DMatch> match;
	DEBUGF(0,"#match %s \n",match.size());
	for(size_t i=0;i<std::min(key1.size(),key2.size());i++)
	{
		match.emplace_back(i,i,1.0f);
	}
	std::cout<<"start scene\n";
	std::cout.flush();
	cvv::qtutil::MatchScene *matchscene = new cvv::qtutil::MatchScene{src,train,key1, key2,match};
	matchscene->setWindowTitle("MatchScene Test");
	matchscene->show();
	return a.exec();
	
}
