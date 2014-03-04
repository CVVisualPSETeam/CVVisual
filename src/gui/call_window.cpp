#include "call_window.hpp"

#include <QMenu>
#include <QStatusBar>
#include <QPushButton>
#include <QHBoxLayout>
#include <QVariant>

#include "../dbg/dbg.hpp"
#include "../stfl/stringutils.hpp"

namespace cvv
{

namespace controller
{
class ViewController;
}

namespace gui
{

CallWindow::CallWindow(util::Reference<controller::ViewController> controller,
                       size_t id)
    : id{ id }, controller{ controller }
{
	TRACEPOINT;
	initTabs();
	initFooter();
	setWindowTitle(QString("CVVisual | window no. %1").arg(id));
	setMinimumWidth(600);
	setMinimumHeight(600);
	TRACEPOINT;
}

void CallWindow::initTabs()
{
	TRACEPOINT;
	tabWidget = new TabWidget(this);
	tabWidget->setTabsClosable(true);
	tabWidget->setMovable(true);
	setCentralWidget(tabWidget);

	auto *flowButtons = new QHBoxLayout();
	auto *flowButtonsWidget = new QWidget(this);
	tabWidget->setCornerWidget(flowButtonsWidget, Qt::TopLeftCorner);
	flowButtonsWidget->setLayout(flowButtons);
	flowButtons->setAlignment(Qt::AlignLeft | Qt::AlignTop);
	closeButton = new QPushButton("Close", this);
	flowButtons->addWidget(closeButton);
	closeButton->setStyleSheet(
	    "QPushButton {background-color: red; color: white;}");
	closeButton->setToolTip("Close this debugging application.");
	connect(closeButton, SIGNAL(clicked()), this, SLOT(closeApp()));
	stepButton = new QPushButton("Step", this);
	flowButtons->addWidget(stepButton);
	stepButton->setStyleSheet(
	    "QPushButton {background-color: green; color: white;}");
	stepButton->setToolTip(
	    "Resume program execution for a next debugging step.");
	connect(stepButton, SIGNAL(clicked()), this, SLOT(step()));
	fastForwardButton = new QPushButton(">>", this);
	flowButtons->addWidget(fastForwardButton);
	fastForwardButton->setStyleSheet(
	    "QPushButton {background-color: yellow; color: blue;}");
	fastForwardButton->setToolTip(
	    "Fast forward until cvv::finalCall() gets called.");
	connect(fastForwardButton, SIGNAL(clicked()), this,
	        SLOT(fastForward()));
	flowButtons->setContentsMargins(0, 0, 0, 0);
	flowButtons->setSpacing(0);

	auto *tabBar = tabWidget->getTabBar();
	tabBar->setElideMode(Qt::ElideRight);
	tabBar->setContextMenuPolicy(Qt::CustomContextMenu);
	connect(tabBar, SIGNAL(customContextMenuRequested(QPoint)), this,
	        SLOT(contextMenuRequested(QPoint)));
	connect(tabBar, SIGNAL(tabCloseRequested(int)), this,
	        SLOT(tabCloseRequested(int)));
	TRACEPOINT;
}

void CallWindow::initFooter()
{
	TRACEPOINT;
	leftFooter = new QLabel();
	rightFooter = new QLabel();
	QStatusBar *bar = statusBar();
	bar->addPermanentWidget(leftFooter, 2);
	bar->addPermanentWidget(rightFooter, 2);
	TRACEPOINT;
}

void CallWindow::showExitProgramButton()
{
	TRACEPOINT;
	stepButton->setVisible(false);
	fastForwardButton->setVisible(false);
	TRACEPOINT;
}

void CallWindow::addTab(CallTab *tab)
{
	TRACEPOINT;
	tabMap[tab->getId()] = tab;
	QString name = QString("[%1] %2").arg(tab->getId()).arg(tab->getName());
	int index =
	    tabWidget->addTab(tab, stfl::shortenString(name, 20, true, true));
	tabWidget->getTabBar()->setTabData(index, QVariant((int)tab->getId()));
	TRACEPOINT;
}

size_t CallWindow::getId()
{
	return id;
}

void CallWindow::removeTab(CallTab *tab)
{
	TRACEPOINT;
	tabMap.erase(tabMap.find(tab->getId()));
	int index = tabWidget->indexOf(tab);
	tabWidget->removeTab(index);
	TRACEPOINT;
}

void CallWindow::removeTab(size_t tabId)
{
	TRACEPOINT;
	if (hasTab(tabId))
	{
		removeTab(tabMap[tabId]);
	}
	TRACEPOINT;
}

void CallWindow::showTab(CallTab *tab)
{
	TRACEPOINT;
	tabWidget->setCurrentWidget(tab);
	TRACEPOINT;
}

void CallWindow::showTab(size_t tabId)
{
	TRACEPOINT;
	if (hasTab(tabId))
	{
		showTab(tabMap[tabId]);
	}
	TRACEPOINT;
}

void CallWindow::updateLeftFooter(QString newText)
{
	TRACEPOINT;
	leftFooter->setText(newText);
	TRACEPOINT;
}

void CallWindow::updateRightFooter(QString newText)
{
	rightFooter->setText(newText);
}

void CallWindow::step()
{
	TRACEPOINT;
	controller->resumeProgramExecution();
	TRACEPOINT;
}

void CallWindow::fastForward()
{
	TRACEPOINT;
	controller->setMode(controller::Mode::FAST_FORWARD);
}

void CallWindow::closeApp()
{
	TRACEPOINT;
	controller->setMode(controller::Mode::HIDE);
}

bool CallWindow::hasTab(size_t tabId)
{
	TRACEPOINT;
	return tabMap.count(tabId);
}

void CallWindow::contextMenuRequested(const QPoint &location)
{
	TRACEPOINT;
	controller->removeEmptyWindows();
	auto tabBar = tabWidget->getTabBar();
	int tabIndex = tabBar->tabAt(location);
	if (tabIndex == tabOffset - 1)
		return;
	QMenu *menu = new QMenu(this);
	connect(menu, SIGNAL(triggered(QAction *)), this,
	        SLOT(contextMenuAction(QAction *)));
	auto windows = controller->getTabWindows();
	menu->addAction(new QAction("Remove call", this));
	menu->addAction(new QAction("Close tab", this));
	menu->addAction(new QAction("Open in new window", this));
	for (auto window : windows)
	{
		if (window->getId() != id)
		{
			menu->addAction(new QAction(
			    QString("Open in '%1'").arg(window->windowTitle()),
			    this));
		}
	}
	currentContextMenuTabId = getCallTabIdByTabIndex(tabIndex);
	menu->popup(tabBar->mapToGlobal(location));
	TRACEPOINT;
}

void CallWindow::contextMenuAction(QAction *action)
{
	TRACEPOINT;
	if (currentContextMenuTabId == -1)
	{
		return;
	}
	auto text = action->text();
	if (text == "Open in new window")
	{
		TRACEPOINT;
		controller->moveCallTabToNewWindow(currentContextMenuTabId);
		TRACEPOINT;
	}
	else if (text == "Remove call")
	{
		TRACEPOINT;
		controller->removeCallTab(currentContextMenuTabId, true, true);
		TRACEPOINT;
	}
	else if (text == "Close tab")
	{
		controller->removeCallTab(currentContextMenuTabId);
	}
	else
	{
		TRACEPOINT;
		auto windows = controller->getTabWindows();
		TRACEPOINT;
		for (auto window : windows)
		{
			TRACEPOINT;
			if (text ==
			    QString("Open in '%1'").arg(window->windowTitle()))
			{
				TRACEPOINT;
				controller->moveCallTabToWindow(
				    currentContextMenuTabId, window->getId());
				TRACEPOINT;
				break;
			}
			TRACEPOINT;
		}
	}
	currentContextMenuTabId = -1;
	TRACEPOINT;
}

size_t CallWindow::tabCount()
{
	return tabMap.size();
}

std::vector<size_t> CallWindow::getCallTabIds()
{
	TRACEPOINT;
	std::vector<size_t> ids{};
	for (auto &elem : tabMap)
	{
		ids.push_back(elem.first);
	}
	TRACEPOINT;
	return ids;
}

void CallWindow::closeEvent(QCloseEvent *event)
{
	TRACEPOINT;
	controller->removeWindowFromMaps(id);
	TRACEPOINT;
	// FIXME: tabWidget is already freed sometimes: Use-after-free Bug
	tabWidget->clear();
	TRACEPOINT;
	for (auto &elem : tabMap)
	{
		DEBUGF("Removing call Tab %s at address %s", elem.first,
		       size_t(elem.second));
		controller->removeCallTab(elem.first, true);
		TRACEPOINT;
	}
	TRACEPOINT;
	event->accept();
	TRACEPOINT;
}

void CallWindow::tabCloseRequested(int index)
{
	TRACEPOINT;
	if (hasTabAtIndex(index))
	{
		controller->removeCallTab(getCallTabIdByTabIndex(index));
	}
	TRACEPOINT;
	controller->removeEmptyWindows();
	TRACEPOINT;
}

size_t CallWindow::getCallTabIdByTabIndex(int index)
{
	DEBUG(index);
	if (hasTabAtIndex(index))
	{
		auto tabData = tabWidget->getTabBar()->tabData(index);
		bool ok = true;
		size_t callTabId = tabData.toInt(&ok);
		if (ok && tabMap.count(callTabId) > 0)
		{
			DEBUG(callTabId);
			return callTabId;
		}
	}
	TRACEPOINT;
	return 0;
}

bool CallWindow::hasTabAtIndex(int index)
{
	auto tabData = tabWidget->getTabBar()->tabData(index);
	return tabData != 0 && !tabData.isNull() && tabData.isValid();
}
}
}
