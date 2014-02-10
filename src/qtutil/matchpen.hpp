#ifndef CVVISUAL_MATCH_PEN
#define CVVISUAL_MATCH_PEN

#include <QWidget>
#include <QPen>


#include "../dbg/dbg.hpp"

namespace cvv{ namespace qtutil{

class CVVMatch;

class MatchPen:public QWidget{
Q_OBJECT

public:
	MatchPen(QWidget* parent):QWidget(parent){TRACEPOINT;}
	
	virtual QPen getPen(const CVVMatch& )const =0;

signals:
	void settingsChanged(const MatchPen&);
};

}}
#endif
