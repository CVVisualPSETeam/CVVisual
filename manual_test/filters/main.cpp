#include <iostream>

#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>

#include "dilate.hpp"
#include "erode.hpp"
#include "sobel.hpp"
#include "morphology_ex.hpp"
#include "final_show.hpp"

void dilateFile(char* filename) {
	auto src = cv::imread(filename);
	auto elem = cv::getStructuringElement(cv::MORPH_RECT, cv::Size(9, 9), cv::Point(4, 4));
	cv::Mat dest;
	
	cv::dilate(src, dest, elem);
	cvv::debugDilate(src, dest, CVVISUAL_LOCATION, filename);
	cv::erode(src, dest, elem);
	cvv::debugErode(src, dest, CVVISUAL_LOCATION, filename);
	cv::Sobel(src, dest, -1, 1, 1);
	cvv::debugSobel(src, dest, CVVISUAL_LOCATION, filename);
	cv::morphologyEx(src, dest,cv::MORPH_GRADIENT, elem );
	cvv::debugMorphologyEx(src, dest, CVVISUAL_LOCATION, filename);
}

int main(int argc, char** argv) {
	if(argc == 1)
	{
		std::cerr << argv[0] << " must be callled with one or more files as arguments\n";
		return 1;
	}
	for(int i=1; i < argc; ++i) {
		dilateFile(argv[i]);
	}
	std::cout << "All calculation done" << std::endl;
	cvv::finalShow();
	std::cout << "Program finished" << std::endl;
}
