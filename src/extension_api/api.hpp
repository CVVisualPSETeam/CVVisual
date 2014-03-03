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
/**
 * @brief Introduces a new filter-view.
 */
void addFilterView(const QString name, FilterViewFactory factory);

using MatchViewFactory = gui::MatchCallTab::MatchViewBuilder;
/**
 * @brief Introduces a new match-view.
 */
void addMatchView(const QString name, MatchViewFactory factory);

using TabFactory = controller::TabFactory;
/**
 * @brief Introduces a new call-type.
 * @param factory A function that recieves a reference to a call and should return the appropriate
 * window.
 */
void addCallType(const QString name, TabFactory factory);

template<std::size_t In, std::size_t Out, class Filter>
/**
 * @brief Introduces a new filter for the filter-selector-widget.
 */
bool registerFilter(const QString& name)
{
	return cvv::qtutil::registerFilter<In, Out, Filter>(name);
}

}} //namespaces cvv::extend


#endif
