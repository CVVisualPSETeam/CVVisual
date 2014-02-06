
#include <algorithm>

#include "cvvmatch.hpp"
#include "matchpen.hpp"

namespace cvv{ namespace qtutil{

	CVVMatch::CVVMatch(const cv::KeyPoint& left_key, const cv::KeyPoint& right_key,const float& matchValue,
			const QPen& pen,QGraphicsItem *parent):
		QGraphicsItem{parent},
		left_key_{left_key},
		rigth_key_{right_key},
		matchValue_{matchValue},
		pen_{pen}
	{ }


	QRectF CVVMatch::boundingRect() const 
	{
		return QRectF{QPointF{std::min(leftImPointInScene().rx(),rightImPointInScene().rx()),
				std::min(leftImPointInScene().ry(),rightImPointInScene().ry())},
				QPointF{std::max(leftImPointInScene().rx(),rightImPointInScene().rx()),
				std::max(leftImPointInScene().ry(),rightImPointInScene().ry())}};
	}

	void CVVMatch::paint(QPainter *painter,const QStyleOptionGraphicsItem *option,QWidget *widget)
	{
		painter->setPen(pen_);
		painter->drawLine(leftImPointInScene(),rightImPointInScene());
		(void) widget;
		(void) option;
	}
}}
