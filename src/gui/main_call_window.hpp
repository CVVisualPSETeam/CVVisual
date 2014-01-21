#ifndef CVVISUAL_MAINCALLWINDOW_HPP
#define	CVVISUAL_MAINCALLWINDOW_HPP

#include "call_window.hpp"
#include "overview_panel.hpp"
#include "../controller/view_controller.hpp"
#include "../util/util.hpp"

namespace cvv { 

namespace controller {
	class ViewController;
}

namespace gui {

class OverviewPanel;

/**
 * @brief A call window also inheriting the overview panel. 
 */
class MainCallWindow : public CallWindow
{
	
	Q_OBJECT
	
public:
	/**
	 * @brief Constructs a new main call window.
	 * @param controller view controller inheriting this main window
	 * @param id id of this main window
	 * @param ovPanel inherited overview panel
	 */
	MainCallWindow(util::Reference<controller::ViewController> controller, size_t id,
				 OverviewPanel *ovPanel);
	
	/**
	 * @brief Show the overview tab.
	 */
	void showOverviewTab();

private:
	OverviewPanel *ovPanel;
};

}}
#endif

