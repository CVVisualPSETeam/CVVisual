#include <iostream>

#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>

#include "../../include/opencv2/dilate.hpp"


int main() {
	cv::Mat input{100, 100, CV_8UC1, 0};
	cv::Mat output{100, 100, CV_8UC1, 0};
	
	//cv::dilate(input, output, cv::Mat());

	cvv::debugDilate(input, output, CVVISUAL_LOCATION);
}
