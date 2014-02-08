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
#include "../dbg/dbg.hpp"

namespace cvv{ namespace qtutil{
class MatchPen;

class CVVMatch:public QGraphicsItem{
public:
	CVVMatch(const cv::KeyPoint& left_key,const cv::KeyPoint& right_key,const float& matchValue,const QPen& pen= QPen{},QGraphicsItem *parent=nullptr);
	
	CVVMatch(const CVVMatch& match):
		QGraphicsItem{match.parentWidget()},
		left_key_{match.left_key_},
		rigth_key_{match.rigth_key_},
		matchValue_{match.matchValue_},
		pen_{match.pen_}{TRACEPOINT;}

	virtual QRectF boundingRect() const;

	virtual void paint(QPainter *painter,const QStyleOptionGraphicsItem *option,QWidget *widget);

	cv::KeyPoint leftKeyPoint() const
		{TRACEPOINT;return left_key_;}

	cv::KeyPoint rightKeyPoint() const
		{TRACEPOINT;return rigth_key_;}

	QPointF leftImPointInScene() const
		{TRACEPOINT;return leftImWidget_->mapToScene(leftImage_->mapImagePointToParent(QPointF{left_key_.pt.x,left_key_.pt.y}));}

	QPointF rightImPointInScene() const
		{TRACEPOINT;return rightImWidget_->mapToScene(rightImage_->mapImagePointToParent(QPointF{rigth_key_.pt.x,rigth_key_.pt.y}));}

public slots:
	/*void updatePen(const MatchPen& pen)
		{pen_=pen.getPen(*this);}
	*/
private:

	cv::KeyPoint left_key_;
	cv::KeyPoint rigth_key_;
	float matchValue_;
	QPen pen_;

	qtutil::ZoomableImage 	*leftImage_;
	qtutil::ZoomableImage 	*rightImage_;

	QGraphicsProxyWidget	*leftImWidget_;
	QGraphicsProxyWidget	*rightImWidget_;
};

}}

#endif
