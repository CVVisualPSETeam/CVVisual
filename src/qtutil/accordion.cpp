#include "accordion.hpp"

#include <QScrollArea>

namespace cvv
{
namespace qtutil
{

Accordion::Accordion(QWidget *parent)
    : QWidget{ parent }, elements_{}, layout_{ nullptr }
{
	TRACEPOINT;
	auto lay = util::make_unique<QVBoxLayout>();
	layout_ = *lay;
	layout_->setAlignment(Qt::AlignTop);

	// needed because scrollArea->setLayout(layout_); does not work
	auto resizehelper = util::make_unique<QWidget>();
	resizehelper->setLayout(lay.release());

	auto scrollArea = util::make_unique<QScrollArea>();
	scrollArea->setWidget(resizehelper.release());
	// needed because no contained widget demands a size
	scrollArea->setWidgetResizable(true);

	auto mainLayout = util::make_unique<QVBoxLayout>();
	mainLayout->addWidget(scrollArea.release());
	setLayout(mainLayout.release());
	TRACEPOINT;
}

void Accordion::collapseAll(bool b)
{
	TRACEPOINT;
	for (auto &elem : elements_)
	{
		elem.second->collapse(b);
	}
	TRACEPOINT;
}

void Accordion::hideAll(bool b)
{
	TRACEPOINT;
	for (auto &elem : elements_)
	{
		elem.second->setVisible(!b);
	}
	TRACEPOINT;
}

Accordion::Handle Accordion::insert(const QString &title,
                                    std::unique_ptr<QWidget> widget,
                                    bool isCollapsed, std::size_t position)
{
	TRACEPOINT;
	// create element
	auto widgetPtr = widget.get();
	elements_.emplace(
	    widgetPtr, util::make_unique<Collapsable>(title, std::move(widget),
	                                              isCollapsed).release());
	// insert element
	layout_->insertWidget(position, &element(widgetPtr));
	TRACEPOINT;
	return widgetPtr;
}

void Accordion::remove(Handle handle)
{
	TRACEPOINT;
	Collapsable *elem = &element(handle);
	layout_->removeWidget(elem);
	elements_.erase(handle);
	elem->setParent(0);
	elem->deleteLater();
	TRACEPOINT;
}

void Accordion::clear()
{
	TRACEPOINT;
	// clear layout
	for (auto &elem : elements_)
	{
		layout_->removeWidget(elem.second);
		elem.second->setParent(nullptr);
		elem.second->deleteLater();
	}
	elements_.clear();
	TRACEPOINT;
}

std::pair<QString, Collapsable *> Accordion::pop(Handle handle)
{
	TRACEPOINT;
	Collapsable *elem = &element(handle);
	// remove from layout
	layout_->removeWidget(elem);
	std::pair<QString, Collapsable *> result{ element(handle).title(),
		                                  elem };
	// remove from map
	elements_.erase(handle);
	TRACEPOINT;
	return result;
}

std::vector<std::pair<QString, Collapsable *>> Accordion::popAll()
{
	TRACEPOINT;
	std::vector<std::pair<QString, Collapsable *>> result{};
	for (auto &elem : elements_)
	{
		// remove from layout
		layout_->removeWidget(elem.second);
		result.push_back(
		    std::pair<QString, Collapsable *>{ elem.second->title(),
			                               elem.second });
	}
	// remove from map
	elements_.clear();
	TRACEPOINT;
	return result;
}
}
} // end namespaces qtutil, cvv
