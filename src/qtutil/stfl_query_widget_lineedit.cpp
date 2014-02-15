#include "stfl_query_widget_lineedit.hpp"

#include "../dbg/dbg.hpp"

namespace cvv { namespace qtutil {

STFLQueryWidgetLineEdit::STFLQueryWidgetLineEdit(QWidget *parent): QLineEdit(parent),
    queryCompleter(new STFLQueryWidgetCompleter(this))
{
	TRACEPOINT;
    queryCompleter->setWidget(this);
    connect(queryCompleter, SIGNAL(activated(const QString&)), this, SLOT(insertCompletion(const QString&)));
    TRACEPOINT;
}

STFLQueryWidgetCompleter* STFLQueryWidgetLineEdit::completer()
{
	TRACEPOINT;
    return queryCompleter;
}

void STFLQueryWidgetLineEdit::insertCompletion(const QString& completion)
{
	TRACEPOINT;
    setText(completion);
    selectAll();
    TRACEPOINT;
}


void STFLQueryWidgetLineEdit::keyPressEvent(QKeyEvent *e)
{
	TRACEPOINT;
    if (queryCompleter->popup()->isVisible())
    {
        // The following keys are forwarded by the completer to the widget
        switch (e->key())
        {
        case Qt::Key_Enter:
        case Qt::Key_Return:
        case Qt::Key_Escape:
        case Qt::Key_Backtab:
            e->ignore();
            return; // Let the completer do default behavior
       	case Qt::Key_Tab:
			QLineEdit::keyPressEvent(new QKeyEvent(e->type(), Qt::DownArrow, e->modifiers()));
			e->ignore();
			return;   
		}
    }

    bool isShortcut = (e->modifiers() & Qt::ControlModifier) && e->key() == Qt::Key_E;
    if (!isShortcut)
        QLineEdit::keyPressEvent(e); // Don't send the shortcut (CTRL-E) to the text edit.

    bool ctrlOrShift = e->modifiers() & (Qt::ControlModifier | Qt::ShiftModifier);
    if (!isShortcut && !ctrlOrShift && e->modifiers() != Qt::NoModifier)
    {
        queryCompleter->popup()->hide();
        return;
    }
    
    requestSuggestions(text());
    TRACEPOINT;
}

void STFLQueryWidgetLineEdit::showSuggestions(QStringList suggestions){
	TRACEPOINT;
    queryCompleter->update(suggestions);
    queryCompleter->popup()->setCurrentIndex(queryCompleter->completionModel()->index(0, 0));
    TRACEPOINT;
}

}}
