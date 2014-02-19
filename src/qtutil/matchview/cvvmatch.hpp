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
#include "../zoomableimage.hpp"
#include "matchpen.hpp"
#include "cvvkeypoint.hpp"
#include "../../dbg/dbg.hpp"

namespace cvv{ namespace qtutil{

class CVVMatch:public QGraphicsObject{
Q_OBJECT
public:

	/**
	* @brief the constructor
	* @param left_key the left KeyPointPen
	* @param right_key the right KeyPointPen
	* @param matchValue the match distance
	* @param parent the parent Widget
	*/
	CVVMatch(CVVKeyPoint *left_key,CVVKeyPoint *right_key,const float& matchValue,
		 const QPen& pen= QPen{Qt::red},QGraphicsItem *parent=nullptr);


	/**
	 * @brief returns the boundingrect of this Mathc
	 * @return the boundingrect of this Mathc
	 */
	virtual QRectF boundingRect() const;

	/**
	 * @brief the paint function
	 */
	virtual void paint(QPainter *painter,const QStyleOptionGraphicsItem *,QWidget *);

	/**
	 * @brief returns the left keypoint.
	 * @return the left keypoint.
	 */
	cv::KeyPoint leftKeyPoint() const
		{TRACEPOINT;return left_key_->keyPoint();}

	/**
	 * @brief returns the right keypoint.
	 * @return the right keypoint.
	 */
	cv::KeyPoint rightKeyPoint() const
		{TRACEPOINT;return right_key_->keyPoint();}

	/**
	 * @brief maps the leftImagePoint to scene
	 * @return the scene point of the leftkeypoint
	 */
	QPointF leftImPointInScene() const
		{TRACEPOINT;return left_key_->imPointInScene();}

	/**
	 * @brief maps the leftImagePoint to scene
	 * @return the scene point of the rightkeypoint
	 */
	QPointF rightImPointInScene() const
		{TRACEPOINT;return right_key_->imPointInScene();}

	/**
	 * @brief returns the match value
	 * @return the match value
	 */
	float matchValue() const
		{TRACEPOINT;return matchValue_;}

public slots:

	/**
	 * @brief this method updates the Pen
	 * @param pen the new Pen
	 */
	void updatePen(const MatchPen& pen)
		{TRACEPOINT;pen_=pen.getPen(*this);TRACEPOINT;}

	/**
	 * @brief if show=true the match will be visible if both keypoints are in the
	 * visibleArea of its images
	 * @param b new show value
	 */
	void setShow(const bool& b);

	/**
	 * @brief returns the show value
	 * @return the show value
	 */
	bool isShown() const
		{TRACEPOINT;return show_;}

	/**
	 * @brief this slot will be called if the right keypoint has changed
	 * @param visible if the rightKey in the visibleArea of its image
	 */
	virtual void updateRightKey(bool visible);

	/**
	 * @brief this slot will be called if the left keypoint has changed
	 * @param visible if the leftKey in the visibleArea of its image
	 */
	virtual void updateLeftKey(bool visible);

protected:

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
