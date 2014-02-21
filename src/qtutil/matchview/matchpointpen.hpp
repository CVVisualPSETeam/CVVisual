#ifndef CVVISUAL_MATCH_POINT_PEN
#define CVVISUAL_MATCH_POINT_PEN

#include "singlecolormatchpen.hpp"
#include "cvvpointmatch.hpp"

namespace cvv{ namespace qtutil{

class CVVPointMatch;

class MatchPointPen: public SingleColorMatchPen{

Q_OBJECT
public:
	MatchPointPen(float maxDistance,QWidget *parent=nullptr):
		SingleColorMatchPen{parent},maxDistance_{maxDistance}
		{TRACEPOINT;}

	virtual void setSettings(CVVMatch& match)override;

	virtual void setSettings(CVVPointMatch&);

public slots:
	QPen getPen(const CVVPointMatch &match) const;
	qreal getRadius(const CVVPointMatch &) const;
	QBrush getBrush(const CVVPointMatch &match) const;
private:
	float maxDistance_;
};
}}
#endif
