#ifndef CVVISUAL_MATCH_PEN
#define CVVISUAL_MATCH_PEN

#include <QWidget>
#include <QPen>

#include "cvvmatch.hpp"
namespace cvv{ namespace qtutil{
class CVVMatch;

class MatchPen:public QWidget{
Q_OBJECT

public:
	MatchPen(QWidget* parent):QWidget(parent){}
	
	virtual QPen getPen(const CVVMatch& cvmatch) = 0;

signals:
	void settingsChanged();
};

}}
#endif
