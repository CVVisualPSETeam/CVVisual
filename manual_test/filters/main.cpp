#include <iostream>

#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>

#include "dilate.hpp"
#include "erode.hpp"
#include "sobel.hpp"
#include "morphology_ex.hpp"
#include "final_show.hpp"

#include "../../src/gui/filter_call_tab.hpp"
#include "../../src/view/filter_view.hpp"
#include "../../src/view/defaultfilterview.hpp"
#include "../../src/view/dual_filter_view.hpp"

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

std::unique_ptr<cvv::view::FilterView> makeDefaultFilterView(const std::vector<cv::Mat>& images, QWidget* parent)
{
	return cvv::util::make_unique<cvv::view::DefaultFilterView>(images, parent);
}
std::unique_ptr<cvv::view::FilterView> makeDualFilterView(const std::vector<cv::Mat>& images, QWidget* parent)
{
	return cvv::util::make_unique<cvv::view::DualFilterView>(images, parent);
}


int main(int argc, char** argv) {
	if(argc == 1)
	{
		std::cerr << argv[0] << " must be callled with one or more files as arguments\n";
		return 1;
	}
	cvv::dbg::setPriority(200);
	
	cvv::gui::FilterCallTab::addFilterViewToMap("DefaultFilterView", makeDefaultFilterView);
	cvv::gui::FilterCallTab::addFilterViewToMap("DualFilterView", makeDualFilterView);
	
	for(int i=1; i < argc; ++i) {
		dilateFile(argv[i]);
	}
	std::cout << "All calculation done" << std::endl;
	cvv::finalShow();
	std::cout << "Program finished" << std::endl;
}
