
#include <algorithm>

#include "cvvmatch.hpp"

namespace cvv{ namespace qtutil{

CVVMatch::CVVMatch(CVVKeyPoint *left_key, CVVKeyPoint* right_key, const cv::DMatch &match,
		const QPen& pen, QGraphicsItem *parent):
	QGraphicsObject{parent},
	left_key_{left_key},
	right_key_{right_key},
	match_{match},
	pen_{pen},
	show_{true},
	left_key_visible_{left_key->imagePointisVisible()},
	right_key_visible_{right_key_->imagePointisVisible()}
{
	TRACEPOINT;
	setVisible(show_&&left_key_visible_&&right_key_visible_);
	setSelected(true);

	setToolTip(QString{"Match distance: %1 \n queryIdx %2 \n trainIdx %3 \n imIdx %4 " }.arg(match_.distance).arg(match_.queryIdx).arg(match_.trainIdx).arg(match_.imgIdx));
	connect(left_key_,SIGNAL(updatePoint(bool)),this,SLOT(updateLeftKey(bool)));
	connect(right_key_,SIGNAL(updatePoint(bool)),this,SLOT(updateRightKey(bool)));
	TRACEPOINT;
}


QRectF CVVMatch::boundingRect() const
{
	TRACEPOINT;
	//TODO minmax
	return QRectF{QPointF{std::min(leftImPointInScene().rx(),rightImPointInScene().rx()),
			std::min(leftImPointInScene().ry(),rightImPointInScene().ry())},
			QPointF{std::max(leftImPointInScene().rx(),rightImPointInScene().rx()),
			std::max(leftImPointInScene().ry(),rightImPointInScene().ry())}};
}

void CVVMatch::paint(QPainter *painter, const QStyleOptionGraphicsItem*, QWidget *)
{
	TRACEPOINT;
	painter->setPen(pen_);
	painter->drawLine(leftImPointInScene(),rightImPointInScene());
	TRACEPOINT;
}

void CVVMatch::setPen(const QPen& pen)
{
	TRACEPOINT;
	pen_=pen;
	update();
	TRACEPOINT;
}

void CVVMatch::setShow(const bool &b)
{
	TRACEPOINT;
	show_=b;
	setVisible(show_&&left_key_visible_&&right_key_visible_);
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
