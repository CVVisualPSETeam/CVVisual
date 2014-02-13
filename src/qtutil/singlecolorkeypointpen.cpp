#include <QHBoxLayout>
#include <QPushButton>

#include "singlecolorkeypointpen.hpp"

namespace cvv{namespace qtutil{

SingleColorKeyPen::SingleColorKeyPen(QWidget * parent):KeyPointPen{parent},colordia_{new QColorDialog{this}}
{
	TRACEPOINT;
	QVBoxLayout *layout=new QVBoxLayout();
	layout->setMargin(0);
	QPushButton *button=new QPushButton{"Color Dialog"};
	layout->addWidget(button);
	setLayout(layout);

	connect(colordia_,SIGNAL(currentColorChanged(const QColor &)),
		this,SLOT(updateColor(const QColor &)));
	connect(button,SIGNAL(clicked(bool)),this,SLOT(colorButtonClicked()));
	TRACEPOINT;
}

void SingleColorKeyPen::updateColor(const QColor& color)
{
	TRACEPOINT;
	pen_=QPen{color};
	emit settingsChanged(*this);
	TRACEPOINT;
}
}}
