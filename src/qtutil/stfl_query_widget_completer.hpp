#ifndef CVVISUAL_STFL_QUERY_WIDGET_COMPLETER_HPP
#define CVVISUAL_STFL_QUERY_WIDGET_COMPLETER_HPP

#include <QStringList>
#include <QStringListModel>
#include <QString>
#include <QCompleter>

namespace cvv { namespace qtutil {

class STFLQueryWidgetCompleter : public QCompleter
{
    Q_OBJECT

public:
    STFLQueryWidgetCompleter(QObject* parent) :
        QCompleter(parent), model()
    {
        setModel(&model);
    }


    void update(QStringList suggestions)
    {
        model.setStringList(suggestions);
        complete();
    }

private:
    QStringListModel model;
};


}}
#endif
