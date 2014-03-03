#include "init.hpp"

#include "../dbg/dbg.hpp"

//filters
#include "../qtutil/filterselectorwidget.hpp"
#include "../qtutil/filter/grayfilterwidget.hpp"
#include "../qtutil/filter/sobelfilterwidget.hpp"
#include "../qtutil/filter/channelreorderfilter.hpp"
#include "../qtutil/filter/diffFilterWidget.hpp"
#include "../qtutil/filter/overlayfilterwidget.hpp"


#include "../gui/filter_call_tab.hpp"
#include "../view/filter_view.hpp"
#include "../view/defaultfilterview.hpp"
#include "../view/dual_filter_view.hpp"

#include "../view/match_view.hpp"
#include "../view/linematchview.hpp"
#include "../view/rawview.hpp"
#include "../view/translationsmatchview.hpp"
#include "../view/depthview.hpp"

#include "../gui/match_call_tab.hpp"

namespace cvv {namespace impl {

std::unique_ptr<cvv::view::FilterView> makeDefaultFilterView(
		const std::vector<cv::Mat>& images, QWidget* parent);

std::unique_ptr<cvv::view::FilterView> makeDualFilterView(
		const std::vector<cv::Mat>& images, QWidget* parent);


std::unique_ptr<cvv::view::MatchView> makeLineMatchView(
		const cv::Mat& img1, const std::vector<cv::KeyPoint>& key1,
		const cv::Mat& img2, const std::vector<cv::KeyPoint>& key2,
		const std::vector<cv::DMatch>& matches, QWidget* parent);

std::unique_ptr<cvv::view::MatchView> makeTranslationMatchView(
		const cv::Mat& img1, const std::vector<cv::KeyPoint>& key1,
		const cv::Mat& img2, const std::vector<cv::KeyPoint>& key2,
		const std::vector<cv::DMatch>& matches, QWidget* parent);

std::unique_ptr<cvv::view::MatchView> makeDepthMatchView(
		const cv::Mat& img1, const std::vector<cv::KeyPoint>& key1,
		const cv::Mat& img2, const std::vector<cv::KeyPoint>& key2,
		const std::vector<cv::DMatch>& matches, QWidget* parent);

std::unique_ptr<cvv::view::Rawview> makeRawview(
		const cv::Mat& img1, const std::vector<cv::KeyPoint>& key1,
		const cv::Mat& img2, const std::vector<cv::KeyPoint>& key2,
		const std::vector<cv::DMatch>& matches, QWidget* parent);


void initializeFilterAndViews()
{
	static bool alreadyCalled = false;
	if(alreadyCalled)
	{
		return;
	}
	alreadyCalled = true;

	// filter for filter-selector-widget
	qtutil::registerFilter<1,1,qtutil::GrayFilterWidget>("Gray filter");
	qtutil::registerFilter<1,1,qtutil::SobelFilterWidget>("Sobel");
	qtutil::registerFilter<1,1,qtutil::ChannelReorderFilter>("Reorder channels");
	
	qtutil::registerFilter<2,1,qtutil::DiffFilterFunction>("Difference");
	qtutil::registerFilter<2,1,qtutil::OverlayFilterWidget>("Overlay");
	
	//filter-views:
	cvv::gui::FilterCallTab::addFilterViewToMap("DefaultFilterView", makeDefaultFilterView);
	cvv::gui::FilterCallTab::addFilterViewToMap("DualFilterView", makeDualFilterView);
	
	//match-views:
	cvv::gui::MatchCallTab::addMatchViewToMap("LineMatchView", makeLineMatchView);
	cvv::gui::MatchCallTab::addMatchViewToMap("TranslationMatchView", makeTranslationMatchView);
	cvv::gui::MatchCallTab::addMatchViewToMap("DepthMatchView", makeDepthMatchView);
	cvv::gui::MatchCallTab::addMatchViewToMap("RawView", makeRawview);
}

std::unique_ptr<cvv::view::FilterView> makeDefaultFilterView(const std::vector<cv::Mat>& images, QWidget* parent)
{
	return cvv::util::make_unique<cvv::view::DefaultFilterView>(images, parent);
}
std::unique_ptr<cvv::view::FilterView> makeDualFilterView(const std::vector<cv::Mat>& images, QWidget* parent)
{
	return cvv::util::make_unique<cvv::view::DualFilterView>(images, parent);
}

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
}}
