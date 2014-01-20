#ifndef CVVISUAL_OVERVIEWPANEL_HPP
#define	CVVISUAL_OVERVIEWPANEL_HPP

#include <QWidget>
#include <QString>

#include "../stfl/stfl_engine.hpp"
#include "../impl/call.hpp"
#include "overview_table.hpp"
#include "overview_table_collumn.hpp"
#include "../qtutil/stfl_query_widget.hpp"
#include "../util/util.hpp"
#include "../controller/view_controller.hpp"

namespace cvv {

namespace controller {
	class ViewController;
}

namespace gui {

class OverviewTable;
class STFLQueryWidget;
class OverviewTableCollumn;

class OverviewPanel : public QWidget
{
	
	Q_OBJECT
	
public:
	
	/**
	 * @todo implement
     */
	OverviewPanel(controller::ViewController *controller);
	
	/**
	 * @todo implement
     */
	void addElement(const util::Reference<const impl::Call> newCall);
	
public slots:	
	/**
	 * @todo implement
     */
	void filterQuery(QString query);
	/**
	 * @todo implement
     */
	void updateQuery(QString query);
	
private:
	stfl::STFLEngine<OverviewTableCollumn> queryEngine;
	qtutil::STFLQueryWidget *queryWidget;
	OverviewTable *table;
	controller::ViewController *controller;
};

}}

#endif
