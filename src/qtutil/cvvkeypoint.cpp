 #include "cvvkeypoint.hpp"

namespace cvv{ namespace qtutil{
	
	void CVVKeyPoint::paint(QPainter *painter, const QStyleOptionGraphicsItem *option,
               QWidget *widget)
		{
			TRACEPOINT;
			painter->drawEllipse(QPointF{key_.pt.x,key_.pt.y},10, 10);
			(void) option;
			(void) widget;
			TRACEPOINT;
		}
}}
