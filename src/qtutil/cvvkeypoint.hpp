#ifndef CVVISUAL_CVVKEYPOINT
#define CVVISUAL_CVVKEYPOINT

#include <QGraphicsItem>
#include <QPainter>
#include <QRectF>
#include <QStyleOptionGraphicsItem>
#include <QWidget>
#include <QGraphicsScene>

#include "opencv2/core/core.hpp"
#include "opencv2/features2d/features2d.hpp"

#include "zoomableimage.hpp"
#include "cvvmatch.hpp"
#include "cvvkeypoint.hpp"

namespace cvv{namespace qtutil{

class CVVKeyPoint:public QGraphicsItem{
public:
	CVVKeyPoint(const cv::KeyPoint& key,qtutil::ZoomableImage *image,QGraphicsProxyWidget* imWidget,
		QPen pen=QPen{},QBrush brush=QBrush{},QGraphicsItem *parent=nullptr):
		QGraphicsItem{parent},
		key_{key},image_{image},imWidget_{imWidget},pen_{pen},brush_{brush}{}

	CVVKeyPoint(const CVVKeyPoint& key):QGraphicsItem{key.parentWidget()},
		key_{key.key_},image_{key.image_},imWidget_{key.imWidget_},pen_{key.pen_},brush_{key.brush_}{}

	QPointF imPointInScene()
		{ return imWidget_->mapToScene(image_->mapImagePointToParent(QPointF{key_.pt.x,key_.pt.y})); }

	QRectF boundingRect() const
		{ return QRectF{QPointF{key_.pt.x-10,key_.pt.y-10},QPointF{key_.pt.x+10,key_.pt.y+10}}; }
	
	cv::KeyPoint keyPoint()
		{return key_;}
	void paint(QPainter *painter, const QStyleOptionGraphicsItem *option,
               QWidget *widget);
public slots:
	void updatePen(QPen pen)
		{pen_=pen;}
	
	void updateBrush(QBrush brush)
		{brush_=brush;}
private:

	cv::KeyPoint key_;

	qtutil::ZoomableImage 	*image_;

	QGraphicsProxyWidget	*imWidget_;
	QPen pen_;
	QBrush brush_;
};
}}
#endif
