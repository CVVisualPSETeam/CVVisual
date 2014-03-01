#include "api.hpp"

#include "../gui/filter_call_tab.hpp"
#include "../gui/match_call_tab.hpp"

namespace cvv { namespace extend {

void addFilterView(const QString name, FilterViewFactory factory)
{
	cvv::gui::FilterCallTab::addFilterViewToMap(name, factory);
}

void addMatchView(const QString name, MatchViewFactory factory)
{
	cvv::gui::MatchCallTab::addMatchViewToMap(name, factory);
}

void addCallType(const QString name, TabFactory factory)
{
	controller::ViewController::addCallType(name, factory);
}
}} //namespaces cvv::extend
