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

namespace cvv { namespace controller {

class CallTab {};

class TabWindow {};

class ViewController
{
public:
	
	ViewController();
	
	~ViewController();
	
	static void addCallType(const QString typeName,
		std::function<CallTab(QString, impl::Call)> constr);
	
	/**
	 * @todo implement
	 */
	void addCall(const impl::Call &data);
	
	/**
	 * @todo implement
	 */
	void addCallTabToWindow(const CallTab &tab, size_t windowId);
	
	void close();
	
	/**
	 * @todo implement
	 */
	void exec();
	
	impl::Call& getCall(size_t id);
	
	/**
	 * @todo implement
	 */
	QString getSetting(const QString &scope, const QString &key);
	
	const std::vector<TabWindow> getTabWindows();
	
	/**
	 * @todo implement
	 */
	void moveCallTabToNewWindow(size_t tabId);
	
	/**
	 * @todo implement
	 */
	size_t moveCallTabToWindow(size_t tabId, size_t windowId); 
	
	/**
	 * @todo implement
	 */
	void openHelpBrowser(const QString &topic);
	
	/**
	 * @todo implement
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
	
	static const int MAIN_WINDOW = -1;
	
private:
	
	static std::map<QString, std::function<CallTab(QString, cvv::impl::Call)>> callTabType;
	
	bool running = true;
	QSettings settings{"CVVisual", QSettings::IniFormat};
	std::vector<TabWindow> tabWindows;
	std::vector<CallTab> callTabs;
	std::vector<impl::Call> calls;

};

}}

#endif
