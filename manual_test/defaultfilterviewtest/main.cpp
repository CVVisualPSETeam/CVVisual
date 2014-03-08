
#include <iostream>

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>

#include <QApplication>
#include <QImage>

#include "../../src/view/defaultfilterview.hpp"

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	
	if (argc < 2)
	{
		std::cerr << "Only execute this with filenames of images as "
		             "arguments!" << std::endl;
		return -1;
	}
	std::vector<cv::Mat> imagelist;
	
	for(int i = 1; i < argc; i++)
	{
		auto src = cv::imread(argv[i]);
		imagelist.push_back(src);
	}
	cvv::view::DefaultFilterView dfv{ imagelist, nullptr };

	dfv.show();
	return a.exec();
}
