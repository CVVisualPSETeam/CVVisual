#include "main_call_window.hpp"

#include <QTimer>

#include "../dbg/dbg.hpp"

namespace cvv { namespace gui {

MainCallWindow::MainCallWindow(util::Reference<controller::ViewController> controller,
							   size_t id, OverviewPanel *ovPanel)
								: CallWindow(controller, id), ovPanel{ovPanel}
{
	TRACEPOINT;
	tabOffset = 1;
	tabWidget->insertTab(0, ovPanel, "Overview");
    auto *tabBar = tabWidget->getTabBar();
    tabBar->tabButton(0,  QTabBar::RightSide)->hide();
    setWindowTitle(QString("CVVisual | main window"));
	QTimer *timer = new QTimer(this);
	connect(timer, SIGNAL(timeout()), this, SLOT(doPeriodically()));
	timer->start(100);
    TRACEPOINT;
}
	
void MainCallWindow::showOverviewTab()
{
	TRACEPOINT;
	tabWidget->setCurrentWidget(ovPanel);
}

void MainCallWindow::closeEvent(QCloseEvent *event)
{
	TRACEPOINT;
    controller->resumeProgramExecution();
    event->ignore();
    TRACEPOINT;
}

void MainCallWindow::doPeriodically()
{
	controller->removeEmptyWindows();
}

}}

