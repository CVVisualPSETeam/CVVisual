#include "main_call_window.hpp"

namespace cvv { namespace gui {

MainCallWindow::MainCallWindow(util::Reference<controller::ViewController> controller,
							   size_t id, OverviewPanel *ovPanel)
								: CallWindow(controller, id), ovPanel{ovPanel}
{
	tabWidget->insertTab(0, ovPanel, "Overview");
    setWindowTitle(QString("CVVisual main window (no. %1)").arg(id));
}
	
void MainCallWindow::showOverviewTab()
{
	tabWidget->setCurrentWidget(ovPanel);
}

void MainCallWindow::closeEvent(QCloseEvent *event)
{
    controller->resumeProgramExecution();
    event->ignore();
}

}}

