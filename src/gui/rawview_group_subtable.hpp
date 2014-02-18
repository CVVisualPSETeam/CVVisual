#ifndef CVVISUAL_RAWVIEW_GROUP_SUBTABLE_HPP
#define	CVVISUAL_RAWVIEW_GROUP_SUBTABLE_HPP

#include <memory>
#include <vector>

#include <QWidget>
#include <QTableWidget>
#include <QAction>
#include <QItemSelection>

#include "../stfl/element_group.hpp"
#include "rawview_table_row.hpp"
#include "../util/optional.hpp"
#include "../util/util.hpp"

namespace cvv { namespace controller {
class ViewController;
}}

namespace cvv { namespace gui {

class RawviewTable;

/**
 * @brief A table for the a group of overview data sets.
 */
class RawviewGroupSubtable : public QWidget
{
	Q_OBJECT

public:

    /**
     * @brief Constructs an over group subtable.
	 * @param controller view controller
	 * @param parent parent table
	 * @param group the displayed group of overview data sets
     */
	RawviewGroupSubtable(RawviewTable *parent, stfl::ElementGroup<RawviewTableRow> group);

	/**
	 * @brief Updates the displayed table UI.
	 */
	void updateUI();

private slots:
	void customMenuRequested(QPoint location);
	void customMenuAction(QAction *action);
	void selectionChanged();


private:
	RawviewTable *parent;
	stfl::ElementGroup<RawviewTableRow> group;
	QTableWidget *qTable;
	std::vector<int> currentRowIndexes;
};

}}

#endif

