
#include "matchpointpen.hpp"

namespace cvv { namespace qtutil {

QPen MatchPointPen::getPen(const CVVPointMatch &match)const
{
	TRACEPOINT;
	QColor color{pen_.color()};
	color.setAlpha(255*(match.matchValue()/maxDistance_));
	TRACEPOINT;
	return QPen{color};
}

qreal MatchPointPen::getRadius(const CVVPointMatch &) const
{
	TRACEPOINT;
	return 2.0;
}

QBrush MatchPointPen::getBrush(const CVVPointMatch & match)const
{
	TRACEPOINT;
	QColor color{pen_.color()};
	color.setAlpha(255*(match.matchValue()/maxDistance_));
	TRACEPOINT;
	return QBrush{color};
}

}}
