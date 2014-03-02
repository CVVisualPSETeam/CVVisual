 #include "cvvkeypoint.hpp"

namespace cvv{ namespace qtutil{

CVVKeyPoint::CVVKeyPoint(const cv::KeyPoint& key,qtutil::ZoomableImage *image,
			QPen pen,QBrush brush,QGraphicsItem *parent):
		QGraphicsObject{parent}, key_{key},image_{image},
		pen_{pen},brush_{brush},show_{true}
{
	TRACEPOINT;
	setSelected(true);
	setToolTip(QString{"KeyPoint size: %1 \n angle %2 \n response %3 "}.arg(key_.size).arg(key_.angle).arg(key_.response));
	if(image!=nullptr)
	{
		updateImageSet(image->visibleArea(),image->zoom());
		connect(image,SIGNAL(updateArea(QRectF,qreal)),
			this,SLOT(updateImageSet(QRectF,qreal)));
	}
	TRACEPOINT;
}

void CVVKeyPoint::paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *)
{
	TRACEPOINT;
	painter->setPen(pen_);
	painter->drawEllipse(boundingRect());
	TRACEPOINT;
}


void CVVKeyPoint::setZoomableImage(ZoomableImage *image)
{
	image_=image;
	updateImageSet(image->visibleArea(),image->zoom());
	connect(image,SIGNAL(updateArea(QRectF,qreal)),
		this,SLOT(updateImageSet(const QRectF&,const qreal& )));
}

void CVVKeyPoint::updateSettings(KeyPointSettings& settings)
{
	TRACEPOINT;
	settings.setSettings(*this);
	TRACEPOINT;
}

void CVVKeyPoint::setPen(const QPen &pen)
{
	TRACEPOINT;
	pen_=pen;
	update();
	TRACEPOINT;
}

void CVVKeyPoint::setBrush(const QBrush &brush)
{
	TRACEPOINT;
	brush_=brush;
	update();
	TRACEPOINT;
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
