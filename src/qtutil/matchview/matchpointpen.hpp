#ifndef CVVISUAL_MATCH_POINT_PEN
#define CVVISUAL_MATCH_POINT_PEN

#include "singlecolorpen.hpp"
#include "cvvpointmatch.hpp"

namespace cvv{ namespace qtutil{

class CVVPointMatch;

class MatchPointPen: public SingleColorPen{

Q_OBJECT
public:
	MatchPointPen(float maxDistance,QWidget *parent=nullptr):
		SingleColorPen{parent},maxDistance_{maxDistance}
		{TRACEPOINT;}

	virtual QPen getPen(const CVVPointMatch& match)const;

	virtual qreal getRadius(const CVVPointMatch&)const;

	virtual QBrush getBrush(const CVVPointMatch&) const;

private:
	float maxDistance_;
};
}}
#endif
