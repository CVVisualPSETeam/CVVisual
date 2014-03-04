#include "stfl_query_widget.hpp"

#include "../dbg/dbg.hpp"

namespace cvv
{
namespace qtutil
{

STFLQueryWidget::STFLQueryWidget()
{
	TRACEPOINT;
	lineEdit = new STFLQueryWidgetLineEdit(this);
	auto *layout = new QHBoxLayout;
	layout->addWidget(lineEdit);
	auto helpButton = new QPushButton("Help", this);
	layout->addWidget(helpButton);
	setLayout(layout);
	connect(helpButton, SIGNAL(released()), this, SLOT(helpRequested()));
	connect(lineEdit, SIGNAL(returnPressed()), this, SLOT(returnPressed()));
	connect(lineEdit, SIGNAL(textChanged(QString)), this,
	        SLOT(textChanged()));
	connect(lineEdit, SIGNAL(requestSuggestions(QString)), this,
	        SIGNAL(requestSuggestions(QString)));
	TRACEPOINT;
}

void STFLQueryWidget::showSuggestions(const QStringList &suggestions)
{
	TRACEPOINT;
	lineEdit->showSuggestions(suggestions);
	TRACEPOINT;
}

void STFLQueryWidget::returnPressed()
{
	TRACEPOINT;
	filterSignal(lineEdit->text());
	TRACEPOINT;
}

void STFLQueryWidget::textChanged()
{
	TRACEPOINT;
	userInputUpdate(lineEdit->text());
	requestSuggestions(lineEdit->text());
	TRACEPOINT;
}

void STFLQueryWidget::helpRequested()
{
	TRACEPOINT;
	showHelp("filterquery");
	TRACEPOINT;
}
}
}
