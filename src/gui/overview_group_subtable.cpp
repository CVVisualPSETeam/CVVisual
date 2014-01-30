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
    qTable->verticalHeader()->setVisible(false);
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
	controller->showAndOpenCallTab(tabId);
}

void OverviewGroupSubtable::customMenuRequested(QPoint location)
{
	QMenu *menu = new QMenu(this);
	connect(menu, SIGNAL(triggered(QAction*)), this, SLOT(customMenuAction(QAction*)));
    auto windows = controller->getTabWindows();
	menu->addAction(new QAction("Open in new Window", this));
	for (auto window : windows)
	{
		menu->addAction(new QAction(QString("Open in '%1'").arg(
						window->windowTitle()), this));
	}
	QModelIndex index = qTable->indexAt(location);
	int row = index.row();
	QString idStr = qTable->item(row, 0)->text();
	currentCustomMenuCallTabId = idStr.toInt();  
	menu->popup(qTable->viewport()->mapToGlobal(location));
}

void OverviewGroupSubtable::customMenuAction(QAction *action)
{
	if (currentCustomMenuCallTabId == -1)
	{
		return;
	}
	QString actionText = action->text();
	auto windows = controller->getTabWindows();
	if (actionText == QString("Open in new window"))
	{
		controller->moveCallTabToNewWindow(currentCustomMenuCallTabId);
		return;
	}
	for (auto window : windows)
	{
		if (actionText == QString("Open in '%1'").arg(window->windowTitle()))
		{
			controller->moveCallTabToWindow(currentCustomMenuCallTabId, window->getId());
			break;
		}
	}
	currentCustomMenuCallTabId = -1;
}
}}
