#include <QString>
#include <QMap>
#include <QPushButton>
#include <QComboBox>

#include <QHBoxLayout>

#include "filter_call_tab.hpp"
#include "../view/filter_view.hpp"
#include "../controller/view_controller.hpp"
#include "../impl/filter_call.hpp"

namespace cvv {
namespace gui {

void FilterCallTab::currentIndexChanged(const QString& text) const
{
	/*filterViewId = text;
	filterView = filterViewMap[filterViewId].createFilterView(filterCall->original(), filterCall->result());*/
	(void) text;
}

void FilterCallTab::helpButtonClicked() const
{
	viewController->openHelpBrowser(filterViewId);
}

FilterCallTab::FilterCallTab(const QString& tabName, const cvv::impl::FilterCall& fc, const cvv::controller::ViewController& vc):
	filterCall{fc}, viewController{vc}
{
	setName(tabName);
	const QString scope{"default_views"};
	const QString key{"default_filter_view"};
	//QString setting = vc.getSetting(scope, key);
	QString setting = "PLACEHOLDER"; (void) scope; (void) key;
	filterViewId = setting;

	setName(tabName);
	hlayout = new QHBoxLayout;
	QPushButton* testHelpButton = new QPushButton{"Help"};
	hlayout->addWidget(testHelpButton);
	setLayout(hlayout);
}

FilterCallTab::FilterCallTab(const QString& tabName, const cvv::impl::FilterCall& fc, const cvv::controller::ViewController& vc, const QString& viewId):
	filterCall{fc}, viewController{vc}
{
	setName(tabName);
	filterViewId = viewId;
}

size_t FilterCallTab::getId() const
{
	return filterCall->getId();
}

void FilterCallTab::addFilterViewToMap(const QString& filterViewId, const cvv::view::FilterView& filterView)
{
	(void) filterViewId;
	(void) filterView;
}

}}//namespaces
