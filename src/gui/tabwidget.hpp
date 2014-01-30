#ifndef CVVISUAL_TABWIDGET
#define CVVISUAL_TABWIDGET

#include <QTabWidget>
#include <QTabBar>

namespace cvv { namespace gui {

/**
 * A simple to QTabWidget Subclass, enabling the access to protected members.
 */
class TabWidget : public  QTabWidget {

public:
	
	TabWidget(QWidget *parent): QTabWidget(parent) {};

	QTabBar* getTabBar() const { return tabBar(); }	

};

}}

#endif
