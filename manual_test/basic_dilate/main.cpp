#include <iostream>

#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>

#include "../../include/opencv2/dilate.hpp"
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

	std::string description = "dilate for ";
	description += filename;
	cv::dilate(src, dest, elem);
	cvv::debugDilate(src, dest, CVVISUAL_LOCATION, description.c_str());
	//from another location:
	cvv::debugDilate(src, dest, CVVISUAL_LOCATION, description.c_str(),
			"some weird view");
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
}
