#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>

#include "../../include/opencv2/dilate.hpp"


int main(int argc, char** argv) {
	if(argc != 2)
	{
		return 1;
	}
	auto src = cv::imread(argv[1]);
	auto elem = cv::getStructuringElement(cv::MORPH_RECT, cv::Size(9, 9), cv::Point(4, 4));
	cv::Mat dest;
	cv::dilate(src, dest, elem);
	
	cvv::debugDilate(src, dest, CVVISUAL_LOCATION);
}
