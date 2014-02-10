#include "rawview_group_subtable.hpp"

#include <utility>

#include <QVBoxLayout>
#include <QStringList>
#include <QModelIndex>
#include <QMenu>
#include <QAction>
#include <QHeaderView>

#include "call_window.hpp"
#include "rawview_table.hpp"
#include "../controller/view_controller.hpp"

namespace cvv { namespace gui {

RawviewGroupSubtable::RawviewGroupSubtable(util::Reference<controller::ViewController> controller,
                                             RawviewTable *parent,
                                             stfl::ElementGroup<RawviewTableCollumn> group):
    controller{controller}, parent{parent}, group{std::move(group)}
{
	qTable = new QTableWidget(this);
    qTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    qTable->setSelectionMode(QAbstractItemView::SingleSelection);
	auto horizontalHeader = qTable->horizontalHeader();
    horizontalHeader->setSectionResizeMode(QHeaderView::Stretch);
	horizontalHeader->setStretchLastSection(false);
	qTable->setVerticalScrollMode(QAbstractItemView::ScrollPerPixel);
	connect(qTable, SIGNAL(cellDoubleClicked(int, int)), this, SLOT(rowClicked(int,int)));
	qTable->setContextMenuPolicy(Qt::CustomContextMenu);
	connect(qTable, SIGNAL(customContextMenuRequested(QPoint)), this, SLOT(customMenuRequested(QPoint)));
    auto *layout = new QVBoxLayout;
    layout->setContentsMargins(0, 0, 0, 0);
    layout->addWidget(qTable);
    setLayout(layout); 
	
	QStringList list{};
	list << "match distance" << "img idx" << "query idx" << "train idx";
	list << "key point 1 x" << "y 1" << "size 1" << "angle 1" << "response 1" << "octave 1" << "class id 1";
	list << "key point 2 x" << "y 2" << "size 2" << "angle 2" << "response 2" << "octave 2" << "class id 2";
	qTable->setColumnCount(list.size());
    qTable->setHorizontalHeaderLabels(list);
	
	updateUI();
}

void RawviewGroupSubtable::updateUI(){
    qTable->setRowCount(group.size());
    for (size_t i = 0; i < group.size(); i++)
    {
        group.get(i).addToTable(qTable, i);
    }
}

void RawviewGroupSubtable::rowClicked(int row, int collumn)
{
    (void)collumn;
	(void)row;
}

void RawviewGroupSubtable::customMenuRequested(QPoint location)
{
	QMenu *menu = new QMenu(this);
	connect(menu, SIGNAL(triggered(QAction*)), this, SLOT(customMenuAction(QAction*)));
	
    menu->addAction(new QAction("TEST", this));
	
	QModelIndex index = qTable->indexAt(location);
	int row = index.row();
	(void)row;
	menu->popup(qTable->viewport()->mapToGlobal(location));
}

void RawviewGroupSubtable::customMenuAction(QAction *action)
{
	std::cerr << "Action: " << action->text().toStdString() << "\n";
}

}}
