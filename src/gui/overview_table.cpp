#include "overview_table.hpp"

#include <utility>

#include <QVBoxLayout>
#include <QStringList>

#include "../stfl/element_group.hpp"
#include "overview_table_collumn.hpp"
#include "overview_group_subtable.hpp"
#include "../qtutil/accordion.hpp"

namespace cvv { namespace gui {

OverviewTable::OverviewTable(util::Reference<controller::ViewController> controller, OverviewPanel *parent):
    controller{controller}, parent{parent}
{
    subtableAccordion = new qtutil::Accordion{};
    auto *layout = new QVBoxLayout{};
    layout->setContentsMargins(0, 0, 0, 0);
    layout->addWidget(subtableAccordion);
    setLayout(layout);
}

void OverviewTable::updateCollumnGroups(std::vector<stfl::ElementGroup<OverviewTableCollumn>> newGroups)
{
    subtableAccordion->clear();
    subTables.clear();
    for (auto &group : newGroups)
    {
        if (group.size() > 0)
        {
            auto subtable = util::make_unique<OverviewGroupSubtable>(controller, this, std::move(group));
	    auto subtablePtr = subtable.get();
            auto titles = group.getTitles();
            QString title = "No grouping specified, use #group to do specify";
            if (titles.size() != 0)
            {
                title = titles.join(", ");
            }
            subtableAccordion->push_back(title, std::move(subtable), false);
            subTables.push_back(subtablePtr);
        }
    }
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
    for (auto *subTable : subTables)
    {
        subTable->updateUI();
    }
}

}}
