#include "collapsable.hpp"

namespace cvv{ namespace qtutil{

Collapsable::Collapsable(const QString& title,QWidget& widget, bool isCollapsed, QWidget *parent):
		QWidget{parent}, title_{title}, widget_(widget)
	//	,button{this}, layoutHeaderAndWidget{this}, layoutHeader{&layoutHeaderAndWidget}
{
	button.setEnabled(true);
	//build header
	layoutHeader.addWidget( &button);
	layoutHeader.addWidget( &title_);

	//build widget
	layoutHeaderAndWidget.addLayout( &layoutHeader);
	layoutHeaderAndWidget.addWidget( &widget_);
	setLayout(&layoutHeaderAndWidget);

	//connect signals and slots
	QObject::connect(&button, SIGNAL(clicked()), this, SLOT(toggleVisibility()));

	//collapse/ expand according to isCollapsed
	collapse(isCollapsed);
}

void Collapsable::collapse(bool b)
{
	if(b)
	{
		widget_.hide();
		button.setArrowType(Qt::DownArrow);
	} else
	{
		widget_.show();
		button.setArrowType(Qt::UpArrow);
	}
}


void Collapsable::setTitle(const QString& title)
{
	title_.setText(title);
}

QString Collapsable::title()
{
	return title_.text();
}

QWidget& Collapsable::widget()
{
	return widget_;
}

void Collapsable::toggleVisibility()
{
	collapse(widget_.isVisible());
}

}} // end namespaces qtutil, cvv
