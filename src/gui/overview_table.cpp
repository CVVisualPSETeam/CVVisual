#include "overview_table.hpp"

namespace cvv { namespace gui {

OverviewTable::OverviewTable(OverviewPanel *parent): parent{parent} {}

void OverviewTable::updateCollumnGroups(const QList<stfl::ElementGroup<OverviewTableCollumn>> &newGroup)
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
