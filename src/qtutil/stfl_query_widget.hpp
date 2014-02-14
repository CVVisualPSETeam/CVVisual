#ifndef CVVISUAL_STFLQUERYWIDGET_HPP
#define	CVVISUAL_STFLQUERYWIDGET_HPP

#include <QString>
#include <QLineEdit>
#include <QStringList>
#include <QWidget>
#include <QVBoxLayout>
#include <QPushButton>

#include "stfl_query_widget_lineedit.hpp"
#include "../util/util.hpp"

namespace cvv { namespace qtutil {

class STFLQueryWidget : public QWidget
{
	Q_OBJECT
	
public:
	
	STFLQueryWidget();
	
	void showSuggestions(const QStringList &suggestions);

public slots:
    void returnPressed();
	
    void textChanged();

	void helpRequested();
	
signals:
	
	void filterSignal(QString query);

	void userInputUpdate(QString query);

    void requestSuggestions(QString query);
	
	void showHelp(QString topic);
	
private:
    STFLQueryWidgetLineEdit *lineEdit;
};

}}

#endif

