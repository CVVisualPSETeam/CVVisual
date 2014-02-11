
#include <QPushButton>
#include <QVBoxLayout>

#include "singlecolorpen.hpp"

namespace cvv{ namespace qtutil{

	SingleColorPen::SingleColorPen(QWidget * parent):MatchPen{parent}
	{
		TRACEPOINT;
		QVBoxLayout *layout=new QVBoxLayout();
		layout->setMargin(0);
		colorDialog_ = new QColorDialog{};
		QPushButton *button=new QPushButton{"Color Dialog"};
		layout->addWidget(button);
		setLayout(layout);

		connect(colorDialog_,SIGNAL(currentColorChanged(const QColor &)),
			this,SLOT(updateColor(const QColor &)));
		connect(button,SIGNAL(clicked(bool)),this,SLOT(colorButtonClicked()));
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
