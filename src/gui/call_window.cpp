#include "call_window.hpp"

#include <QMenu>
#include <QStatusBar>
#include <QPushButton>

#include "../dbg/dbg.hpp"

namespace cvv { 

namespace controller {
	class ViewController;
}

namespace gui {

CallWindow::CallWindow(util::Reference<controller::ViewController> controller, size_t id):
	id{id}, controller{controller}
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
	setCentralWidget(tabWidget);
	progButton = new QPushButton("Resume program execution", this);
	progButton->setStyleSheet("QPushButton {background-color: green; color: white;}");
	connect(progButton, SIGNAL(clicked()), this, SLOT(resumeProgramExecution()));
	tabWidget->setCornerWidget(progButton, Qt::TopLeftCorner);
	auto *tabBar = tabWidget->getTabBar();
	tabBar->setContextMenuPolicy(Qt::CustomContextMenu);
	connect(tabBar, SIGNAL(customContextMenuRequested(QPoint)), this, SLOT(contextMenuRequested(QPoint)));
    connect(tabBar, SIGNAL(tabCloseRequested(int)), this, SLOT(tabCloseRequested(int)));
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
	progButton->setText("Exit program");
	progButton->setStyleSheet("QPushButton {background-color: red; color: white;}");
	TRACEPOINT;
}

void CallWindow::addTab(CallTab *tab)
{
	TRACEPOINT;
	tabMap[tab->getId()] = tab;
	int index = tabWidget->addTab(tab, tab->getName());
	tabAtTabIndex[index] = tab;
	TRACEPOINT;
}
	
size_t CallWindow::getId()
{
	TRACEPOINT;
	return id;
}

void CallWindow::removeTab(CallTab *tab)
{
	TRACEPOINT;
	tabMap.erase(tabMap.find(tab->getId()));
	int index = tabWidget->indexOf(tab);
	tabWidget->removeTab(index);
	tabAtTabIndex.erase(index);
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

void CallWindow::resumeProgramExecution()
{
	TRACEPOINT;
	controller->resumeProgramExecution();
	TRACEPOINT;
}

bool CallWindow::hasTab(size_t tabId){
	TRACEPOINT;
	return tabMap.count(tabId);
}

void CallWindow::contextMenuRequested(const QPoint &location)
{
	TRACEPOINT;
	auto tabBar = tabWidget->getTabBar();
	int tabIndex = tabBar->tabAt(location);
	if (tabIndex == tabOffset - 1)
		return;
	QMenu *menu = new QMenu(this);
	connect(menu, SIGNAL(triggered(QAction*)), this, SLOT(contextMenuAction(QAction*)));
    auto windows = controller->getTabWindows();
    menu->addAction(new QAction("Remove call", this));
	menu->addAction(new QAction("Open in new window", this));
	for (auto window : windows)
	{
		if (window->getId() != id)
		{
			menu->addAction(new QAction(QString("Open in '%1'").arg(
						window->windowTitle()), this));
		}
	}
	currentContextMenuTabId = tabAtTabIndex[tabIndex]->getId();  
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
	if (text == "Remove call") 
	{
		TRACEPOINT;
    	controller->removeCallTab(currentContextMenuTabId, true, true);
    	TRACEPOINT;
	} 
	else 
	{
		auto windows = controller->getTabWindows();
		for (auto window : windows)
		{
			if (text == QString("Open in '%1'").arg(window->windowTitle()))
			{
				controller->moveCallTabToWindow(currentContextMenuTabId, window->getId());
				break;
			}
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
	//tabWidget->clear();
	TRACEPOINT;
	for (auto &elem : tabMap)
	{
		DEBUGF("Removing call Tab %s at address %s", elem.first, size_t(elem.second));
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
    if (tabAtTabIndex.count(index) >= 1)
    {
        controller->removeCallTab(tabAtTabIndex[index]->getId());
    }
    TRACEPOINT;
}

}}
