
#include <QColorDialog>
#include <QVBoxLayout>

#include "singlecolorpen.hpp"

namespace cvv{ namespace qtutil{

	SingleColorPen::SingleColorPen(QWidget * parent):MatchPen{parent}
	{
		TRACEPOINT;
		QVBoxLayout *layout=new QVBoxLayout();
		layout->setMargin(0);
		QColorDialog *colorDialog = new QColorDialog{};
		layout->addWidget(colorDialog);
		setLayout(layout);

		connect(colorDialog,SIGNAL(colorSelected(const QColor &)),
			this,SLOT(updateColor(const QColor &)));
		TRACEPOINT;
	}

	void SingleColorPen::updateColor(const QColor& color)
	{
		TRACEPOINT;
		pen_=QPen{color};
		emit settingsChanged(*this);
		TRACEPOINT;
	}
}}
