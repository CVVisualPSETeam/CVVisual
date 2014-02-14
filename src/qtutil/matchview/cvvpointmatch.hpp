#ifndef CVVISUAL_CVV_POINT_MATCH
#define CVVISUAL_CVV_POINT_MATCH

#include "cvvmatch.hpp"

namespace cvv{namespace qtutil{

class CVVPointMatch:CVVMatch{
Q_OBJECT
public:
	CVVPointMatch(CVVKeyPoint *left_key,CVVKeyPoint *right_key,const float& matchValue,bool isLeftKey=true,qreal radius=5,const QPen& pen= QPen{Qt::red},QGraphicsItem *parent=nullptr);
	


	virtual QRectF boundingRect() const override;

	virtual void paint(QPainter *painter,const QStyleOptionGraphicsItem *,QWidget *)override;



protected:
	
	bool isLeftKey_;
	qreal radius_;
};

}}
#endif
