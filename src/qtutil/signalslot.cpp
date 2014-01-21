#include "signalslot.hpp"


namespace cvv { namespace qtutil{

Signal::Signal(QObject* parent):
	QObject{parent}
{}

void Signal::emitSignal()
{
	emit signal();
}

Slot::Slot(const std::function<void()>& f, QObject* parent):
	QObject{parent}, function_{f}
{}

void Slot::slot()
{
	function_();
}
}} // end namespaces qtutil, cvv
