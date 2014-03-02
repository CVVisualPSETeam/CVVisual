#include "overview_group_subtable.hpp"

#include <utility>
#include <algorithm>
#include <sstream>

#include <QVBoxLayout>
#include <QStringList>
#include <QModelIndex>
#include <QMenu>
#include <QAction>
#include <QHeaderView>
#include <QList>
#include <QSize>

#include "call_window.hpp"
#include "overview_table.hpp"
#include "../controller/view_controller.hpp"

#include "../dbg/dbg.hpp"

namespace cvv { namespace gui {

OverviewGroupSubtable::OverviewGroupSubtable(
		 util::Reference<controller::ViewController> controller,
		 OverviewTable *parent,
		 stfl::ElementGroup<OverviewTableRow> group):
	controller{controller}, parent{parent}, group{std::move(group)}
{
	TRACEPOINT;
	controller->setDefaultSetting("overview", "imgsize", QString::number(100));
	initUI();
	TRACEPOINT;
}

void OverviewGroupSubtable::initUI()
{
	TRACEPOINT;
	controller->setDefaultSetting("overview", "imgzoom", "30");
	qTable = new QTableWidget(this);
	qTable->setSelectionBehavior(QAbstractItemView::SelectRows);
	qTable->setSelectionMode(QAbstractItemView::SingleSelection);
	qTable->setTextElideMode(Qt::ElideNone);
	auto verticalHeader = qTable->verticalHeader();
	verticalHeader->setVisible(false);
	auto horizontalHeader = qTable->horizontalHeader();
	horizontalHeader->setSectionResizeMode(QHeaderView::ResizeToContents);
	horizontalHeader->setStretchLastSection(false);
	connect(qTable, SIGNAL(cellDoubleClicked(int, int)), this, SLOT(rowClicked(int,int)));
	qTable->setContextMenuPolicy(Qt::CustomContextMenu);
	connect(qTable, SIGNAL(customContextMenuRequested(QPoint)), 
			this, SLOT(customMenuRequested(QPoint)));
	auto *layout = new QVBoxLayout;
	layout->setContentsMargins(0, 0, 0, 0);
	layout->addWidget(qTable);
	setLayout(layout);
	updateUI();
	TRACEPOINT;
}

void OverviewGroupSubtable::updateUI(){
	TRACEPOINT;
	imgSize = controller->getSetting("overview", "imgzoom").toInt() * width() / 400;
	QStringList list{};
	list << "ID";
	maxImages = 0;
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
	int textRowHeight = qTable->fontMetrics().height() + 5;
	if (textRowHeight >= imgSize)
	{
		qTable->setVerticalScrollMode(QAbstractItemView::ScrollPerItem);
	}
	else
	{
		qTable->setVerticalScrollMode(QAbstractItemView::ScrollPerPixel);
	}
	rowHeight = std::max(imgSize, textRowHeight);
	for (size_t i = 0; i < group.size(); i++)
	{
		group.get(i).addToTable(qTable, i, parent->isShowingImages(),
								maxImages, imgSize, imgSize);
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
	updateMinimumSize();
	TRACEPOINT;
}

void OverviewGroupSubtable::rowClicked(int row, int collumn)
{
	TRACEPOINT;
	(void)collumn;
	size_t tabId = group.get(row).id();
	controller->showAndOpenCallTab(tabId);
	TRACEPOINT;
}

void OverviewGroupSubtable::customMenuRequested(QPoint location)
{
	TRACEPOINT;
	if (qTable->rowCount() == 0)
	{
		return;
	}
	controller->removeEmptyWindows();
	QMenu *menu = new QMenu(this);
	TRACEPOINT;
	auto windows = controller->getTabWindows();
	menu->addAction(new QAction("Open in new window", this));
	for (auto window : windows)
	{
		menu->addAction(new QAction(QString("Open in '%1'").arg(
						window->windowTitle()), this));
	}
	TRACEPOINT;
	menu->addAction(new QAction("Remove call", this));
	TRACEPOINT;
	QModelIndex index = qTable->indexAt(location);
	if (!index.isValid())
	{
		return;
	}
	TRACEPOINT;
	int row = index.row();
	TRACEPOINT;
	QString idStr = qTable->item(row, 0)->text();
	TRACEPOINT;
	connect(menu, SIGNAL(triggered(QAction*)), this, SLOT(customMenuAction(QAction*)));
	TRACEPOINT;
	std::stringstream{idStr.toStdString()} >> currentCustomMenuCallTabId;
	currentCustomMenuCallTabIdValid = true;
	TRACEPOINT;
	menu->popup(mapToGlobal(location));
	TRACEPOINT;
}

void OverviewGroupSubtable::customMenuAction(QAction *action)
{
	TRACEPOINT;
	if (!currentCustomMenuCallTabIdValid)
	{
		return;
	}
	QString actionText = action->text();
	if (actionText == "Open in new window")
	{
		TRACEPOINT;
		controller->moveCallTabToNewWindow(currentCustomMenuCallTabId);
		TRACEPOINT;
		currentCustomMenuCallTabId = -1;
		return;
	}
	else if (actionText == "Remove call")
	{
		TRACEPOINT;
		controller->removeCallTab(currentCustomMenuCallTabId, true, true);
		TRACEPOINT;
		currentCustomMenuCallTabId = -1;
		return;
	}
	auto windows = controller->getTabWindows();
	for (auto window : windows)
	{
		if (actionText == QString("Open in '%1'").arg(window->windowTitle()))
		{
			TRACEPOINT;
			controller->moveCallTabToWindow(currentCustomMenuCallTabId, window->getId());
			TRACEPOINT;
			break;
		}
	}
	currentCustomMenuCallTabId = -1;
	TRACEPOINT;
}

void OverviewGroupSubtable::resizeEvent(QResizeEvent *event)
{
	TRACEPOINT;
	(void)event;
	imgSize = controller->getSetting("overview", "imgzoom").toInt() * width() / 400;
	rowHeight = std::max(imgSize, qTable->fontMetrics().height() + 5);
	for (size_t row = 0; row < group.size(); row++)
	{
		group.get(row).addToTable(qTable, row, parent->isShowingImages(),
										maxImages, imgSize, imgSize);
		qTable->setRowHeight(row, rowHeight);
	}
	updateMinimumSize();
	TRACEPOINT;
}

void OverviewGroupSubtable::removeRow(size_t id)
{
	TRACEPOINT;
	for (size_t i = 0; i < group.size(); i++)
	{
		if (group.get(i).id() == id)
		{
			group.removeElement(i);
			updateUI();
			break;
		}
	}
	TRACEPOINT;
}

bool OverviewGroupSubtable::hasRow(size_t id)
{
	for (size_t i = 0; i < group.size(); i++)
	{
		if (group.get(i).id() == id)
		{
			return true;
		}
	}
	return false;
}

void OverviewGroupSubtable::setRowGroup(stfl::ElementGroup<OverviewTableRow> &newGroup)
{
	auto compFunc = [](const OverviewTableRow &first, const OverviewTableRow &second){
		return first.id() == second.id();
	};
	if (group.hasSameElementList(newGroup, compFunc))
	{
		return;
	}
	//Now both groups aren't the same 
	size_t newMax = 0;
	for (auto row : newGroup.getElements())
	{
		if (row.call()->matrixCount() > newMax)
		{
			newMax = row.call()->matrixCount();
		}
	}
	if (newMax == maxImages)
	{
		group = newGroup;
		updateUI();
		return;
	}
	//Now both groups have the same maximum number of images within their elements
	size_t minLength = std::min(group.size(), newGroup.size());
	for (size_t i = 0; i < minLength; i++)
	{
		if (group.get(i).id() != newGroup.get(i).id())
		{
			group = newGroup;
			updateUI();
			return;
		}
	}
	//Now the bigger group's element lists starts with the smaller group's one
	if (group.size() < newGroup.size())
	{
		//Now the new group appends the current group
		for (size_t row = group.size(); row < newGroup.size(); row++)
		{
			newGroup.get(row).addToTable(qTable, row, parent->isShowingImages(),
											maxImages, imgSize, imgSize);
			qTable->setRowHeight(row, rowHeight);
		}
	}
	else
	{
		//Now the new group deletes elements from the current group
		for (size_t row = group.size() - 1; row >= newGroup.size(); row--)
		{
			qTable->removeRow(row);
		}
	}
	group = newGroup;
	updateMinimumSize();
}

void OverviewGroupSubtable::updateMinimumSize()
{
	int width = qTable->sizeHint().width();
	setMinimumWidth(width);
	int height = qTable->horizontalHeader()->height();
	for (int a = 0; a < qTable->rowCount(); ++a)
	{
		height += qTable->rowHeight(a);
	}
	setMinimumHeight(height + (qTable->rowCount() * 1));
	DEBUG(height);
}

}}
