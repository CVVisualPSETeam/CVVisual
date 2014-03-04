#ifndef CVVISUAL_COLOR_UTIL
#define CVVISUAL_COLOR_UTIL

#include <QColor>

namespace cvv
{
namespace qtutil
{

QColor getFalseColor(double value, double max, double min = 0)
{
	TRACEPOINT;
	QColor color{};
	color.setHsvF(((value - min) / (max - min)) * 255, 255, 255);
	TRACEPOINT;
	return color;
}
}
}
#endif
