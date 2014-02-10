#ifndef CVVISUAL_SINGLE_COLOR_MATCHPEN
#define CVVISUAL_SINGLE_COLOR_MATCHPEN

#include "matchpen.hpp"
#include "cvvmatch.hpp"
#include <QPen>
#include <QColor>
#include <QWidget>
namespace cvv{namespace qtutil{

class SingleColorPen:public MatchPen{
Q_OBJECT
public:
	SingleColorPen(QWidget * parent =nullptr);

	virtual QPen getPen(const CVVMatch& cvmatch)const override
		{TRACEPOINT;(void)cvmatch;TRACEPOINT;return pen_;}

public slots:

	void updateColor(const QColor& color);

private:
	QPen pen_;
};
}}
#endif
