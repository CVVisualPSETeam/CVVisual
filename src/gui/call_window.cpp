#include "call_window.hpp"

#include <QMenu>
#include <QStatusBar>
#include <QPushButton>

namespace cvv { 

namespace controller {
	class ViewController;
}

namespace gui {

CallWindow::CallWindow(util::Reference<controller::ViewController> controller, size_t id):
	id{id}, controller{controller}
{
	initTabs();
	initFooter();
    setWindowTitle(QString("CVVisual | window no. %1").arg(id));
	setMinimumWidth(600);
	setMinimumHeight(400);
}

void CallWindow::initTabs()
{
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
}

void CallWindow::initFooter()
{
	leftFooter = new QLabel();
	rightFooter = new QLabel();
	QStatusBar *bar = statusBar();
	bar->addPermanentWidget(leftFooter, 2);
	bar->addPermanentWidget(rightFooter, 2);
}

void CallWindow::showExitProgramButton()
{
	progButton->setText("Exit program");
	progButton->setStyleSheet("QPushButton {background-color: red; color: white;}");
}

void CallWindow::addTab(CallTab *tab)
{
	tabMap[tab->getId()] = tab;
	int index = tabWidget->addTab(tab, tab->getName());
	tabAtTabIndex[index] = tab;
}
	
size_t CallWindow::getId()
{
	return id;
}

void CallWindow::removeTab(CallTab *tab)
{
	tabMap.erase(tabMap.find(tab->getId()));
	int index = tabWidget->indexOf(tab);
	tabWidget->removeTab(index);
	tabAtTabIndex.erase(index);
}

void CallWindow::removeTab(size_t tabId)
{
    if (hasTab(tabId))
    {
        removeTab(tabMap[tabId]);
    }
}

void CallWindow::showTab(CallTab *tab)
{
	tabWidget->setCurrentWidget(tab);
}

void CallWindow::showTab(size_t tabId)
{
	if (hasTab(tabId))
	{
		showTab(tabMap[tabId]);
	}
}

void CallWindow::updateLeftFooter(QString newText)
{
	leftFooter->setText(newText);
}

void CallWindow::updateRightFooter(QString newText)
{
	rightFooter->setText(newText);
}

void CallWindow::resumeProgramExecution()
{
	controller->resumeProgramExecution();
}

bool CallWindow::hasTab(size_t tabId){
	return tabMap.count(tabId);
}

void CallWindow::contextMenuRequested(const QPoint &location)
{
	if (location.isNull())
		return;
	auto tabBar = tabWidget->getTabBar();
	int tabIndex = tabBar->tabAt(location);
	if (tabIndex == 0)
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
}

void CallWindow::contextMenuAction(QAction *action)
{
	if (currentContextMenuTabId == -1)
	{
		return;
	}
	auto text = action->text();
    if (text == "Open in new window")
	{
		controller->moveCallTabToNewWindow(currentContextMenuTabId);
	
	} if (text == "Remove call") {
    	controller->removeCallTab (currentContextMenuTabId, true, true);
	} else 
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
}

size_t CallWindow::tabCount()
{
	return tabMap.size();
}

std::vector<size_t> CallWindow::getCallTabIds()
{
	std::vector<size_t> ids{};
	for (auto &elem : tabMap)
	{
		ids.push_back(elem.first);
	}
	return ids;
}

void CallWindow::closeEvent(QCloseEvent *event)
{
	controller->removeWindowFromMaps(id);
	tabWidget->clear();
	for (auto &elem : tabMap)
	{
		controller->removeCallTab(elem.first, true);
	}
	event->accept();
}

void CallWindow::tabCloseRequested(int index)
{
    if (tabAtTabIndex.count(index) >= 1)
    {
        controller->removeCallTab(tabAtTabIndex[index]->getId());
    }
}

}}
