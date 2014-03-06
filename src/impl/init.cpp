#include "init.hpp"

#include "../dbg/dbg.hpp"

// filters
#include "../qtutil/filterselectorwidget.hpp"
#include "../qtutil/filter/grayfilterwidget.hpp"
#include "../qtutil/filter/sobelfilterwidget.hpp"
#include "../qtutil/filter/channelreorderfilter.hpp"
#include "../qtutil/filter/diffFilterWidget.hpp"
#include "../qtutil/filter/overlayfilterwidget.hpp"
#include "../qtutil/filter/changed_pixels_widget.hpp"

#include "../gui/filter_call_tab.hpp"
#include "../view/filter_view.hpp"
#include "../view/defaultfilterview.hpp"
#include "../view/dual_filter_view.hpp"
#include "../view/singlefilterview.hpp"

#include "../gui/match_call_tab.hpp"
#include "../view/match_view.hpp"
#include "../view/linematchview.hpp"
#include "../view/rawview.hpp"
#include "../view/translationsmatchview.hpp"
#include "../view/depthview.hpp"

#include "../qtutil/matchview/matchselectionselector.hpp"
#include "../qtutil/matchview/matchintervallselection.hpp"

#include "../qtutil/matchview/matchsettingsselector.hpp"
#include "../qtutil/matchview/singlecolormatchpen.hpp"
#include "../qtutil/matchview/falsecolormatchpen.hpp"

namespace cvv
{
namespace impl
{

void initializeFilterAndViews()
{
	static bool alreadyCalled = false;
	if (alreadyCalled)
	{
		return;
	}
	alreadyCalled = true;

	// filter for filter-selector-widget
	qtutil::registerFilter<1, 1, qtutil::GrayFilterWidget>("Gray filter");
	qtutil::registerFilter<1, 1, qtutil::SobelFilterWidget>("Sobel");
	qtutil::registerFilter<1, 1, qtutil::ChannelReorderFilter>(
	    "Reorder channels");

	qtutil::registerFilter<2, 1, qtutil::DiffFilterFunction>("Difference");
	qtutil::registerFilter<2, 1, qtutil::OverlayFilterWidget>("Overlay");
	qtutil::registerFilter<2, 1, qtutil::ChangedPixelsWidget>("Changed Pixels");

	// filter-views:
	cvv::gui::FilterCallTab::registerFilterView<
	    cvv::view::DefaultFilterView> ("DefaultFilterView");
	cvv::gui::FilterCallTab::registerFilterView<cvv::view::DualFilterView> (
	    "DualFilterView");
	cvv::gui::FilterCallTab::registerFilterView<
	    cvv::view::SingleFilterView>("SingleFilterView");

	// match-views:
	cvv::gui::MatchCallTab::registerMatchView<cvv::view::LineMatchView>(
	    "LineMatchView");
	cvv::gui::MatchCallTab::registerMatchView<
	    cvv::view::TranslationMatchView>("TranslationMatchView");
	cvv::gui::MatchCallTab::registerMatchView<cvv::view::DepthMatchView>(
	    "DepthMatchView");
	cvv::gui::MatchCallTab::registerMatchView<cvv::view::Rawview>(
	    "RawView");

	//match Settings
	cvv::qtutil::MatchSettingsSelector::registerElement("Single Color", [](const std::vector<cv::DMatch>& univers)
		{
			return std::unique_ptr<cvv::qtutil::MatchSettings>{new cvv::qtutil::SingleColorMatchPen{univers}};
		}
	);

	cvv::qtutil::MatchSettingsSelector::registerElement("False Color", [](const std::vector<cv::DMatch>& univers)
		{
			return std::unique_ptr<cvv::qtutil::MatchSettings>{new cvv::qtutil::FalseColorMatchPen{univers}};
		}
	);

	//match Selector
	cvv::qtutil::MatchSelectionSelector::registerElement("Intervall Selector", [](const std::vector<cv::DMatch>& univers)
	{
		return std::unique_ptr<cvv::qtutil::MatchSelection>{new cvv::qtutil::MatchIntervallSelector{univers}};
	});

}
}
}
