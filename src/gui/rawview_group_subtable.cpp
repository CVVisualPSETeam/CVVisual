#include "rawview_group_subtable.hpp"

#include <utility>

#include <QVBoxLayout>
#include <QStringList>
#include <QModelIndexList>
#include <QModelIndex>
#include <QMenu>
#include <QAction>
#include <QHeaderView>
#include <QApplication>
#include <QClipboard>

#include "call_window.hpp"
#include "rawview_table.hpp"
#include "../controller/view_controller.hpp"
#include "../dbg/dbg.hpp"

namespace cvv { namespace gui {

RawviewGroupSubtable::RawviewGroupSubtable(RawviewTable *parent,
										   stfl::ElementGroup<RawviewTableRow> group):
	parent{parent}, group{std::move(group)}
{
	TRACEPOINT;
	qTable = new QTableWidget(this);
    qTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    qTable->setSelectionMode(QAbstractItemView::MultiSelection);
	auto horizontalHeader = qTable->horizontalHeader();
    horizontalHeader->setSectionResizeMode(QHeaderView::Stretch);
	horizontalHeader->setStretchLastSection(false);
	qTable->setVerticalScrollMode(QAbstractItemView::ScrollPerPixel);
	qTable->setContextMenuPolicy(Qt::CustomContextMenu);
	connect(qTable, SIGNAL(customContextMenuRequested(QPoint)), this, SLOT(customMenuRequested(QPoint)));
    auto *layout = new QVBoxLayout;
    layout->setContentsMargins(0, 0, 0, 0);
    layout->addWidget(qTable);
    setLayout(layout); 
	
	QStringList list{};
	list << "match distance" << "img idx" << "query idx" << "train idx";
	list << "key point 1 x" << "y 1" << "size 1" << "angle 1" << "response 1" << "octave 1" << "class id 1";
	list << "ke ypoint 2 x" << "y 2" << "size 2" << "angle 2" << "response 2" << "octave 2" << "class id 2";
	qTable->setColumnCount(list.size());
    qTable->setHorizontalHeaderLabels(list);
	updateUI();
	TRACEPOINT;
}

void RawviewGroupSubtable::updateUI(){
	TRACEPOINT;
    qTable->setRowCount(group.size());
    for (size_t i = 0; i < group.size(); i++)
    {
        group.get(i).addToTable(qTable, i);
    }
    TRACEPOINT;
}

void RawviewGroupSubtable::selectionChanged()
{
	TRACEPOINT;
	currentRowIndexes = {};
	QModelIndexList indexList = qTable->selectionModel()->selectedIndexes();
	for (QModelIndex index : indexList) 
	{
		if (index.isValid())
		{
			auto row = index.row();
			if (row < qTable->rowCount() && row >= 0)
			{
				currentRowIndexes.push_back(row);
			}
		}
	}
	TRACEPOINT;
}

void RawviewGroupSubtable::customMenuRequested(QPoint location)
{
	TRACEPOINT;
	QModelIndex index = qTable->indexAt(location);
	if (!index.isValid())
	{
		return;
	}
	
	QMenu *menu = new QMenu(this);
	connect(menu, SIGNAL(triggered(QAction*)), this, SLOT(customMenuAction(QAction*)));
	
	auto formats = RawviewTableRow::getAvailableTextFormats();
   	for (auto format : formats)
	{
		menu->addAction(new QAction(QString("Copy as %1").arg(format), this));
	}
	int row = index.row();
	if (currentRowIndexes.size() == 0)
	{
		currentRowIndexes = { row };
	}
	menu->popup(qTable->viewport()->mapToGlobal(location));
	TRACEPOINT;
}

void RawviewGroupSubtable::customMenuAction(QAction *action)
{
	TRACEPOINT;
	if (currentRowIndexes.size() > 0)
	{
		std::vector<RawviewTableRow> rows;
		for (auto index : currentRowIndexes)
		{
			if (index < qTable->rowCount() && index > 0)
			{
				rows.push_back(group.get(index));
			}
		}
		auto formats = RawviewTableRow::getAvailableTextFormats();
		for (auto format : formats)
		{
			if (action->text() == QString("Copy as %1").arg(format))
			{
				QString formattedRows = RawviewTableRow::rowsToText(rows, format);
				QApplication::clipboard()->setText(formattedRows);
				break;
			}
		}
	}
	DEBUG("Action: " + action->text().toStdString());
	currentRowIndexes = {};
	TRACEPOINT;
}

}}
