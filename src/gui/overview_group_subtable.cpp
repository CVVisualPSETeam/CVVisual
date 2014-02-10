#include "overview_group_subtable.hpp"

#include <utility>

#include <QVBoxLayout>
#include <QStringList>
#include <QModelIndex>
#include <QMenu>
#include <QAction>
#include <QHeaderView>

#include "call_window.hpp"
#include "overview_table.hpp"
#include "../controller/view_controller.hpp"

#include "../dbg/dbg.hpp"

namespace cvv { namespace gui {

OverviewGroupSubtable::OverviewGroupSubtable(util::Reference<controller::ViewController> controller,
		 OverviewTable *parent,
		 stfl::ElementGroup<OverviewTableRow> group):
	controller{controller}, parent{parent}, group{std::move(group)}
{
	controller->setDefaultSetting("overview", "imgsize", QString::number(100));
	initUI();
}

void OverviewGroupSubtable::initUI()
{
	controller->setDefaultSetting("overview", "imgzoom", "30");
	qTable = new QTableWidget(this);
	qTable->setSelectionBehavior(QAbstractItemView::SelectRows);
	qTable->setSelectionMode(QAbstractItemView::SingleSelection);
	auto verticalHeader = qTable->verticalHeader();
	verticalHeader->setVisible(false);
	auto horizontalHeader = qTable->horizontalHeader();
	horizontalHeader->setSectionResizeMode(QHeaderView::ResizeToContents);
	horizontalHeader->setStretchLastSection(false);
	qTable->setVerticalScrollMode(QAbstractItemView::ScrollPerPixel);
	connect(qTable, SIGNAL(cellDoubleClicked(int, int)), this, SLOT(rowClicked(int,int)));
	qTable->setContextMenuPolicy(Qt::CustomContextMenu);
	connect(qTable, SIGNAL(customContextMenuRequested(QPoint)), this, SLOT(customMenuRequested(QPoint)));
	auto *layout = new QVBoxLayout;
	layout->setContentsMargins(0, 0, 0, 0);
	layout->addWidget(qTable);
	setLayout(layout);
	updateUI();
}

void OverviewGroupSubtable::updateUI(){
	int imgSize = controller->getSetting("overview", "imgzoom").toInt() * width() / 400;
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
		for (auto element : group.getElements ())
		{
		if (maxImages < element.call()->matrixCount())
			{
				maxImages = element.call()->matrixCount();
			}
		}
		for (size_t i = 0; i < maxImages; i++)
		{
			list << QString("Image ") + QString::number(i + 1);
		}
	}
	list << "Description" << "Function" << "File" << "Line" << "Type";
	qTable->setRowCount(group.size());
	qTable->setColumnCount(list.size());
	qTable->setHorizontalHeaderLabels(list);
	int rowHeight = std::max(imgSize, qTable->fontMetrics().height() + 5);
	for (size_t i = 0; i < group.size(); i++)
	{
		group.get(i).addToTable(qTable, i, parent->isShowingImages(), maxImages, imgSize, imgSize);
		qTable->setRowHeight(i, rowHeight);
	}
	auto header = qTable->horizontalHeader();
	header->setSectionResizeMode(0, QHeaderView::ResizeToContents);
	for (size_t i = 1; i < maxImages + 1; i++)
	{
		header->setSectionResizeMode(i, QHeaderView::ResizeToContents);
	}
	for (size_t i = maxImages + 1; i < maxImages + 4; i++)
	{
		header->setSectionResizeMode(i, QHeaderView::Stretch);
	}
	header->setSectionResizeMode(maxImages + 4, QHeaderView::ResizeToContents);
	header->setSectionResizeMode(maxImages + 5, QHeaderView::ResizeToContents);
}

void OverviewGroupSubtable::rowClicked(int row, int collumn)
{
	(void)collumn;
	size_t tabId = group.get(row).id();
	controller->showAndOpenCallTab(tabId);
}

void OverviewGroupSubtable::customMenuRequested(QPoint location)
{
	QMenu *menu = new QMenu(this);
	connect(menu, SIGNAL(triggered(QAction*)), this, SLOT(customMenuAction(QAction*)));
	auto windows = controller->getTabWindows();
	menu->addAction(new QAction("Open in new window", this));
	for (auto window : windows)
	{
		// TODO: is this code really the best we can come up with?
		menu->addAction(new QAction(QString("Open in '%1'").arg(
						window->windowTitle()), this));
	}
	menu->addAction(new QAction("Remove call", this));
	QModelIndex index = qTable->indexAt(location);
	int row = index.row();
	QString idStr = qTable->item(row, 0)->text();
	currentCustomMenuCallTabId = idStr.toInt();  
	menu->popup(qTable->viewport()->mapToGlobal(location));
	TRACEPOINT;
}

void OverviewGroupSubtable::customMenuAction(QAction *action)
{
	if (currentCustomMenuCallTabId == -1)
	{
		return;
	}
	QString actionText = action->text();
	auto windows = controller->getTabWindows();
	if (actionText == "Open in new window")
	{
		controller->moveCallTabToNewWindow(currentCustomMenuCallTabId);
		currentCustomMenuCallTabId = -1;
		return;
	}
	else if (actionText == "Remove call")
	{
		controller->removeCallTab(currentCustomMenuCallTabId, true, true);
		currentCustomMenuCallTabId = -1;
		return;
	}
	for (auto window : windows)
	{
		//TODO: see above: is string-compare really the best solution to this?
		if (actionText == QString("Open in '%1'").arg(window->windowTitle()))
		{
			controller->moveCallTabToWindow(currentCustomMenuCallTabId, window->getId());
			break;
		}
	}
	currentCustomMenuCallTabId = -1;
}

void OverviewGroupSubtable::resizeEvent(QResizeEvent *event)
{
	(void)event;
	updateUI();
}

}}
