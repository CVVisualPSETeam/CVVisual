#include <iostream>
#include <memory>
#include <vector>
#include <utility>

#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/features2d/features2d.hpp>

#include "show_image.hpp"
#include "final_show.hpp"
#include "filter.hpp"

#include "../../src/gui/filter_call_tab.hpp"
#include "../../src/view/filter_view.hpp"
#include "../../src/view/defaultfilterview.hpp"
#include "../../src/view/dual_filter_view.hpp"


std::unique_ptr<cvv::view::FilterView> makeDefaultFilterView(const std::vector<cv::Mat>& images, QWidget* parent)
{
	return cvv::util::make_unique<cvv::view::DefaultFilterView>(images, parent);
}
std::unique_ptr<cvv::view::FilterView> makeDualFilterView(const std::vector<cv::Mat>& images, QWidget* parent)
{
	return cvv::util::make_unique<cvv::view::DualFilterView>(images, parent);
}

int main(int argc, char** argv)
{
	cvv::gui::FilterCallTab::addFilterViewToMap("DefaultFilterView", makeDefaultFilterView);
	cvv::gui::FilterCallTab::addFilterViewToMap("DualFilterView", makeDualFilterView);
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
