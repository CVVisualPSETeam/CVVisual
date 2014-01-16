#include "overview_table.hpp"

namespace cvv { namespace stfl {

OverviewTable::OverviewTable(OverviewPanel *parent)
{
	this->parent = parent;
}

void OverviewTable::updateCollumnGroups(const QList<ElementGroup<OverviewTableCollumn>> &newGroup)
{
	(void)newGroup;
}
	
void OverviewTable::hideImages()
{
	doesShowImages = false;
	updateUI();
}
	
void OverviewTable::showImages()
{
	doesShowImages = true;
	updateUI();
}
	
bool OverviewTable::isShowingImages()
{
	return doesShowImages;
}

void OverviewTable::updateUI()
{
	
}

}}
