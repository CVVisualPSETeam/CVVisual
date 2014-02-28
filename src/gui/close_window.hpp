#ifndef CVVISUAL_CLOSE_WINDOW_HPP
#define CVVISUAL_CLOSE_WINDOW_HPP

#include <QDialog>

#include "../util/util.hpp"
#include "../controller/view_controller.hpp"
#include "../dbg/dbg.hpp"

namespace cvv {
namespace gui {

/**
 * @brief The window that popsup if you try to close the main window.
 */
class CloseWindow : public QDialog
{
	Q_OBJECT
	
public:
	
	/**
	 * @brief Constructor of this class.
	 * @param controller the controller which will be used for it's actions
	 */
	CloseWindow(util::Reference<controller::ViewController> controller);
	
	~CloseWindow() { TRACEPOINT; }
	
private slots:
	void helpRequested();
	void exitRequested();
	void fastForwardRequested();
	void hideCVVRequested();
	void resumeProgramExecution();
	
private:
	util::Reference<controller::ViewController> controller;
};

}
}

#endif
