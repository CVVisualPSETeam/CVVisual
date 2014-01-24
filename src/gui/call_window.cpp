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
	initMenu();
	initTabs();
	initFooter();
}

void CallWindow::initMenu()
{
	//auto *test = menuBar()->addMenu("Test menu");
	//test->addAction("Test");
}

void CallWindow::initTabs()
{
	tabWidget = new QTabWidget(this);
	setCentralWidget(tabWidget);
	QPushButton *button = new QPushButton("Resume program execution", this);
	//connect(button, SIGNAL(keyPressEvent(KeyEvent * e)), this, SLOT(resumeProgramExecution()));
	tabWidget->setCornerWidget(button, Qt::TopLeftCorner);
}

void CallWindow::initFooter()
{
	leftFooter = new QLabel();
	rightFooter = new QLabel();
	QStatusBar *bar = statusBar();
	bar->addPermanentWidget(leftFooter, 2);
	bar->addPermanentWidget(rightFooter, 2);
}

void CallWindow::addTab(CallTab *tab)
{
	tabMap[tab->getId()] = tab;
	tabWidget->addTab(tab, tab->getName());
}
	
size_t CallWindow::getId()
{
	return id;
}

void CallWindow::removeTab(CallTab *tab)
{
	tabMap.erase(tabMap.find(tab->getId()));
	tabWidget->removeTab(tabWidget->indexOf(tab));
}

void CallWindow::removeTab(size_t tabId)
{
	removeTab(tabMap[tabId]);
}

void CallWindow::showTab(CallTab *tab)
{
	tabWidget->setCurrentWidget(tab);
}

void CallWindow::showTab(size_t tabId)
{
	showTab(tabMap[tabId]);
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

}}
