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

/**
 * @brief Window inheriting some call tabs with in a tab widget.
 */
class CallWindow : public QMainWindow 
{
	
	Q_OBJECT

public:
	/**
	 * @brief Contructs a new call window.
	 * @param controller view controller that this window belongs to
	 * @param id id of the window
	 */
	CallWindow(util::Reference<controller::ViewController> controller, size_t id);

	/**
	 * @brief Add a new tab to the inherited tab widget.
	 * @param tab new tab
	 */
	void addTab(CallTab *tab);
	
	/**
	 * @brief Get the id of this window.
	 * @return id of this window.
	 */
	size_t getId();

	/**
	 * @brief Remove the given tab from this window.
	 * @param given tab to remove
	 */
	void removeTab(CallTab *tab);

	/**
	 * @brief Remove the given tab from this window.
	 * @param id of the given tab
	 */
	void removeTab(size_t tabId);

	/**
	 * @brief Show the given tab.
	 * @param given tab
	 */
	void showTab(CallTab *tab);

	/**
	 * @brief Show the given tab.
	 * @param id of the given tab
	 */
	void showTab(size_t tabId);

	/**
	 * @brief Update the left footer with the given text.
	 * @param newText given text
	 * @note Might only updating the left most footer part.
	 */
	void updateLeftFooter(QString newText);

	/**
	 * @brief Update the right footer with the given text.
	 * @param newText given text
	 * @note Might only updating the right most footer part.
	 */
	void updateRightFooter(QString newText);

	/**
	 * @brief Examines whether or not the given is inherited in this window.
	 * @param id of the given tab
	 */
	bool hasTab(size_t tabId);

public slots:
	/**
	 * @brief Resume the execution of the original program.
	 * @note Should only be used by 'Resume Execution' like buttons (or menu items)
	 */
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