#include "overview_panel.hpp"

namespace cvv {
namespace stfl {

OverviewPanel::OverviewPanel()
{
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
