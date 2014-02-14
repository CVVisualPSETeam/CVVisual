#ifndef CVVISUAL_RAWVIEWTABLE_HPP
#define	CVVISUAL_RAWVIEWTABLE_HPP

#include <vector>

#include <QWidget>
#include <QList>

#include "../view/rawview.hpp"
#include "rawview_table_row.hpp"
#include "../stfl/element_group.hpp"
#include "../qtutil/accordion.hpp"
#include "../util/util.hpp"
#include "rawview_group_subtable.hpp"

namespace cvv { 

namespace view {
class Rawview;
}

namespace gui {

class RawviewTableCollumn;

class RawviewTable : public QWidget
{
	Q_OBJECT
	
public:
	
	RawviewTable(view::Rawview *parent);
	
	void updateRowGroups(const std::vector<stfl::ElementGroup<RawviewTableRow>> newGroups);

	/**
	 * @brief Updates the UI
	 */
	void updateUI();

private:
	view::Rawview *parent;
	qtutil::Accordion *subtableAccordion;
	std::vector<RawviewGroupSubtable*> subTables{};

};

}}

#endif

