#ifndef CVVISUAL_OVERVIEW_GROUP_SUBTABLE_HPP
#define	CVVISUAL_OVERVIEW_GROUP_SUBTABLE_HPP

#include <memory>

#include <QWidget>
#include <QTableWidget>

#include "../stfl/element_group.hpp"
#include "overview_table_collumn.hpp"
#include "../util/optional.hpp"
#include "../util/util.hpp"
#include "../controller/view_controller.hpp"

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

private slots:
    void rowClicked(int row, int collumn);

private:
    util::Reference<controller::ViewController> controller;
    OverviewTable *parent;
    stfl::ElementGroup<OverviewTableCollumn> group;
    QTableWidget *qTable;

    void initUI();
};

}}

#endif

