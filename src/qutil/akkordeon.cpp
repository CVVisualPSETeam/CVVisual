#include "akkordeon.hpp"

namespace cvv{ namespace qtutil{

Akkordeon::Akkordeon(QWidget *parent):
	QWidget(parent), elements(), layout(), lastHandle(0){}




void Akkordeon::collapseAll(bool b)
{
	for(auto& elem: elements)
	{
		elem.second->collapse(b);
	}
}

void Akkordeon::hideAll(bool b)
{
	for(auto& elem: elements)
	{
		elem.second->setVisible(!b);
	}
}

Akkordeon::handle_t Akkordeon::insert(const QString& title,QWidget& widget, bool isCollapsed,
			   std::size_t position)
{
	//get handle
	handle_t handle = getNextHandle();
	//create element
	elements.emplace(handle, cvv::util::make_unique<Collapsable>(title, widget, isCollapsed));
	//insert element
	layout.insertWidget(position, &element(handle));
	//set last handle
	lastHandle = handle;
	return handle;
}

void Akkordeon::remove(handle_t handle)
{
	layout.removeWidget(&element(handle));
	elements.erase(handle);
}

void Akkordeon::clear()
{
	//clear layout
	for(auto& elem: elements)
	{
		layout.removeWidget(elem.second.get());
	}
	elements.clear();
}

std::pair<QString, QWidget&> Akkordeon::pop(handle_t handle)
{
	std::pair<QString, QWidget&> result{element(handle).title(), element(handle).widget()};
	remove(handle);
	return result;
}

std::vector<std::pair<QString, QWidget&>> Akkordeon::popAll()
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

Akkordeon::handle_t Akkordeon::getNextHandle()
{
	for(handle_t i= lastHandle; i <= max_size(); i++)
	{
		if(!isValidHandle(i))
		{
			return i;
		}
	}
	//check i<= lastHandle
	for(handle_t i= 0; i <= lastHandle; i++)
	{
		if(!isValidHandle(i))
		{
			return i;
		}
	}
	throw std::length_error("Akkordeon reached max size already");
}
}} // end namespaces qtutil, cvv
