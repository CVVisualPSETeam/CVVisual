#include "collapsable.hpp"

namespace cvv{ namespace qtutil{

Collapsable::Collapsable(const QString& title,QWidget& widget, bool isCollapsed, QWidget *parent):
		QWidget{parent}, widget_{&widget},
		button_{new QPushButton{}}, layout_{new QVBoxLayout{}}
{
	layout_->setAlignment(Qt::AlignTop);
	//put elements on the heap
	//QHBoxLayout* layoutHeader = new QHBoxLayout{};

	//build header
	button_->setEnabled(true);
	button_->setText(title);
	button_->setCheckable(true);
	//layoutHeader->addWidget(button_);
	//layoutHeader->addWidget(title_);

	//build widget
	//layout_->addLayout(layoutHeader);
	layout_->addWidget(button_);
	layout_->addWidget(widget_);
	setLayout(layout_);

	//connect signals and slots
	QObject::connect(button_, SIGNAL(clicked()), this, SLOT(toggleVisibility()));

	//collapse/ expand according to isCollapsed
	collapse(isCollapsed);
}

void Collapsable::collapse(bool b)
{
	button_->setChecked(!b);
	if(b)
	{
		widget_->hide();
		//button_->setArrowType(Qt::DownArrow);
	} else
	{
		widget_->show();
		//button_->setArrowType(Qt::UpArrow);
	}
}

QWidget* Collapsable::detachWidget()
{
	if(!widget_){return nullptr;}
	layout_->removeWidget(widget_);
	QWidget* tmp = widget_;
	widget_ =  nullptr;
	return tmp;
}

}} // end namespaces qtutil, cvv
