#ifndef CVVISUAL_OVERVIEW_GROUP_SUBTABLE_HPP
#define	CVVISUAL_OVERVIEW_GROUP_SUBTABLE_HPP

#include <memory>

#include <QWidget>
#include <QTableWidget>
#include <QAction>
#include <QResizeEvent>

#include "../stfl/element_group.hpp"
#include "overview_table_collumn.hpp"
#include "../util/optional.hpp"
#include "../util/util.hpp"
#include "../controller/view_controller.hpp"

namespace cvv { namespace controller {
class ViewController;
}}

namespace cvv { namespace gui {

class OverviewTable;

class OverviewGroupSubtable : public QWidget
{
    Q_OBJECT

public:

    /**
     * @todo implement
     */
    OverviewGroupSubtable(util::Reference<controller::ViewController> controller,
                         OverviewTable *parent,
                         stfl::ElementGroup<OverviewTableCollumn> group);

    void updateUI();

protected:
	void resizeEvent(QResizeEvent *event);

private slots:
    void rowClicked(int row, int collumn);
	void customMenuRequested(QPoint location);
    void customMenuAction(QAction *action);

private:
    util::Reference<controller::ViewController> controller;
    OverviewTable *parent;
    stfl::ElementGroup<OverviewTableCollumn> group;
    QTableWidget *qTable;
	int currentCustomMenuCallTabId = -1;

	void initUI();
};

}}

#endif

