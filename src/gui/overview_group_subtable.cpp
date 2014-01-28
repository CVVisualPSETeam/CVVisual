#include "overview_group_subtable.hpp"

#include <utility>

#include <QVBoxLayout>
#include <QStringList>

#include "overview_table.hpp"

namespace cvv { namespace gui {

OverviewGroupSubtable::OverviewGroupSubtable(util::Reference<controller::ViewController> controller,
                                             OverviewTable *parent,
                                             stfl::ElementGroup<OverviewTableCollumn> group):
    controller{controller}, parent{parent}, group{std::move(group)}
{
    initUI();
}

void OverviewGroupSubtable::initUI()
{
	qTable = new QTableWidget(this);
    qTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    qTable->setSelectionMode(QAbstractItemView::SingleSelection);
    connect(qTable, SIGNAL(cellDoubleClicked(int, int)), this, SLOT(rowClicked(int,int)));
    auto *layout = new QVBoxLayout;
    layout->setContentsMargins(0, 0, 0, 0);
    layout->addWidget(qTable);
    setLayout(layout);
    updateUI();
}

void OverviewGroupSubtable::updateUI(){
    QStringList list{};
    list << "ID";
    size_t maxImages = 0;
    for (auto element : group.getElements ())
    {
        if (maxImages < element.call()->matrixCount())
        {
            maxImages = element.call()->matrixCount();
        }
    }
    if (parent->isShowingImages())
    {
        for (size_t i = 0; i < maxImages; i++)
        {
            list << QString("Image ") + QString::number(i + 1);
        }
    }
    list << "Description" << "Function" << "File" << "Line" << "Type";
    qTable->setRowCount(group.size());
    qTable->setColumnCount(list.size());
    qTable->setHorizontalHeaderLabels(list);
    for (size_t i = 0; i < group.size(); i++)
    {
        group.get(i).addToTable(qTable, i, parent->isShowingImages(), maxImages);
    }
    qTable->setVisible(false);
    qTable->resizeColumnsToContents();
    qTable->setVisible(true);
}

void OverviewGroupSubtable::rowClicked(int row, int collumn)
{
    (void)collumn;
    size_t tabId = group.get(row).id();
    controller->moveCallTabToWindow(tabId, 0);
    controller->showCallTab(tabId);
}
}}
