#ifndef CVVISUAL_COLOR_UTIL
#define CVVISUAL_COLOR_UTIL

#include <QColor>

namespace cvv
{
namespace qtutil
{
//TODO Replace
QColor getFalseColor(double value, double max, double min)
{
	QColor color{};
	color.setHsvF(((value - min) / (max - min)) * 255, 255, 255);
	return color;
}

}
}
#endif
