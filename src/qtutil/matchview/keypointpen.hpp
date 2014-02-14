#ifndef CVVISUAL_KEYPOINT_PEN
#define CVVISUAL_KEYPOINT_PEN

#include <QWidget>
#include <QPen>


#include "../../dbg/dbg.hpp"

namespace cvv{ namespace qtutil{

class CVVKeyPoint;

class KeyPointPen:public QWidget{
Q_OBJECT

public:
	KeyPointPen(QWidget* parent):QWidget(parent){TRACEPOINT;}
	
	virtual QPen getPen(const CVVKeyPoint& )const =0;

signals:
	void settingsChanged(const KeyPointPen&);
};

}}
#endif
