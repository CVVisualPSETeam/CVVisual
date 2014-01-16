#ifndef CVVISUAL_OVERVIEWTABLE_HPP
#define	CVVISUAL_OVERVIEWTABLE_HPP

#include <QWidget>
#include <QList>

#include "overview_panel.hpp"
#include "overview_table_collumn.hpp"
#include "element_group.hpp"
#include "../qtutil/accordion.hpp"

namespace cvv { namespace stfl {

class OverviewPanel;
class OverviewTableCollumn;

class OverviewTable : public QWidget
{
	Q_OBJECT
	
public:
	
	/**
	 * @todo implement
     */
	OverviewTable(OverviewPanel *parent);
	
	/**
	 * @todo implement
     */
	void updateCollumnGroups(const QList<ElementGroup<OverviewTableCollumn>> &newGroup);
	
	void hideImages();
	
	void showImages();
	
	bool isShowingImages();

private:
	OverviewPanel *parent;
	bool doesShowImages = true;
	qtutil::Accordion subtableAccordion;
	
	void updateUI();
};

}}

#endif

