#include "view_controller.hpp"
#include <stdexcept>

namespace cvv {
namespace controller {

ViewController::ViewController()
{
}

ViewController::~ViewController()
{
}

void ViewController::addCallType(const QString typeName,
		std::function<CallTab(QString, impl::Call)> constr)
{
	ViewController::callTabType[typeName] = constr;
}

std::map<QString, std::function<CallTab(QString, impl::Call) >> ViewController::callTabType =
	*(new std::map<QString, std::function<CallTab(QString, impl::Call) >>());

void ViewController::addCall(const impl::Call &data)
{
	(void) data;
}

void ViewController::close()
{
	running = false;
}

void ViewController::exec()
{

}

impl::Call ViewController::getCall(size_t id)
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

const std::vector<TabWindow> ViewController::getTabWindows()
{
	return tabWindows;
}

void ViewController::moveCallTabToNewWindow(size_t tabId)
{
	(void) tabId;
}

size_t ViewController::moveCallTabToWindow(size_t tabId, size_t windowId)
{
	(void) tabId;
	(void) windowId;
	return 0;
}

void ViewController::openHelpBrowser(const QString &topic)
{
	(void) topic;
}

void resumeProgramExecution()
{

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
	(void)tabId;
}

void ViewController::showOverview()
{

}

}
}