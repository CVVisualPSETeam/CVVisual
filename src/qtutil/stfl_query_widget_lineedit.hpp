#ifndef CVVISUAL_STFL_QUERY_WIDGET_LINEEDIT_HPP
#define CVVISUAL_STFL_QUERY_WIDGET_LINEEDIT_HPP

#include <QStringList>
#include <QLineEdit>
#include <QKeyEvent>
#include <QAbstractItemView>

#include "stfl_query_widget_completer.hpp"

namespace cvv { namespace qtutil {

/**
 * @brief The STFLQueryWidgetLineEdit class
 * @note It's heavily based on http://www.qtcentre.org/archive/index.php/t-23518.html
 */
class STFLQueryWidgetLineEdit : public QLineEdit
{
    Q_OBJECT
public:
    STFLQueryWidgetLineEdit(QWidget *parent = 0);

    STFLQueryWidgetCompleter* completer();

    void showSuggestions(QStringList suggestions);

protected:
    void keyPressEvent(QKeyEvent *e);

signals:
    void requestSuggestions(QString input);

private slots:
    void insertCompletion(const QString &completion);

private:
    STFLQueryWidgetCompleter *queryCompleter;
};


}}
#endif
