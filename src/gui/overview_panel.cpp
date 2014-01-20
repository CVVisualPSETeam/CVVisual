#include "overview_panel.hpp"

namespace cvv { namespace gui {

OverviewPanel::OverviewPanel(controller::ViewController *controller)
{
	(void)controller;
	//queryEngine = new STFLEngine();
}

void OverviewPanel::addElement(const util::Reference<const impl::Call> newCall)
{
	OverviewTableCollumn col(newCall);
	queryEngine.addNewElement(col);
}

void OverviewPanel::filterQuery(QString query)
{
	(void)query;
}

void OverviewPanel::updateQuery(QString query)
{
	(void)query;
}

}}
