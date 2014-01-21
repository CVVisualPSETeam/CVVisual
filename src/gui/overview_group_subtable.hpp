#ifndef CVVISUAL_OVERVIEW_GROUP_SUBTABLE_HPP
#define	CVVISUAL_OVERVIEW_GROUP_SUBTABLE_HPP

#include <QWidget>
#include <QTableWidget>

#include "overview_table.hpp"
#include "../stfl/element_group.hpp"
#include "overview_table_collumn.hpp"
#include "../util/optional.hpp"
#include "../controller/view_controller.hpp"

namespace cvv { namespace gui {

class OverviewGroupSubtable : public QWidget
{
	Q_OBJECT
	
public:
		
	/**
	 * @todo implement
     */
	OverviewGroupSubtable(controller::ViewController *controller,
						 OverviewTable *parent,
						 const stfl::ElementGroup<OverviewTableCollumn> group);
	
	/**
	 * @todo implement
     */
	void updateGroup(const stfl::ElementGroup<OverviewTableCollumn> &group);
	
private:
	controller::ViewController *controller;
	OverviewTable *parent;
	stfl::ElementGroup<OverviewTableCollumn> group;
	QTableWidget *qTable;
	
	
	void initUI();
};

}}

#endif

