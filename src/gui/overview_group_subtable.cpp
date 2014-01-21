#include "overview_group_subtable.hpp"

namespace cvv { namespace gui {

OverviewGroupSubtable::OverviewGroupSubtable(controller::ViewController *controller,
		    OverviewTable *parent, const stfl::ElementGroup<OverviewTableCollumn> group):
			controller{controller}, parent{parent}, group{group}
{
}

void OverviewGroupSubtable::initUI()
{
	qTable = new QTableWidget(this);
}

void OverviewGroupSubtable::updateGroup(const stfl::ElementGroup<OverviewTableCollumn> &group)
{
	(void)group;
}

}}
