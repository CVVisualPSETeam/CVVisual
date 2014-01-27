#include "stfl_query_widget.hpp"

#include <iostream>

namespace cvv { namespace qtutil {

STFLQueryWidget::STFLQueryWidget()
{
    lineEdit = new STFLQueryWidgetLineEdit(this);
    auto *layout = new QVBoxLayout;
    layout->addWidget(lineEdit);
    setLayout(layout);
    connect(lineEdit, SIGNAL(returnPressed()), this, SLOT(returnPressed()));
    connect(lineEdit, SIGNAL(textChanged(QString)), this, SLOT(textChanged()));
    connect(lineEdit, SIGNAL(requestSuggestions(QString)), this, SIGNAL(requestSuggestions(QString)));
}

void STFLQueryWidget::showSuggestions(const QStringList &suggestions)
{
    lineEdit->showSuggestions(suggestions);
}

void STFLQueryWidget::returnPressed(){
	filterSignal(lineEdit->text());
}

void STFLQueryWidget::textChanged(){
    userInputUpdate(lineEdit->text());
    requestSuggestions(lineEdit->text());
}

}}
