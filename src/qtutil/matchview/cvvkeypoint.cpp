 #include "cvvkeypoint.hpp"

namespace cvv{ namespace qtutil{

CVVKeyPoint::CVVKeyPoint(const cv::KeyPoint& key,qtutil::ZoomableImage *image, QPen pen,QBrush brush,QGraphicsItem *parent):
			QGraphicsObject{parent}, key_{key},image_{image},
			pen_{pen},brush_{brush},show_{true}
{
	TRACEPOINT;
	if(image!=nullptr)
	{
		updateImageSet(image->visibleArea(),image->zoom());
		connect(image,SIGNAL(updateArea(QRectF,qreal)),this,SLOT(updateImageSet(QRectF,qreal)));
	}
	TRACEPOINT;
}

void CVVKeyPoint::paint(QPainter *painter, const QStyleOptionGraphicsItem *option,
       QWidget *widget)
{
	TRACEPOINT;
	painter->setPen(pen_);
	painter->drawEllipse(boundingRect());
	(void) option;
	(void) widget;
	TRACEPOINT;
}


void CVVKeyPoint::setZoomableImage(ZoomableImage *image)
{
	image_=image;
	updateImageSet(image->visibleArea(),image->zoom());
	connect(image,SIGNAL(updateArea(QRectF,qreal)),this,SLOT(updateImageSet(const QRectF&,const qreal& )));
}

QRectF CVVKeyPoint::boundingRect() const
{
	TRACEPOINT;
	//TODO throw image==nullptr
	return QRectF{QPointF{imPointInScene().x()-3,imPointInScene().y()-3}
			,QPointF{imPointInScene().x()+3,imPointInScene().y()+3}};
}

void CVVKeyPoint::updateImageSet(const QRectF& visibleArea,const qreal& zoom)
{
	TRACEPOINT;
	setVisible(show_&&visibleArea.contains(key_.pt.x,key_.pt.y));
	emit updatePoint(visibleArea.contains(key_.pt.x,key_.pt.y));
	zoom_=zoom;
	prepareGeometryChange();
	update();
	TRACEPOINT;
}
}}
