#ifndef CVVISUAL_CALLWINDOW_HPP
#define CVVISUAL_CALLWINDOW_HPP

#include <QTabWidget>
#include <QMainWindow>
#include <QString>
#include <vector>
#include <QLabel>
#include <QKeyEvent>

#include "call_tab.hpp"
#include "../controller/view_controller.hpp"
#include "../util/util.hpp"

namespace cvv { 

namespace controller {
	class ViewController;
}

namespace gui {

class CallWindow : public QMainWindow 
{
	
	Q_OBJECT

public:
	CallWindow(util::Reference<controller::ViewController> controller, size_t id);

	void addTab(CallTab *tab);
	
	size_t getId();

	void removeTab(CallTab *tab);

	void removeTab(size_t tabId);

	void showTab(CallTab *tab);

	void showTab(size_t tabId);

	void updateLeftFooter(QString newText);

	void updateRightFooter(QString newText);

	bool hasTab(size_t tabId);

public slots:
	void resumeProgramExecution(); 

protected:
	
	size_t id;
	util::Reference<controller::ViewController> controller;
	QTabWidget *tabWidget;
	QMainWindow *window;
	std::map<size_t, CallTab*> tabMap; 
	QLabel *leftFooter;
	QLabel *rightFooter;
	
	void initMenu();
	
	void initTabs();
	
	void initFooter();
};

}}

#endif
