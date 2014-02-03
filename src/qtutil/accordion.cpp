#include "accordion.hpp"

#include<QScrollArea>

namespace cvv{ namespace qtutil{

Accordion::Accordion(QWidget *parent):
	QWidget{parent}, elements_{}, layout_{new QVBoxLayout{}}
{
	layout_->setAlignment(Qt::AlignTop);
	setLayout(layout_);

	//needed because scrollArea->setLayout(layout_); does not work
	QWidget* resizehelper= new QWidget{};
	resizehelper->setLayout(layout_);

	QScrollArea* scrollArea = new QScrollArea{};
	scrollArea->setWidget(resizehelper);
	//needed because no contained widget demands a size
	scrollArea->setWidgetResizable(true);

	QVBoxLayout* mainLayout = new QVBoxLayout{};
	mainLayout->addWidget(scrollArea);
	setLayout(mainLayout);
}

void Accordion::collapseAll(bool b)
{
	for(auto& elem: elements_)
	{
		elem.second->collapse(b);
	}
}

void Accordion::hideAll(bool b)
{
	for(auto& elem: elements_)
	{
		elem.second->setVisible(!b);
	}
}

Accordion::Handle Accordion::insert(const QString& title, std::unique_ptr<QWidget> widget, bool isCollapsed,
			   std::size_t position)
{
	//create element
	auto widgetPtr = widget.get();
	elements_.emplace(widgetPtr, new Collapsable{title, std::move(widget), isCollapsed});
	//insert element
	layout_->insertWidget(position, &element(widgetPtr));
	return widgetPtr;
}

void Accordion::remove(Handle handle)
{
	Collapsable* elem = &element(handle);
	layout_->removeWidget(elem);
	elements_.erase(handle);
	elem->setParent(0);
	delete elem;
}

void Accordion::clear()
{
	//clear layout
	for(auto& elem: elements_)
	{
		layout_->removeWidget(elem.second);
		elem.second->setParent(0);
		delete elem.second;
	}
	elements_.clear();
}

std::pair<QString, Collapsable*> Accordion::pop(Handle handle)
{
	Collapsable* elem = &element(handle);
	//remove from layout
	layout_->removeWidget(elem);
	std::pair<QString, Collapsable*> result{element(handle).title(), elem};
	//remove from map
	elements_.erase(handle);
	return result;
}

std::vector<std::pair<QString, Collapsable*>> Accordion::popAll()
{
	std::vector<std::pair<QString, Collapsable*>> result{};
	for(auto& elem: elements_)
	{
		//remove from layout
		layout_->removeWidget(elem.second);
		result.push_back(std::pair<QString, Collapsable*>{elem.second->title(),
								elem.second});
	}
	//remove from map
	elements_.clear();
	return result;
}
}} // end namespaces qtutil, cvv
