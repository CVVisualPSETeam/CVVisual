#include "cvvkeypoint.hpp"

namespace cvv
{
namespace qtutil
{

CVVKeyPoint::CVVKeyPoint(const cv::KeyPoint &key, qtutil::ZoomableImage *image,
                         QPen pen, QBrush brush, QGraphicsItem *parent)
    : QGraphicsObject{ parent }, key_{ key }, image_{ image }, pen_{ pen },
      brush_{ brush }, show_{ true }
{
	setFlag(QGraphicsItem::ItemIsSelectable, true);
	setSelected(true);
	setToolTip(QString
	{ "KeyPoint size: %1 \n angle %2 \n response %3 " }
	               .arg(key_.size)
	               .arg(key_.angle)
	               .arg(key_.response));
	if (image != nullptr)
	{
		updateImageSet(image->visibleArea(), image->zoom());
		connect(image, SIGNAL(updateArea(QRectF, qreal)), this,
		        SLOT(updateImageSet(QRectF, qreal)));
	}
}

void CVVKeyPoint::paint(QPainter *painter, const QStyleOptionGraphicsItem *,
                        QWidget *)
{
	painter->setPen(pen_);
	painter->setBrush(brush_);
	painter->drawEllipse(boundingRect());
}

void CVVKeyPoint::setZoomableImage(ZoomableImage *image)
{
	image_ = image;
	updateImageSet(image->visibleArea(), image->zoom());
	connect(image, SIGNAL(updateArea(QRectF, qreal)), this,
	        SLOT(updateImageSet(const QRectF &, const qreal &)));
}

bool CVVKeyPoint::operator==(const cv::KeyPoint &o)
{
	return o.pt == key_.pt && o.size == key_.size &&
	       o.angle == key_.angle && o.response == key_.response &&
	       o.octave == key_.octave && o.class_id == key_.class_id;
}

void CVVKeyPoint::updateSettings(KeyPointSettings &settings)
{
	settings.setSettings(*this);
}

void CVVKeyPoint::setPen(const QPen &pen)
{
	pen_ = pen;
	update();
}

void CVVKeyPoint::setBrush(const QBrush &brush)
{
	brush_ = brush;
	update();
}

QRectF CVVKeyPoint::boundingRect() const
{
	// TODO throw image==nullptr
	return QRectF{
		QPointF{ imPointInScene().x() - 3, imPointInScene().y() - 3 },
		QPointF{ imPointInScene().x() + 3, imPointInScene().y() + 3 }
	};
}

void CVVKeyPoint::updateImageSet(const QRectF &visibleArea, const qreal &zoom)
{
	setVisible(show_ && visibleArea.contains(key_.pt.x, key_.pt.y));
	emit updatePoint(visibleArea.contains(key_.pt.x, key_.pt.y));
	zoom_ = zoom;
	prepareGeometryChange();
	// update();
}
}
}
