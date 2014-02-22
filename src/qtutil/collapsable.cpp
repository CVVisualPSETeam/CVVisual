#include "collapsable.hpp"

namespace cvv{ namespace qtutil{

Collapsable::Collapsable(const QString& title, std::unique_ptr<QWidget> widget, bool isCollapsed,
	QWidget *parent):
		QFrame{parent}, widget_{widget.get()}, layout_{nullptr}
{
	TRACEPOINT;
	auto lay=util::make_unique<QVBoxLayout>();
	layout_=*lay;
	//set alignment+border
	setLineWidth(1);
	setFrameStyle(QFrame::Box);
	layout_->setAlignment(Qt::AlignTop);
	layout_->setContentsMargins(0,0,0,0);

	//build header
	auto tmpButton = util::make_unique<QPushButton>();
	button_ = tmpButton.get();
	button_->setEnabled(true);
	button_->setText(title);
	button_->setCheckable(true);

	//build widget
	setLayout(lay.release());
	layout_->addWidget(tmpButton.release());
	layout_->addWidget(widget.release());

	//connect signals and slots
	QObject::connect(button_, SIGNAL(clicked()), this, SLOT(toggleVisibility()));

	//collapse/ expand according to isCollapsed
	collapse(isCollapsed);
	TRACEPOINT;
}

//Collapsable::Collapsable(const QString& title,QWidget& widget, bool isCollapsed, QWidget *parent):
//	Collapsable{title, std::unique_ptr<QWidget>{&widget}, isCollapsed, parent} {}

void Collapsable::collapse(bool b)
{
	TRACEPOINT;
	button_->setChecked(!b);
	if(b)
	{
		widget_->hide();
	} else
	{
		widget_->show();
	}
	TRACEPOINT;
}

QWidget* Collapsable::detachWidget()
{
	TRACEPOINT;
	if(!widget_){return nullptr;}
	layout_->removeWidget(widget_);
	QWidget* tmp = widget_;
	widget_ =  nullptr;
	TRACEPOINT;
	return tmp;
}

}} // end namespaces qtutil, cvv
