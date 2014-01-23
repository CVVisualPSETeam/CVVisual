#include <QString>
#include <QMap>
#include <QPushButton>
#include <QComboBox>

#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QLabel>

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
/* Only for testing: */	helpButton->setText("Connect successful");
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

	createGui();
}

FilterCallTab::FilterCallTab(const QString& tabName, const cvv::impl::FilterCall& fc, const cvv::controller::ViewController& vc, const QString& viewId):
	filterCall{fc}, viewController{vc}
{
	setName(tabName);
	filterViewId = viewId;

	createGui();
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

void FilterCallTab::createGui()
{
	QHBoxLayout* hlayout = new QHBoxLayout;
	hlayout->setAlignment(Qt::AlignTop);
	QLabel* selectionLabel = new QLabel{"View:"};
	hlayout->addWidget(selectionLabel);
	filterViewSelection = new QComboBox{};
	hlayout->addWidget(filterViewSelection);
	helpButton = new QPushButton{"Help"};
	hlayout->addWidget(helpButton);
	connect(helpButton, SIGNAL(clicked()), this, SLOT(helpButtonClicked()));

	QWidget* upperBar = new QWidget;
	upperBar->setLayout(hlayout);

	QVBoxLayout* vlayout = new QVBoxLayout;
	QLabel* viewDummy = new QLabel{"There will be a view here."};
	vlayout->addWidget(upperBar);
	vlayout->addWidget(viewDummy);

	setLayout(vlayout);
}

}}//namespaces
