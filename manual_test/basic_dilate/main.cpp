#include <iostream>

#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>

#include "../../include/opencv2/dilate.hpp"


void actualWork(char* filename) {
	auto src = cv::imread(filename);
	auto elem = cv::getStructuringElement(cv::MORPH_RECT, cv::Size(9, 9), cv::Point(4, 4));
	cv::Mat dest;
	cv::dilate(src, dest, elem);
	
	cvv::debugDilate(src, dest, CVVISUAL_LOCATION);
	std::cout << "debugDilate returned, local destructors will run now" << std::endl;
}

int main(int argc, char** argv) {
	if(argc != 2)
	{
		return 1;
	}
	actualWork(argv[1]);
	std::cout << "local destructors have run, now all the global one will be executed" << std::endl;
}
