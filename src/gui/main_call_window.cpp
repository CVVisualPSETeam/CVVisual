#include "main_call_window.hpp"

#include <QApplication>
#include <QPoint>

#include "../dbg/dbg.hpp"
#include "../util/util.hpp"
#include "../stfl/stringutils.hpp"

namespace cvv { namespace gui {

MainCallWindow::MainCallWindow(util::Reference<controller::ViewController> controller,
		size_t id, OverviewPanel *ovPanel):
	CallWindow(controller, id), ovPanel{ovPanel}
{
	TRACEPOINT;
	tabOffset = 1;
	QString name = "Overview";
	tabWidget->insertTab(0, ovPanel, name);
	auto *tabBar = tabWidget->getTabBar();
	tabBar->tabButton(0,  QTabBar::RightSide)->hide();
	setWindowTitle(QString("CVVisual | main window"));
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
	(void)event;
    controller->setMode(controller::Mode::HIDE);
	TRACEPOINT;
}

}}

