#include <vector>

#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/features2d/features2d.hpp>

#include "show_image.hpp"
#include "final_show.hpp"
#include "filter.hpp"


int main(int argc, char** argv)
{
	std::vector<cv::Mat> images;
	for(int i=1; i < argc; ++i)
	{
		auto img = cv::imread(argv[i]);
		cvv::showImage(img, CVVISUAL_LOCATION, argv[i]);
		auto elem = cv::getStructuringElement(cv::MORPH_RECT, cv::Size(9, 9), cv::Point(4, 4));
		cv::Mat filteredImg;
		cv::morphologyEx(img, filteredImg, cv::MORPH_GRADIENT, elem );
		cvv::debugFilter(img, filteredImg, CVVISUAL_LOCATION, argv[i]);
		images.emplace_back(img);
	}
	cvv::finalShow();
}
