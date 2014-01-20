#ifndef CVVISUAL_VIEWCONTROLLER_HPP
#define	CVVISUAL_VIEWCONTROLLER_HPP

#include <vector>
#include <algorithm>
#include <iostream>
#include <map>
#include <QString>
#include <functional>
#include <utility>
#include <QSettings>

#include "../impl/call.hpp"
#include "../util/util.hpp"

namespace cvv { 

namespace gui {
	class CallTab;
	class CallWindow;
	class MainCallWindow;
	class OverviewPanel;
}

namespace controller {

class ViewController
{
public:
	
	ViewController();
	
	~ViewController();
	
	static void addCallType(const QString typeName,
		std::function<gui::CallTab(QString, impl::Call)> constr);
	
	void addCall(const impl::Call &data);
		
	void exec();
	
	impl::Call& getCall(size_t id);

	QString getSetting(const QString &scope, const QString &key);
	
	const std::map<size_t, gui::CallWindow*> getTabWindows();
	
	/**
	 * @todo implement
	 */
	void moveCallTabToNewWindow(size_t tabId);
	
	/**
	 * @todo implement
	 */
	void moveCallTabToWindow(size_t tabId, size_t windowId); 
	
	/**
	 * @todo implement
	 */
	void openHelpBrowser(const QString &topic);
	
	/**
	 * @brief Resume the execution of the calling program.
	 */
	void resumeProgramExecution();
	
	/**
	 * @todo implement
	 */
	void setDefaultSetting(const QString &scope, const QString &key, const QString &value); 
	
	/**
	 * @todo implement
	 */
	void setSetting(const QString &scope, const QString &key, const QString &value);
	
	/**
	 * @todo implement
	 */
	void showCallTab(size_t tabId);
	
	/**
	 * @todo implement
	 */
	void showOverview(); 
	
	gui::CallWindow* getCurrentWindowOfTab(size_t tabId);

	static const int MAIN_WINDOW = -1;
	
private:
	
	static std::map<QString, std::function<gui::CallTab(QString, impl::Call)>> callTabType;
	
	QSettings settings{"CVVisual", QSettings::IniFormat};
	std::map<size_t, gui::CallWindow*> windowMap;
	gui::OverviewPanel *ovPanel;
	gui::MainCallWindow *mainWindow;
	std::map<size_t, gui::CallTab*> callTabMap;
	std::vector<impl::Call> calls;
	size_t max_window_id = 0;
};

}}

#endif
