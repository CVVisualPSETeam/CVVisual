#ifndef CVVISUAL_EXTENSION_API_HPP
#define CVVISUAL_EXTENSION_API_HPP

#include <opencv2/core/core.hpp>

#include <QString>
#include <QWidget>

#include "../impl/call.hpp"
#include "../controller/view_controller.hpp"
#include "../view/filter_view.hpp"
#include "../gui/match_call_tab.hpp"
#include "../qtutil/filterselectorwidget.hpp"

namespace cvv { namespace extend {
using FilterViewFactory =
	std::function<std::unique_ptr<cvv::view::FilterView>(const std::vector<cv::Mat>&, QWidget*)>;
void addFilterView(const QString name, FilterViewFactory factory);

using MatchViewFactory = gui::MatchCallTab::MatchViewBuilder;
void addMatchView(const QString name, MatchViewFactory factory);

using TabFactory = controller::TabFactory;
void addCallType(const QString name, TabFactory factory);

template<std::size_t In, std::size_t Out, class Filter>
bool registerFilter(const QString& name)
{
	return cvv::qtutil::registerFilter<In, Out, Filter>(name);
}

}} //namespaces cvv::extend


#endif
