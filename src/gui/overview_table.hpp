#ifndef CVVISUAL_OVERVIEWTABLE_HPP
#define	CVVISUAL_OVERVIEWTABLE_HPP

#include <vector>

#include <QWidget>
#include <QList>

#include "overview_panel.hpp"
#include "overview_table_row.hpp"
#include "../stfl/element_group.hpp"
#include "../qtutil/accordion.hpp"
#include "../util/util.hpp"
#include "../controller/view_controller.hpp"
#include "overview_group_subtable.hpp"

namespace cvv { namespace gui {

class OverviewPanel;
class OverviewTableRow;

class OverviewTable : public QWidget
{
	Q_OBJECT
	
public:
	
	OverviewTable(util::Reference<controller::ViewController> controller, OverviewPanel *parent);
	
	void updateRowGroups(const std::vector<stfl::ElementGroup<OverviewTableRow>> newGroups);
	
	void hideImages();
	
	void showImages();
	
	bool isShowingImages();

	/**
	 * @brief Updates the UI
	 */
	void updateUI();

private:
	util::Reference<controller::ViewController> controller;
	OverviewPanel *parent;
	bool doesShowImages = true;
	qtutil::Accordion *subtableAccordion;
	std::vector<OverviewGroupSubtable*> subTables{};

};

}}

#endif

