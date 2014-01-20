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

class MainCallWindow : public CallWindow
{
	
	Q_OBJECT
	
public:
	MainCallWindow(util::Reference<controller::ViewController> controller, size_t id,
				 OverviewPanel *ovPanel);
	
	void showOverviewTab();

private:
	OverviewPanel *ovPanel;
};

}}
#endif

