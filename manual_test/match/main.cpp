#include <iostream>

#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/features2d/features2d.hpp>

#include "dmatch.hpp"
#include "final_show.hpp"

#include "../../src/util/util.hpp"

#include "../../src/gui/match_call_tab.hpp"

#include "../../src/view/match_view.hpp"
#include "../../src/view/linematchview.hpp"

std::unique_ptr<cvv::view::MatchView> makeLineMatchView(
		const cv::Mat& img1, const std::vector<cv::KeyPoint>& key1,
		const cv::Mat& img2, const std::vector<cv::KeyPoint>& key2,
		const std::vector<cv::DMatch>& matches, QWidget* parent)
{
	return cvv::util::make_unique<cvv::view::LineMatchView>(key1, key2, matches, img1, img2, parent);
}

int main(int argc, char** argv) {
	TRACEPOINT;
	if(argc != 2)
	{
		std::cerr << argv[0] << " must be callled with one files as arguments\n";
		return 1;
	}
	//cvv::dbg::setLoggingState(true);
	TRACEPOINT;
	cvv::gui::MatchCallTab::addMatchViewToMap("LineMatchView", makeLineMatchView);
	TRACEPOINT;
	
	auto src = cv::imread(argv[1]);
	TRACEPOINT;
	
	std::vector<cv::KeyPoint> key;
	TRACEPOINT;
	for(int i=0;i<std::min(src.rows,src.cols);i+=30)
	{
		cv::Point2f pt{static_cast<float>(i),static_cast<float>(i)};
		key.emplace_back(pt,0.0f);
	}
	TRACEPOINT;

	std::vector<cv::DMatch> match;
	for(size_t i=0;i<key.size();i++)
	{
		match.emplace_back(i,i,1.0f);
	}
	
	TRACEPOINT;
	cvv::debugDMatch(src, key, src, key, match, CVVISUAL_LOCATION);
	
	std::cout << "All calculation done" << std::endl;
	cvv::finalShow();
	std::cout << "Program finished" << std::endl;
}
