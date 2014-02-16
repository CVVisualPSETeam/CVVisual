#include "overview_table.hpp"

#include <utility>
#include <algorithm>

#include <QVBoxLayout>
#include <QStringList>

#include "../stfl/element_group.hpp"
#include "overview_table_row.hpp"
#include "overview_group_subtable.hpp"
#include "../qtutil/accordion.hpp"
#include "../dbg/dbg.hpp"

namespace cvv { namespace gui {

OverviewTable::OverviewTable(util::Reference<controller::ViewController> controller, OverviewPanel *parent):
	controller{controller}, parent{parent}
{
	TRACEPOINT;
	subtableAccordion = new qtutil::Accordion{};
	auto *layout = new QVBoxLayout{};
	layout->setContentsMargins(0, 0, 0, 0);
	layout->addWidget(subtableAccordion);
	setLayout(layout);
	TRACEPOINT;
}

void OverviewTable::updateRowGroups(std::vector<stfl::ElementGroup<OverviewTableRow>> newGroups)
{
	TRACEPOINT;
	bool startTheSame = true;
	for (size_t i = 0; i < std::min(groups.size(), newGroups.size()); i++)
	{
		if (!newGroups.at(i).hasSameTitles(groups.at(i)))
		{
			startTheSame = false;
			break;
		}
	}
	TRACEPOINT;
	if (startTheSame)
	{
		TRACEPOINT;
		for (size_t i = 0; i < std::min(groups.size(), newGroups.size()); i++)
		{
			subTables.at(i)->setRowGroup(newGroups.at(i));
			subTables.at(i)->updateUI();
		}
		if (groups.size() < newGroups.size())
		{
			for (size_t i = groups.size(); i < newGroups.size(); i++)
			{
				TRACEPOINT;
				appendRowGroupToTable(newGroups.at(i));
				TRACEPOINT;
				subTables.at(i)->setRowGroup(newGroups.at(i));
			}
		}
		else if (groups.size() > newGroups.size())
		{
			TRACEPOINT;
			for (size_t i = groups.size(); i >= newGroups.size(); i--)
			{
				subTables.pop_back();
				subtableAccordion->deleteLast();
			}
		}
		TRACEPOINT;
	}
	else
	{
		TRACEPOINT;
		subtableAccordion->clear();
		subTables.clear();
		for (auto &group : newGroups)
		{
			appendRowGroupToTable(group);
		}
		TRACEPOINT;
	}
	groups = newGroups;
	TRACEPOINT;
}
	
void OverviewTable::hideImages()
{
	TRACEPOINT;
	doesShowImages = false;
	updateUI();
	TRACEPOINT;
}
	
void OverviewTable::showImages()
{
	TRACEPOINT;
	doesShowImages = true;
	updateUI();
	TRACEPOINT;
}
	
bool OverviewTable::isShowingImages()
{
	TRACEPOINT;
	return doesShowImages;
}

void OverviewTable::updateUI()
{
	TRACEPOINT;
	for (auto *subTable : subTables)
	{
		subTable->updateUI();
	}
	TRACEPOINT;
}

void OverviewTable::removeElement(size_t id)
{
	for (auto *subTable : subTables)
	{
		if (subTable->hasRow(id))
		{
			subTable->removeRow(id);
			break;
		}
	}
}

void OverviewTable::appendRowGroupToTable(stfl::ElementGroup<OverviewTableRow> group)
{
	if (group.size() > 0)
	{
		auto subtable = util::make_unique<OverviewGroupSubtable>(controller, this, std::move(group));
		auto subtablePtr = subtable.get();
		auto titles = group.getTitles();
		QString title = "No grouping specified, use #group to specify one";
		if (titles.size() != 0)
		{
			title = titles.join(", ");
		}
		subtableAccordion->push_back(title, std::move(subtable), false);
		subTables.push_back(subtablePtr);
	}
}

}}
