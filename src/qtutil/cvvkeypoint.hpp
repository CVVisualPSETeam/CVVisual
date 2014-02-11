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
//#include "cvvmatch.hpp"
#include "cvvkeypoint.hpp"
#include "../dbg/dbg.hpp"

namespace cvv{namespace qtutil{

class CVVKeyPoint:public QGraphicsObject{
Q_OBJECT
public:
	CVVKeyPoint(const cv::KeyPoint& key,qtutil::ZoomableImage *image=nullptr,QGraphicsProxyWidget* imWidget=nullptr,
		QPen pen=QPen{Qt::red},QBrush brush=QBrush{Qt::white},QGraphicsItem *parent=nullptr);


	QPointF imPointInScene() const
		{TRACEPOINT; return image_->mapImagePointToParent(QPointF{key_.pt.x,key_.pt.y}); }

	QRectF boundingRect() const;

	cv::KeyPoint keyPoint() const
		{TRACEPOINT;return key_;}

	void paint(QPainter *painter, const QStyleOptionGraphicsItem *option,
               QWidget *widget);

	void setProxyWidget(QGraphicsProxyWidget*imWidget);

	void setZoomableImage(ZoomableImage *image);
signals:
	void updateShown(bool);

	void updatePoint(bool visible);

public slots:
	void updatePen(const QPen& pen)
		{TRACEPOINT;pen_=pen;TRACEPOINT;}
	
	void updateBrush(const QBrush& brush)
		{TRACEPOINT;brush_=brush;}

	void setShow(bool b)
		{TRACEPOINT;show_=b;emit updateShown(true);TRACEPOINT;}
	
	bool isShown() const
		{TRACEPOINT;return show_;}

	void updateImageSet(const QRectF& visibleArea,const qreal& zoom);
private:

	cv::KeyPoint key_;

	qtutil::ZoomableImage 	*image_;

	QGraphicsProxyWidget	*imWidget_;
	QPen pen_;
	QBrush brush_;
	qreal zoom_;
	bool show_;
};
}}
#endif
