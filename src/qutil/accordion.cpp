#include "accordion.hpp"

namespace cvv{ namespace qtutil{

Accordion::Accordion(QWidget *parent):
	QWidget(parent), elements(), layout(), lastHandle(0){}




void Accordion::collapseAll(bool b)
{
	for(auto& elem: elements)
	{
		elem.second->collapse(b);
	}
}

void Accordion::hideAll(bool b)
{
	for(auto& elem: elements)
	{
		elem.second->setVisible(!b);
	}
}

Accordion::Handle Accordion::insert(const QString& title,QWidget& widget, bool isCollapsed,
			   std::size_t position)
{
	//get handle
	Handle handle = getNextHandle();
	//create element
	elements.emplace(handle, cvv::util::make_unique<Collapsable>(title, widget, isCollapsed));
	//insert element
	layout.insertWidget(position, &element(handle));
	//set last handle
	lastHandle = handle;
	return handle;
}

void Accordion::remove(Handle handle)
{
	layout.removeWidget(&element(handle));
	elements.erase(handle);
}

void Accordion::clear()
{
	//clear layout
	for(auto& elem: elements)
	{
		layout.removeWidget(elem.second.get());
	}
	elements.clear();
}

std::pair<QString, QWidget&> Accordion::pop(Handle handle)
{
	std::pair<QString, QWidget&> result{element(handle).title(), element(handle).widget()};
	remove(handle);
	return result;
}

std::vector<std::pair<QString, QWidget&>> Accordion::popAll()
{
	std::vector<std::pair<QString, QWidget&>> result{};
	for(auto& elem: elements)
	{
		result.push_back(std::pair<QString, QWidget&>{elem.second->title(),
							      elem.second->widget()});
	}
	clear();
	return result;
}

Accordion::Handle Accordion::getNextHandle()
{
	for(Handle i= lastHandle; i <= max_size(); i++)
	{
		if(!isValidHandle(i))
		{
			return i;
		}
	}
	//check i<= lastHandle
	for(Handle i= 0; i <= lastHandle; i++)
	{
		if(!isValidHandle(i))
		{
			return i;
		}
	}
	throw std::length_error("Accordion reached max size already");
}
}} // end namespaces qtutil, cvv
