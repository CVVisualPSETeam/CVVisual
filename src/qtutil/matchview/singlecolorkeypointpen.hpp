#ifndef CVVISUAL_SINGLE_COLOR_KEY_PEN
#define CVVISUAL_SINGLE_COLOR_KEY_PEN

#include <QColorDialog>
#include <QPen>

#include "keypointpen.hpp"
#include "../../dbg/dbg.hpp"

namespace cvv{ namespace qtutil{

class SingleColorKeyPen:public KeyPointPen{
Q_OBJECT
public:
	SingleColorKeyPen(QWidget *parent=nullptr);

	~SingleColorKeyPen()
	{
		TRACEPOINT;
		colordia_->deleteLater();
		TRACEPOINT;
	}

	virtual QPen getPen(const CVVKeyPoint&)const override
		{TRACEPOINT; return pen_;}
public slots:
	void updateColor(const QColor& color);


private slots:
	void colorButtonClicked()
		{TRACEPOINT;
		colordia_->show();
		TRACEPOINT;}

private:
	QColorDialog *colordia_;
	QPen pen_;
};
}}
#endif
