
#include <QPushButton>
#include <QVBoxLayout>

#include "singlecolorpen.hpp"

namespace cvv{ namespace qtutil{

SingleColorPen::SingleColorPen(QWidget * parent):MatchPen{parent}
{
	TRACEPOINT;
	auto layout	= util::make_unique<QVBoxLayout>();
	colorDialog_	= new QColorDialog{};//wird im Destructor zerstört
	auto button	= util::make_unique<QPushButton>("Color Dialog");

	connect(colorDialog_,SIGNAL(currentColorChanged(const QColor &)),
		this,SLOT(updateColor(const QColor &)));

	connect(button.get(),SIGNAL(clicked(bool)),this,SLOT(colorButtonClicked()));

	layout->setMargin(0);
	layout->addWidget(button.release());

	setLayout(layout.release());
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
