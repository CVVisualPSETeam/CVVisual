#include "view_controller.hpp"

#include <stdexcept>
#include <iostream>

#include <QApplication>
#include <QDesktopServices>
#include <QUrl>

#include "../gui/call_tab.hpp"
#include "../gui/call_window.hpp"
#include "../gui/overview_panel.hpp"
#include "../gui/main_call_window.hpp"

namespace cvv {
namespace controller {

char *emptyArray[] = {nullptr};

ViewController::ViewController()
{
	if(!QApplication::instance()) {
		int zero = 0;
		new QApplication{zero, emptyArray};
	}
	ovPanel = new gui::OverviewPanel{this};
	mainWindow = new gui::MainCallWindow(util::makeRef<ViewController>(*this), 0, ovPanel);
	windowMap[0] = std::unique_ptr<gui::CallWindow>(mainWindow);
	mainWindow->show();
	max_window_id = 0;
}

void ViewController::addCallType(const QString typeName,
		TabFactory constr)
{
	ViewController::callTabType[typeName] = constr;
}

std::map<QString, TabFactory> ViewController::callTabType;

void ViewController::addCall(util::Reference<impl::Call> data)
{
	mainWindow->showOverviewTab();
	ovPanel->addElement(*data);
	calls.push_back(data);
}

void ViewController::exec()
{
	QApplication::instance()->exec();
}

impl::Call& ViewController::getCall(size_t id)
{
	return *calls.at(id);
}

QString ViewController::getSetting(const QString &scope, const QString &key) const
{
	QString _key = scope + "/" + key;
	if (!settings.contains(_key))
	{
		throw std::invalid_argument{ "there is no such setting" };
	}
	QString set = settings.value(_key).value<QString>();
	return set;
}

std::vector<util::Reference<gui::CallWindow>> ViewController::getTabWindows()
{
	std::vector<util::Reference<gui::CallWindow>>  windows{};
	for (auto& it: windowMap)
	{
		windows.push_back(util::makeRef(*(it.second)));
	}
	return windows;
}

void ViewController::moveCallTabToNewWindow(size_t tabId)
{
	auto newWindow = util::make_unique<gui::CallWindow>(util::makeRef<ViewController>(*this), ++max_window_id);
	removeCallTab(tabId);
	newWindow->addTab(getCallTab(tabId));
	newWindow->show();
	windowMap[max_window_id] = std::move(newWindow);
	removeEmptyWindows();
}

void ViewController::moveCallTabToWindow(size_t tabId, size_t windowId)
{
	removeCallTab(tabId);
	auto *tab = getCallTab(tabId);
	windowMap[windowId]->addTab(tab);
	removeEmptyWindows();
}

void ViewController::removeCallTab(size_t tabId, bool deleteIt)
{
	auto *curWindow = getCurrentWindowOfTab(tabId);
	if (curWindow->hasTab(tabId))
	{
		getCurrentWindowOfTab(tabId)->removeTab(tabId);
		if (deleteIt)
		{
			callTabMap.erase(tabId);
		}
		removeEmptyWindows();
	}
}

void ViewController::openHelpBrowser(const QString &topic) const
{
	auto topicEncoded = QUrl::toPercentEncoding(topic);
	QDesktopServices::openUrl(QUrl(QString("http://cvv.mostlynerdless.de/help.php?topic=") + topicEncoded));
}

void ViewController::resumeProgramExecution()
{
	QApplication::instance()->exit();
}

void ViewController::setDefaultSetting(const QString &scope, const QString &key, const QString &value)
{
	QString _key = scope + "/" + key;
	if (!settings.contains(_key))
	{
		settings.setValue(_key, value);
	}
}

void ViewController::setSetting(const QString &scope, const QString &key, const QString &value)
{
	QString _key = scope + "/" + key;
	settings.setValue(_key, value);
}

void ViewController::showCallTab(size_t tabId)
{
	auto *window = getCurrentWindowOfTab(tabId);
	window->showTab(tabId);
	window->raise();
}

void ViewController::showAndOpenCallTab(size_t tabId)
{
	auto curWindow = getCurrentWindowOfTab(tabId);
	if (!curWindow->hasTab(tabId))
	{
		moveCallTabToWindow(tabId, 0);
		curWindow = mainWindow;
	}
	curWindow->showTab(tabId);
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
			return elem.second.get();
		}
	}
	return mainWindow;
}

gui::CallTab* ViewController::getCallTab(size_t tabId)
{
	if (callTabMap.count(tabId) == 0)
	{
		auto call = calls.at(tabId);
		if (callTabType.count(call->type()) == 0)
		{
			throw std::invalid_argument{ "no such type '" + call->type().toStdString() + "'" };
			exit(1);
		}
		callTabMap[tabId] = callTabType[call->type()](call, *this);
	}
	return callTabMap[tabId].get();
}

void ViewController::removeWindowFromMaps(size_t windowId)
{
	if (windowMap.count(windowId) > 0)
	{
		windowMap.erase(windowId);
	}
}

void ViewController::removeEmptyWindows()
{
	std::vector<size_t> remIds{};
	for (auto &elem : windowMap)
	{
		if (elem.second->tabCount() == 0 && elem.second->getId() != 0)
		{
			remIds.push_back(elem.first);
		}
	}
	for (auto windowId : remIds)
	{
		windowMap.erase(windowId);
	}
}

void ViewController::showExitProgramButton()
{
	for (auto &elem : windowMap)
	{
		elem.second->showExitProgramButton();
	}
}

}}
