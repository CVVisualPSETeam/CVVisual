#include "main_call_window.hpp"

#include <QTimer>
#include <QPoint>

#include "../dbg/dbg.hpp"
#include "../util/util.hpp"

namespace cvv { namespace gui {

MainCallWindow::MainCallWindow(util::Reference<controller::ViewController> controller,
							   size_t id, OverviewPanel *ovPanel)
								: CallWindow(controller, id), ovPanel{ovPanel},
								  closeWindow{util::make_unique<CloseWindow>(controller)}
{
	TRACEPOINT;
	tabOffset = 1;
	tabWidget->insertTab(0, ovPanel, "Overview");
    auto *tabBar = tabWidget->getTabBar();
    tabBar->tabButton(0,  QTabBar::RightSide)->hide();
    setWindowTitle(QString("CVVisual | main window"));
	QTimer *timer = new QTimer(this);
	connect(timer, SIGNAL(timeout()), this, SLOT(removeEmptyWindowsDelayed()));	
	timer->start(125);
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
	if (!controller->hasFinalCall()){
		int x = (width() - closeWindow->width()) / 2;
		int y = (height() - closeWindow->height()) / 2;
		QPoint point{x, y};
		closeWindow->move(mapToGlobal(point));
		closeWindow->show();
		closeWindow->setWindowState( (closeWindow->windowState() & ~Qt::WindowMinimized) | Qt::WindowActive);
		closeWindow->raise();
		isCloseWindowHidden = false;
		event->ignore();
	}
    TRACEPOINT;
}

void MainCallWindow::removeEmptyWindowsDelayed()
{
	if (controller->shouldRunRemoveEmptyWindows())
	{
		if (removeEmptyWindowsDelayedCounter <= 0)
		{
			controller->removeEmptyWindows();
			removeEmptyWindowsDelayedCounter = 4;
		}
		else
		{
			removeEmptyWindowsDelayedCounter--;
		}
	}
	else
	{
		removeEmptyWindowsDelayedCounter = 4;
	}
}

void MainCallWindow::hideCloseWindow()
{
	DEBUG(isCloseWindowHidden);
	if (!isCloseWindowHidden)
	{
		TRACEPOINT;
		closeWindow->close();
		isCloseWindowHidden = true;
	}
}

}}

