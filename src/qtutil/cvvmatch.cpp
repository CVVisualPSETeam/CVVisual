
#include <algorithm>

#include "cvvmatch.hpp"
#include "matchpen.hpp"

namespace cvv{ namespace qtutil{

CVVMatch::CVVMatch(CVVKeyPoint *left_key,CVVKeyPoint* right_key,const float& matchValue,
		const QPen& pen,QGraphicsItem *parent):
	QGraphicsObject{parent},
	left_key_{left_key},
	right_key_{right_key},
	matchValue_{matchValue},
	pen_{pen},
	show_{true}
{
	TRACEPOINT;
	connect(left_key_,SIGNAL(updatePoint(bool)),this,SLOT(updateLeftKey(bool)));
	connect(right_key_,SIGNAL(updatePoint(bool)),this,SLOT(updateRightKey(bool)));
	TRACEPOINT;
}


QRectF CVVMatch::boundingRect() const 
{
	TRACEPOINT;
	return QRectF{QPointF{std::min(leftImPointInScene().rx(),rightImPointInScene().rx()),
			std::min(leftImPointInScene().ry(),rightImPointInScene().ry())},
			QPointF{std::max(leftImPointInScene().rx(),rightImPointInScene().rx()),
			std::max(leftImPointInScene().ry(),rightImPointInScene().ry())}};
}

void CVVMatch::paint(QPainter *painter,const QStyleOptionGraphicsItem *option,QWidget *widget)
{
	TRACEPOINT;
	painter->setPen(pen_);
	painter->drawLine(leftImPointInScene(),rightImPointInScene());
	(void) widget;
	(void) option;
	TRACEPOINT;
}

void CVVMatch::updateLeftKey(bool visible)
{
	TRACEPOINT;
	left_key_visible_=visible;
	setVisible(show_&&left_key_visible_&&right_key_visible_);
	prepareGeometryChange();
	update();
	TRACEPOINT;
}

void CVVMatch::updateRightKey(bool visible)
{
	TRACEPOINT;
	right_key_visible_=visible;
	setVisible(show_&&left_key_visible_&&right_key_visible_);
	prepareGeometryChange();
	update();
	TRACEPOINT;
}
}}
