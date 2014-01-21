#ifndef CVVISUAL_STFLQUERYWIDGET_HPP
#define	CVVISUAL_STFLQUERYWIDGET_HPP

#include <QString>
#include <QLineEdit>
#include <QStringList>
#include <QWidget>

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

signals:
	
	void filterSignal(QString query);

	void userInputUpdate(QString query);

private:
	QLineEdit *inputLine;
};

}}

#endif

