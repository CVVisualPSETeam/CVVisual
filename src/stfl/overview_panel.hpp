#ifndef CVVISUAL_OVERVIEWPANEL_HPP
#define	CVVISUAL_OVERVIEWPANEL_HPP

#include <QWidget>
#include <QString>

#include "stfl_engine.hpp"
#include "../impl/call.hpp"
#include "overview_table.hpp"
#include "overview_table_collumn.hpp"
#include "stfl_query_widget.hpp"
#include "../util/util.hpp"

namespace cvv {
namespace stfl {

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
	OverviewPanel();
	
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
	STFLEngine<OverviewTableCollumn> queryEngine;
	STFLQueryWidget *queryWidget;
	OverviewTable *table;
};

}}

#endif
