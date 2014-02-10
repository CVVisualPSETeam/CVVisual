 #include "cvvkeypoint.hpp"

namespace cvv{ namespace qtutil{

CVVKeyPoint::CVVKeyPoint(const cv::KeyPoint& key,qtutil::ZoomableImage *image,
		QGraphicsProxyWidget* imWidget, QPen pen,QBrush brush,QGraphicsItem *parent):
			QGraphicsObject{parent}, key_{key},image_{image},
			imWidget_{imWidget},pen_{pen},brush_{brush},show_{true}
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


void CVVKeyPoint::setProxyWidget(QGraphicsProxyWidget*imWidget)
{
	imWidget_=imWidget;
	if(image_!=nullptr){
		updateImageSet(image_->visibleArea(),image_->zoom());
	}
}

void CVVKeyPoint::setZoomableImage(ZoomableImage *image)
{
	image_=image;
	if(imWidget_!=nullptr){
		updateImageSet(image_->visibleArea(),image_->zoom());
	}
}

QRectF CVVKeyPoint::boundingRect() const
{
	TRACEPOINT;
	//TODO throw image==nullptr|| imWidget==nullptr
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
