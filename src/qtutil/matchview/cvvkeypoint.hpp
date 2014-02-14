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

#include "../zoomableimage.hpp"
#include "cvvkeypoint.hpp"
#include "keypointpen.hpp"
#include "../../dbg/dbg.hpp"

namespace cvv{namespace qtutil{
/**
 * @brief this class represents a Keypoint which is displayed an a Matchscene.
 * 
 **/

class CVVKeyPoint:public QGraphicsObject{
Q_OBJECT
public:
	/**
	 * @brief the construor
	 * @param key the keypoint with the image point
	 * @param image the zoomable image 
	 */
	CVVKeyPoint(const cv::KeyPoint& key,qtutil::ZoomableImage *image=nullptr,
		QPen pen=QPen{Qt::red},QBrush brush=QBrush{Qt::white},QGraphicsItem *parent=nullptr);


	QPointF imPointInScene() const
		{TRACEPOINT; return image_->mapImagePointToParent(QPointF{key_.pt.x,key_.pt.y}); }

	QRectF boundingRect() const;

	cv::KeyPoint keyPoint() const
		{TRACEPOINT;return key_;}

	void paint(QPainter *painter, const QStyleOptionGraphicsItem *option,
               QWidget *widget);

	void setZoomableImage(ZoomableImage *image);

	bool imagePointisVisible()
		{TRACEPOINT;
		return image_->visibleArea().contains(key_.pt.x,key_.pt.y); }
signals:
	void updateShown(bool);

	void updatePoint(bool visible);

public slots:
	void updatePen(const KeyPointPen& pen)
		{TRACEPOINT;pen_=pen.getPen(*this);TRACEPOINT;}
	
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

	QPen pen_;
	QBrush brush_;
	qreal zoom_;
	bool show_;
};
}}
#endif
