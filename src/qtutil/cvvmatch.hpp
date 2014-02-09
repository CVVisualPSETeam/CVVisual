#ifndef CVVISUAL_CVVMATCH
#define CVVISUAL_CVVMATCH

#include <QGraphicsItem>
#include <QPainter>
#include <QPointF>
#include <QRectF>
#include <QStyleOptionGraphicsItem>
#include <QWidget>
#include <QGraphicsScene>
#include <QGraphicsProxyWidget>

#include "opencv2/core/core.hpp"
#include "opencv2/features2d/features2d.hpp"
#include "zoomableimage.hpp"
#include "matchpen.hpp"
#include "cvvkeypoint.hpp"
#include "../dbg/dbg.hpp"

namespace cvv{ namespace qtutil{
class MatchPen;

class CVVMatch:public QGraphicsObject{
Q_OBJECT
public:
	CVVMatch(CVVKeyPoint *left_key,CVVKeyPoint *right_key,const float& matchValue,const QPen& pen= QPen{Qt::red},QGraphicsItem *parent=nullptr);
	


	virtual QRectF boundingRect() const;

	virtual void paint(QPainter *painter,const QStyleOptionGraphicsItem *option,QWidget *widget);

	cv::KeyPoint leftKeyPoint() const
		{TRACEPOINT;return left_key_->keyPoint();}

	cv::KeyPoint rightKeyPoint() const
		{TRACEPOINT;return right_key_->keyPoint();}

	QPointF leftImPointInScene() const
		{TRACEPOINT;return left_key_->imPointInScene();}

	QPointF rightImPointInScene() const
		{TRACEPOINT;return right_key_->imPointInScene();}

public slots:
	void updatePen(const MatchPen& pen)
		{TRACEPOINT;pen_=pen.getPen(*this);TRACEPOINT;}
	
	void setShow(const bool& b)
		{TRACEPOINT;show_=b;}
	
	bool isShown() const
		{TRACEPOINT;return show_;}
	
	void updateRightKey(bool visible);
	
	void updateLeftKey(bool visible);
	
private:
	
	CVVKeyPoint *left_key_;
	CVVKeyPoint *right_key_;
	float matchValue_;

	QPen pen_;
	bool show_;
	bool left_key_visible_;
	bool right_key_visible_;
};

}}

#endif
