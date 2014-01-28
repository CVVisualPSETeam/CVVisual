#ifndef CVVISUAL_OVERVIEWPANEL_HPP
#define	CVVISUAL_OVERVIEWPANEL_HPP

#include <QWidget>
#include <QString>

#include "../stfl/stfl_engine.hpp"
#include "../impl/call.hpp"
#include "overview_table.hpp"
#include "overview_table_collumn.hpp"
#include "../util/util.hpp"
#include "../controller/view_controller.hpp"

namespace cvv {

namespace controller {
	class ViewController;
}

namespace qtutil {
	class STFLQueryWidget;
}

namespace gui {

class OverviewTable;
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

    void requestSuggestions(QString query);
	
private:
	stfl::STFLEngine<OverviewTableCollumn> queryEngine;
	qtutil::STFLQueryWidget *queryWidget;
	OverviewTable *table;
	controller::ViewController *controller;

    void initEngine();
};

}}

#endif
