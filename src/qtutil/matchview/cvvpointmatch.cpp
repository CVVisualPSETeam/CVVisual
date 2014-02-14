#include "cvvpointmatch.hpp"

namespace cvv{ namespace qtutil{

CVVPointMatch::CVVPointMatch(CVVKeyPoint *left_key,CVVKeyPoint *right_key,const float& matchValue,bool isLeftKey,qreal radius,const QPen& pen,QGraphicsItem *parent):
	CVVMatch{left_key,right_key,matchValue,pen,parent},isLeftKey_{isLeftKey},radius_{radius}{}

QRectF CVVPointMatch::boundingRect() const
{
	TRACEPOINT;
	QPointF point=(isLeftKey_?leftImPointInScene():rightImPointInScene());
	TRACEPOINT;
	return QRectF{point.x()-radius_,point.y()-radius_,point.x()+radius_,point.y()+radius_};
}

void CVVPointMatch::paint(QPainter *painter,const QStyleOptionGraphicsItem *,QWidget *)
{
	painter->setPen(pen_);
	painter->drawEllipse(boundingRect());
}
}}
