#include <iostream>
#include <random>
#include <vector>

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
#include "../../src/view/rawview.hpp"
#include "../../src/view/translationsmatchview.hpp"
#include "../../src/view/depthview.hpp"

std::unique_ptr<cvv::view::MatchView> makeLineMatchView(
		const cv::Mat& img1, const std::vector<cv::KeyPoint>& key1,
		const cv::Mat& img2, const std::vector<cv::KeyPoint>& key2,
		const std::vector<cv::DMatch>& matches, QWidget* parent)
{
	return cvv::util::make_unique<cvv::view::LineMatchView>(key1, key2, matches, img1, img2, parent);
}

std::unique_ptr<cvv::view::MatchView> makeTranslationMatchView(
		const cv::Mat& img1, const std::vector<cv::KeyPoint>& key1,
		const cv::Mat& img2, const std::vector<cv::KeyPoint>& key2,
		const std::vector<cv::DMatch>& matches, QWidget* parent)
{
	return cvv::util::make_unique<cvv::view::TranslationMatchView>(key1, key2, matches, img1, img2, parent);
}

std::unique_ptr<cvv::view::MatchView> makeDepthMatchView(
		const cv::Mat& img1, const std::vector<cv::KeyPoint>& key1,
		const cv::Mat& img2, const std::vector<cv::KeyPoint>& key2,
		const std::vector<cv::DMatch>& matches, QWidget* parent)
{
	return cvv::util::make_unique<cvv::view::DepthMatchView>(key1, key2, matches, img1, img2, parent);
}

std::unique_ptr<cvv::view::Rawview> makeRawview(
		const cv::Mat& img1, const std::vector<cv::KeyPoint>& key1,
		const cv::Mat& img2, const std::vector<cv::KeyPoint>& key2,
		const std::vector<cv::DMatch>& matches, QWidget* parent)
{
	(void)img1;
	(void)img2;
	(void)parent;
	return cvv::util::make_unique<cvv::view::Rawview>(key1, key2, matches);
}


std::vector<cv::KeyPoint> makeRandomKeys(size_t x, size_t y, size_t n)
{
	static std::mt19937_64 gen{std::random_device{}()};
	std::uniform_real_distribution<float> xdist{0.0f, static_cast<float>(x)};
	std::uniform_real_distribution<float> ydist{0.0f, static_cast<float>(y)};
	std::uniform_real_distribution<float> sdist{0.0f, 3.0f};
	std::vector<cv::KeyPoint> keypoints;
	for(size_t i=0; i < n; ++i)
	{
		keypoints.emplace_back(xdist(gen), ydist(gen), sdist(gen));
	}
	return keypoints;
}
std::vector<cv::KeyPoint> scaleDown(const std::vector<cv::KeyPoint>& in, size_t x, size_t y,
		float factor)
{
	std::vector<cv::KeyPoint> points;
	points.reserve(in.size());
	
	for(const auto& point: in)
	{
		auto newX = (x*(1.0f-factor)/2)+ factor*point.pt.x;
		auto newY = (y*(1.0f-factor)/2)+ factor*point.pt.y;
		points.emplace_back(newX, newY, point.size);
	}
	
	return points;
}
int main(int argc, char** argv)
{
	TRACEPOINT;
	if(argc < 2)
	{
		std::cerr << argv[0] << " must recieve one or more files as arguments\n";
		return 1;
	}
	//cvv::dbg::setLoggingState(true);
	TRACEPOINT;
	cvv::gui::MatchCallTab::addMatchViewToMap("LineMatchView", makeLineMatchView);
	cvv::gui::MatchCallTab::addMatchViewToMap("TranslationMatchView", makeTranslationMatchView);
	cvv::gui::MatchCallTab::addMatchViewToMap("DepthMatchView", makeDepthMatchView);
	cvv::gui::MatchCallTab::addMatchViewToMap("RawView", makeRawview);
	
	for(int i = 1; i < argc; ++i)
	{
		auto src = cv::imread(argv[i]);
		TRACEPOINT;
		const size_t keypointCount = 20;
		auto keypoints1 = makeRandomKeys(src.cols, src.rows, keypointCount);
		auto keypoints2 = scaleDown(keypoints1, src.cols, src.rows,0.8f);
		TRACEPOINT;

		std::vector<cv::DMatch> match;
		std::mt19937_64 gen{std::random_device{}()};
		std::uniform_real_distribution<float> dist{0.0f, 3.0f};
		for(size_t i=0;i<keypointCount;i++)
		{
			match.emplace_back(i,i,dist(gen));
		}
		
		TRACEPOINT;
		cvv::debugDMatch(src, keypoints1, src, keypoints2, match, CVVISUAL_LOCATION);
	}
	cvv::finalShow();
}
