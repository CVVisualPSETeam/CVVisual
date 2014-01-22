#include "view_controller.hpp"

#include <stdexcept>

#include <QApplication>
#include <QDesktopServices>
#include <QUrl>

#include "../gui/call_tab.hpp"
#include "../gui/call_window.hpp"
#include "../gui/overview_panel.hpp"
#include "../gui/main_call_window.hpp"

namespace cvv {
namespace controller {

ViewController::ViewController(): windowMap{},
	ovPanel{new gui::OverviewPanel{this}},
	mainWindow{new gui::MainCallWindow{util::makeRef<ViewController>(*this), 0, ovPanel}},
	callTabMap{},calls{}
{
	windowMap[0] = mainWindow;
	max_window_id = 0;
}

ViewController::~ViewController()
{
	delete mainWindow;
	/*delete &windowMap;
	delete &callTabMap;*/
}

void ViewController::addCallType(const QString typeName,
		std::function<gui::CallTab(QString, impl::Call)> constr)
{
	ViewController::callTabType[typeName] = constr;
}

std::map<QString, std::function<gui::CallTab(QString, impl::Call) >> ViewController::callTabType =
	*(new std::map<QString, std::function<gui::CallTab(QString, impl::Call) >>());

void ViewController::addCall(const impl::Call &data)
{
	mainWindow->showOverviewTab();
	ovPanel->addElement(util::makeRef(data));
}

void ViewController::exec()
{
	QApplication::exec();
}

impl::Call& ViewController::getCall(size_t id)
{
	return calls.at(id);
}

QString ViewController::getSetting(const QString &scope, const QString &key)
{
	settings.beginGroup(scope);
	if (!settings.contains(key))
	{
		throw std::invalid_argument{ "there is no such setting" };
	}
	QString set = settings.value(scope + "/" + key).value<QString>();
	settings.endGroup();
	return set;
}

const std::map<size_t, gui::CallWindow*> ViewController::getTabWindows()
{
	return windowMap;
}

void ViewController::moveCallTabToNewWindow(size_t tabId)
{
	gui::CallWindow *newWindow = new gui::CallWindow(util::makeRef<ViewController>(*this),
		       	++max_window_id);
	newWindow->addTab(callTabMap[tabId]);
	windowMap[max_window_id] = newWindow;
	getCurrentWindowOfTab(tabId)->removeTab(tabId);
}

void ViewController::moveCallTabToWindow(size_t tabId, size_t windowId)
{
	getCurrentWindowOfTab(tabId)->removeTab(tabId);
	windowMap[windowId]->addTab(callTabMap[tabId]);
}

void ViewController::openHelpBrowser(const QString &topic) const
{
	(void) topic;
	//auto topicEncoded = QUrl::toPercentEncoding(topic);
	//QDesktopServices::openUrl(QUrl("http://cvv.mostlynerdless.de/help.php?" + topicEncoded));
}

void ViewController::resumeProgramExecution()
{
	QApplication::exit();
}

void ViewController::setDefaultSetting(const QString &scope, const QString &key, const QString &value)
{
	settings.beginGroup(scope);
	if (!settings.contains(key))
	{
		settings.setValue(key, value);
	}
	settings.endGroup();
}

void ViewController::setSetting(const QString &scope, const QString &key, const QString &value)
{
	settings.beginGroup(scope);
	settings.setValue(key, value);
	settings.endGroup();
}

void ViewController::showCallTab(size_t tabId)
{
	auto window = getCurrentWindowOfTab(tabId);
	window->showTab(tabId);
	window->raise();
}

void ViewController::showOverview()
{
	mainWindow->raise();
	mainWindow->showOverviewTab();
}


gui::CallWindow* ViewController::getCurrentWindowOfTab(size_t tabId)
{
	for (auto &elem  : windowMap)
	{
		if (elem.second->hasTab(tabId))
		{
			return elem.second;
		}
	}
	return mainWindow;
}

}}
