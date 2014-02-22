#include "init.hpp"

#include "../dbg/dbg.hpp"

//filters
#include "../qtutil/filterselectorwidget.hpp"
#include "../qtutil/filter/grayfilterwidget.hpp"
#include "../qtutil/filter/sobelfilterwidget.hpp"
#include "../qtutil/filter/channelreorderfilter.hpp"

#include "../gui/match_call_tab.hpp"

namespace cvv {namespace impl {


void initializeFilterAndViews()
{
	TRACEPOINT;
	qtutil::registerFilter<1,1,qtutil::GrayFilterWidget>("Gray filter");
	TRACEPOINT;
	qtutil::registerFilter<1,1,qtutil::SobelFilterWidget>("Sobel");
	TRACEPOINT;
	qtutil::registerFilter<1,1,qtutil::ChannelReorderFilter>("Reorder channels");
	TRACEPOINT;

//	beispiel für einen filter call tab
//	gui::FilterCallTab::registerElement(#NAME#,[](const std::vector<cv::Mat>& mat, QWidget* p){
//		return std::unique_ptr<view::FilterView>{new #KLASSE#(mat,p)};
//	});

//	beispiel für einen match call tab
//	gui::MatchCallTab::registerElement(#NAME#,
//		[](const cv::Mat& mat1, const std::vector<cv::KeyPoint>& key1,
//		   const cv::Mat& mat2, const std::vector<cv::KeyPoint>& key2,
//		   const std::vector<cv::DMatch>& matches, QWidget* p){
//		return std::unique_ptr<view::MatchView>{new #KLASSE#(mat1,key1,mat2,key2,matches,p)};
//	});


/*
alternativ könnten folgende funktionen erstellt und dann genutzt werden:


template<class View>
bool registerFilterView(const QString& name)
{
	TRACEPOINT;
	return gui::FilterCallTab::registerElement(name,
		[](const std::vector<cv::Mat>& mat, QWidget* p){
			TRACEPOINT;
			return std::unique_ptr<view::FilterView>{new View(mat,p)};
		}
	);
}

template<class View>
bool registerMatchView(const QString& name)
{
	TRACEPOINT;
	return gui::MatchCallTab::registerElement(name,
		[](const cv::Mat& mat1, const std::vector<cv::KeyPoint>& key1,
			const cv::Mat& mat2, const std::vector<cv::KeyPoint>& key2,
			const std::vector<cv::DMatch>& matches, QWidget* p)
		{
			TRACEPOINT;
			return std::unique_ptr<view::MatchView>
				{new View(mat1,key1,mat2,key2,matches,p)};
		}
	);
}

nutzen sollte klar sein:
registerFilterView<VIEW>("NAME");
registerMatchView<VIEW>("NAME");

return ist natuerlich, ob der filter registriert wurde (wenn flase nein, da der
name schon genutzt wurde)
*/
}

}}
