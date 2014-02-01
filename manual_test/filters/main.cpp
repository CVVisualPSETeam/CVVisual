#include <iostream>

#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>

#include "../../include/opencv2/dilate.hpp"
#include "../../include/opencv2/erode.hpp"
#include "../../include/opencv2/sobel.hpp"
#include "../../include/opencv2/morphology_ex.hpp"
#include "../../include/opencv2/final_show.hpp"

#include "../../src/controller/view_controller.hpp"
#include "../../src/gui/call_tab.hpp"
#include "../../src/gui/filter_call_tab.hpp"
#include "../../src/impl/call.hpp"
#include "../../src/util/util.hpp"

#include "../../src/dbg/dbg.hpp"


void dilateFile(char* filename) {
	auto src = cv::imread(filename);
	auto elem = cv::getStructuringElement(cv::MORPH_RECT, cv::Size(9, 9), cv::Point(4, 4));
	cv::Mat dest;

	auto newFct = [] (cvv::util::Reference<cvv::impl::Call> call, cvv::util::Reference<cvv::controller::ViewController> vc)
		{ return new cvv::gui::FilterCallTab(*call.castTo<cvv::impl::FilterCall>(), *vc); };
	cvv::controller::ViewController::addCallType("dilate", newFct);
	cvv::controller::ViewController::addCallType("erode", newFct);
	cvv::controller::ViewController::addCallType("morphologyEx", newFct);
	cvv::controller::ViewController::addCallType("sobel", newFct);

	cv::dilate(src, dest, elem);
	cvv::debugDilate(src, dest, CVVISUAL_LOCATION, filename);
	cv::erode(src, dest, elem);
	cvv::debugErode(src, dest, CVVISUAL_LOCATION, filename);
	cv::Sobel(src, dest, -1, 1, 1);
	cvv::debugSobel(src, dest, CVVISUAL_LOCATION, filename);
	cv::morphologyEx(src, dest,cv::MORPH_GRADIENT, elem );
	cvv::debugMorphologyEx(src, dest, CVVISUAL_LOCATION, filename);
	TRACEPOINT;
}

int main(int argc, char** argv) {
	cvv::dbg::setPriority(100);
	if(argc == 1)
	{
		std::cerr << argv[0] << " must be callled with one or more files as arguments\n";
		return 1;
	}
	for(int i=1; i < argc; ++i) {
		dilateFile(argv[i]);
	}
	TRACEPOINT;
	cvv::finalShow();
	TRACEPOINT;
}
