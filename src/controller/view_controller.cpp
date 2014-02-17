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
#include "../gui/filter_call_tab.hpp"
#include "../gui/match_call_tab.hpp"
#include "../impl/filter_call.hpp"
#include "../impl/data_controller.hpp"
#include "../dbg/dbg.hpp"

namespace cvv {
namespace controller {

//It's only used for instatiating a QApplication.
char *emptyArray[] = {nullptr};

ViewController::ViewController()
{
	TRACEPOINT;
    if(!QApplication::instance()) {
        int zero = 0;
        new QApplication{zero, emptyArray};
    }
    ovPanel = new gui::OverviewPanel{util::makeRef<ViewController>(*this)};
    mainWindow = new gui::MainCallWindow(util::makeRef<ViewController>(*this), 0, ovPanel);
    windowMap[0] = std::unique_ptr<gui::CallWindow>(mainWindow);
    mainWindow->show();
    max_window_id = 0;
	TRACEPOINT;
}

void ViewController::addCallType(const QString typeName,
        TabFactory constr)
{
	TRACEPOINT;
    ViewController::callTabType[typeName] = constr;
	TRACEPOINT;
}

std::unique_ptr<cvv::gui::FilterCallTab> makeFilterCallTab(
		cvv::util::Reference<cvv::impl::Call> call,
		cvv::controller::ViewController& vc)
{
	TRACEPOINT;
	return cvv::util::make_unique<cvv::gui::FilterCallTab>(*call.castTo<cvv::impl::FilterCall>(), vc);
}

std::unique_ptr<cvv::gui::MatchCallTab> makeMatchCallTab(
		cvv::util::Reference<cvv::impl::Call> call,
		cvv::controller::ViewController& vc)
{
	TRACEPOINT;
	return cvv::util::make_unique<cvv::gui::MatchCallTab>(*call.castTo<cvv::impl::MatchCall>(), vc);
}

std::map<QString, TabFactory> ViewController::callTabType {
	{"filter", makeFilterCallTab},
	{"match", makeMatchCallTab}
};

void ViewController::addCall(util::Reference<impl::Call> data)
{
	TRACEPOINT;
	updateMode();
	if (mode == Mode::NORMAL)
	{
		mainWindow->showOverviewTab();
	}
	if (mode != Mode::HIDE)
	{
		ovPanel->addElement(*data);
	}
	TRACEPOINT;
}

void ViewController::exec()
{
	TRACEPOINT;
	updateMode();
	if (mode == Mode::NORMAL)
	{
		QApplication::instance()->exec();
	}
	TRACEPOINT;
}

impl::Call& ViewController::getCall(size_t id)
{
	TRACEPOINT;
    return impl::dataController().getCall(id);
}

QString ViewController::getSetting(const QString &scope, const QString &key) const
{
	TRACEPOINT;
    QString _key = scope + "/" + key;
    if (!settings.contains(_key))
    {
        throw std::invalid_argument{ "there is no such setting" };
    }
    QString set = settings.value(_key).value<QString>();
	TRACEPOINT;
    return set;
}

std::vector<util::Reference<gui::CallWindow>> ViewController::getTabWindows()
{
	TRACEPOINT;
    std::vector<util::Reference<gui::CallWindow>>  windows{};
    for (auto &it : windowMap)
    {
        windows.push_back(util::makeRef(*(it.second)));
    }
    TRACEPOINT;
    return windows;
}

util::Reference<gui::MainCallWindow> ViewController::getMainWindow()
{
    TRACEPOINT;
    return util::makeRef(*mainWindow);
}

void ViewController::moveCallTabToNewWindow(size_t tabId)
{
	TRACEPOINT;
    if (!hasCall(tabId))
        return;
    auto newWindow = util::make_unique<gui::CallWindow>(util::makeRef<ViewController>(*this), ++max_window_id);
    removeCallTab(tabId);
    newWindow->addTab(getCallTab(tabId));
    newWindow->show();
   	if (doesShowExitProgramButton)
	{
		newWindow->showExitProgramButton();
	}
   	windowMap[max_window_id] = std::move(newWindow);
   	removeEmptyWindowsWithDelay();
    TRACEPOINT;
}

void ViewController::moveCallTabToWindow(size_t tabId, size_t windowId)
{
	TRACEPOINT;
    if (!hasCall(tabId))
        return;
    removeCallTab(tabId);
    auto tab = getCallTab(tabId);
    windowMap[windowId]->addTab(tab);
    removeEmptyWindowsWithDelay();
    TRACEPOINT;
}

void ViewController::removeCallTab(size_t tabId, bool deleteIt, bool deleteCall)
{
	TRACEPOINT;
    auto *curWindow = getCurrentWindowOfTab(tabId);
    if (curWindow->hasTab(tabId))
    {
        getCurrentWindowOfTab(tabId)->removeTab(tabId);
        if (deleteIt)
        {
            callTabMap.erase(tabId);
        }
    }
    TRACEPOINT;
	if (deleteCall && hasCall(tabId))
    {
        ovPanel->removeElement(tabId);
        impl::dataController().removeCall(tabId);
    }
    removeEmptyWindowsWithDelay();
    TRACEPOINT;
}

void ViewController::openHelpBrowser(const QString &topic) const
{
	TRACEPOINT;
    auto topicEncoded = QUrl::toPercentEncoding(topic);
    QDesktopServices::openUrl(QUrl(QString("http://cvv.mostlynerdless.de/help.php?topic=") + topicEncoded));
    TRACEPOINT;
}

void ViewController::resumeProgramExecution()
{
	TRACEPOINT;
    QApplication::instance()->exit();
    TRACEPOINT;
}

void ViewController::setDefaultSetting(const QString &scope, const QString &key, const QString &value)
{
	TRACEPOINT;
    QString _key = scope + "/" + key;
    if (!settings.contains(_key))
    {
        settings.setValue(_key, value);
    }
    TRACEPOINT;
}

void ViewController::setSetting(const QString &scope, const QString &key, const QString &value)
{
	TRACEPOINT;
    QString _key = scope + "/" + key;
    settings.setValue(_key, value);
    TRACEPOINT;
}

void ViewController::showCallTab(size_t tabId)
{
	TRACEPOINT;
    auto *window = getCurrentWindowOfTab(tabId);
    window->showTab(tabId);
    window->setWindowState( (window->windowState() & ~Qt::WindowMinimized) | Qt::WindowActive);
    window->raise();
    TRACEPOINT;
}

void ViewController::showAndOpenCallTab(size_t tabId)
{
	TRACEPOINT;
    auto curWindow = getCurrentWindowOfTab(tabId);
    if (!curWindow->hasTab(tabId))
    {
        moveCallTabToWindow(tabId, 0);
        curWindow = mainWindow;
    }
    curWindow->showTab(tabId);
    TRACEPOINT;
}

void ViewController::showOverview()
{
	TRACEPOINT;
	mainWindow->setWindowState( (mainWindow->windowState() & ~Qt::WindowMinimized) | Qt::WindowActive);
    mainWindow->raise();
    mainWindow->showOverviewTab();
    TRACEPOINT;
}

gui::CallWindow* ViewController::getCurrentWindowOfTab(size_t tabId)
{
	TRACEPOINT;
    for (auto &elem  : windowMap)
    {
        if (elem.second->hasTab(tabId))
        {
            return elem.second.get();
        }
    }
    TRACEPOINT;
    return mainWindow;
}

gui::CallTab* ViewController::getCallTab(size_t tabId)
{
	TRACEPOINT;
    if (callTabMap.count(tabId) == 0)
    {
        auto *call = &(getCall(tabId));
        if (callTabType.count(call->type()) == 0)
        {
            throw std::invalid_argument{ "no such type '" + call->type().toStdString() + "'" };
        }
        callTabMap[tabId] = callTabType[call->type()](util::makeRef(*call), *this);
    }
    TRACEPOINT;
    return callTabMap[tabId].get();
}

void ViewController::removeWindowFromMaps(size_t windowId)
{
	TRACEPOINT;
    if (windowMap.count(windowId) > 0)
    {
        windowMap.erase(windowId);
    }
   	TRACEPOINT;
}

void ViewController::removeEmptyWindows()
{
	TRACEPOINT;
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
	shouldRunRemoveEmptyWindows_ = false;
	TRACEPOINT;
}

void ViewController::removeEmptyWindowsWithDelay()
{
	shouldRunRemoveEmptyWindows_ = true;
}
	
bool ViewController::shouldRunRemoveEmptyWindows()
{
	return shouldRunRemoveEmptyWindows_;
}
										  
void ViewController::showExitProgramButton()
{
	TRACEPOINT;
    for (auto &elem : windowMap)
    {
        elem.second->showExitProgramButton();
    }
	doesShowExitProgramButton = true;
	TRACEPOINT;
}

bool ViewController::hasCall(size_t id)
{
	TRACEPOINT;
	return impl::dataController().hasCall(id);
}
											  
void ViewController::setMode(Mode newMode)
{
	TRACEPOINT;
	DEBUG(newMode);
	mode = newMode;
	switch (newMode)
	{
		case Mode::NORMAL:
			break;
		case Mode::HIDE:
			hideAll();
			QApplication::instance()->exit();
			break;
		case Mode::FAST_FORWARD:
			if (!doesShowExitProgramButton)
			{
				QApplication::instance()->exit();
			}
			else
			{
				mode = Mode::NORMAL;
			}
			break;
	}
	TRACEPOINT;
}

Mode ViewController::getMode()
{
	return mode;
}

void ViewController::updateMode()
{
	TRACEPOINT;
	if (mode == Mode::FAST_FORWARD && hasFinalCall())
	{
		mode = Mode::NORMAL;
	}
	TRACEPOINT;
}

void ViewController::hideAll()
{
	TRACEPOINT;
	for (auto &window : windowMap)
    {
        window.second->hide();
    }
    TRACEPOINT;
}

bool ViewController::hasFinalCall()
{
	return doesShowExitProgramButton;
}

void ViewController::hideCloseWindow()
{
	mainWindow->hideCloseWindow();
}

}}
