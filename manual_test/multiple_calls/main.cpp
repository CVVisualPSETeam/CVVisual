#include <iostream>

#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/features2d/features2d.hpp>

#include "show_image.hpp"
#include "final_show.hpp"

int main(int argc, char **argv)
{
	for (int i = 1; i < argc; ++i)
	{
		cvv::showImage(cv::imread(argv[i]), CVVISUAL_LOCATION);
		cvv::finalShow();
	}
}
