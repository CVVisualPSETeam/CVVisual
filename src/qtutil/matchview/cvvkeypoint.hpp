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


	/**
	 * @brief this method maps the imagepoint to the scene
	 * @return maps the imagepoint to the scene
	 */
	QPointF imPointInScene() const
		{TRACEPOINT; return image_->mapImagePointToParent(QPointF{key_.pt.x,key_.pt.y}); }

	/**
	 * @brief boundingRect
	 * @return the boundingRect
	 */
	QRectF boundingRect() const;

	/**
	 * @brief returns the keypoint
	 * @return the keypoint
	 */
	cv::KeyPoint keyPoint() const
		{TRACEPOINT;return key_;}

	/**
	 * @brief the paint function.
	 */
	void paint(QPainter *painter, const QStyleOptionGraphicsItem *option,QWidget *widget);

	/**
	 * @brief this method sets and connects this keypoint which the given ZoomableImage.
	 * the ZoomableImage should be in a QGraphicScene and should have same parent
	 * @param image the image
	 */
	void setZoomableImage(ZoomableImage *image);

	/**
	 * @brief returns true if this keypoint is in the visble area of its image
	 * @return true if this keypoint is in the visble area of its image
	 */
	bool imagePointisVisible()
		{TRACEPOINT;
		return image_->visibleArea().contains(key_.pt.x,key_.pt.y); }
signals:
	/**
	 * @brief this signal will be emited when the imagepoint in the scene has changed
	 * @param visible it is true if this keypoint is in the visibleArea
	 */
	void updatePoint(bool visible);

public slots:
	/**
	 * @brief updates the pen of this KeyPoint
	 * @param pen a new Pen
	 */
	void updatePen(const KeyPointPen& pen)
		{TRACEPOINT;pen_=pen.getPen(*this);TRACEPOINT;}

	/**
	 * @brief updates the brush of this KeyPoint
	 * @param brush a new brush
	 */
	void updateBrush(const QBrush& brush)
		{TRACEPOINT;brush_=brush;}

	/**
	 * @brief if show is true this keypoint will be visible if it is the visibleArea
	 * @param b the new show Value
	 */
	void setShow(bool b)
		{TRACEPOINT;show_=b;TRACEPOINT;}

	/**
	 * @brief if show is true this keypoint will be visible if it is the visibleArea
	 * @return the show Value
	 */
	bool isShown() const
		{TRACEPOINT;return show_;}

	/**
	 * @brief updates the coordinates and visibleState of this KeyPoint
	 * @param visibleArea the visibleArea of the ZoomableImage
	 * @param zoom the zoomfactor
	 */
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
