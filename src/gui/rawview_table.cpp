#include "rawview_table.hpp"

#include <utility>

#include <QVBoxLayout>
#include <QStringList>

#include "../stfl/element_group.hpp"
#include "rawview_table_row.hpp"
#include "rawview_group_subtable.hpp"
#include "../qtutil/accordion.hpp"

namespace cvv { namespace gui {

RawviewTable::RawviewTable(util::Reference<controller::ViewController> controller, view::Rawview *parent):
    controller{controller}, parent{parent}
{
    subtableAccordion = new qtutil::Accordion{};
    auto *layout = new QVBoxLayout{};
    layout->setContentsMargins(0, 0, 0, 0);
    layout->addWidget(subtableAccordion);
    setLayout(layout);
}

void RawviewTable::updateRowGroups(std::vector<stfl::ElementGroup<RawviewTableRow>> newGroups)
{
    subtableAccordion->clear();
    subTables.clear();
    for (auto &group : newGroups)
    {
        if (group.size() > 0)
        {
			auto subtable = util::make_unique<RawviewGroupSubtable>(controller, this, std::move(group));
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

void RawviewTable::updateUI()
{
    for (auto *subTable : subTables)
    {
        subTable->updateUI();
    }
}

}}
