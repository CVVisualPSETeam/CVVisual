#ifndef CVVISUAL_SINGLE_COLOR_MATCHPEN
#define CVVISUAL_SINGLE_COLOR_MATCHPEN

#include <QPen>
#include <QColor>
#include <QWidget>
#include <QColorDialog>

#include "matchpen.hpp"
#include "cvvmatch.hpp"
#include "../../dbg/dbg.hpp"

namespace cvv{namespace qtutil{

class SingleColorPen:public MatchPen{
Q_OBJECT
public:
	SingleColorPen(QWidget * parent =nullptr);

	~SingleColorPen()
	{
		TRACEPOINT;
		colorDialog_->deleteLater();
		TRACEPOINT;
	}
	
	virtual QPen getPen(const CVVMatch& cvmatch)const override
		{TRACEPOINT;(void)cvmatch;TRACEPOINT;return pen_;}

public slots:

	void updateColor(const QColor& color);

private slots:
	void colorButtonClicked()
		{colorDialog_->show();}
private:
	QPen pen_;
	QColorDialog *colorDialog_;
};
}}
#endif
