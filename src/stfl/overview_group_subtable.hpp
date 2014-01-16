#ifndef CVVISUAL_OVERVIEW_GROUP_SUBTABLE_HPP
#define	CVVISUAL_OVERVIEW_GROUP_SUBTABLE_HPP

#include <QWidget>
#include <QTableWidget>

#include "overview_table.hpp"
#include "element_group.hpp"
#include "overview_table_collumn.hpp"
#include "../util/optional.hpp"

namespace cvv { namespace stfl {

class OverviewGroupSubtable : public QWidget
{
	Q_OBJECT
	
public:
		
	/**
	 * @todo implement
     */
	OverviewGroupSubtable(OverviewTable *parent, const ElementGroup<OverviewTableCollumn> &group);
	
	/**
	 * @todo implement
     */
	void updateGroup(const ElementGroup<OverviewTableCollumn> &group);
	
private:
	ElementGroup<OverviewTableCollumn> &group();
	OverviewTable *parent;
	QTableWidget *qTable;
};

}}

#endif

