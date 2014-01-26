#ifndef CVVISUAL_STFLQUERYWIDGET_HPP
#define	CVVISUAL_STFLQUERYWIDGET_HPP

#include <QString>
#include <QLineEdit>
#include <QStringList>
#include <QWidget>
#include <QVBoxLayout>

#include "stfl_query_widget_lineedit.hpp"

namespace cvv { namespace qtutil {

class STFLQueryWidget : public QWidget
{
	Q_OBJECT
	
public:
	
	/**
	 * @todo implement
     */
	STFLQueryWidget();
	
	/**
	 * @todo implement
     */
	void showSuggestions(const QStringList &suggestions);

public slots:
    void returnPressed();
    void textChanged();

signals:
	
	void filterSignal(QString query);

	void userInputUpdate(QString query);

    void requestSuggestions(QString query);

private:
    STFLQueryWidgetLineEdit *lineEdit;
};

}}

#endif

