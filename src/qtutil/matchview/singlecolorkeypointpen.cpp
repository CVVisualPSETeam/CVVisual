#include <QHBoxLayout>
#include <QPushButton>

#include "singlecolorkeypointpen.hpp"
#include "../../util/util.hpp"

namespace cvv{namespace qtutil{

SingleColorKeyPen::SingleColorKeyPen(QWidget * parent):KeyPointSettings{parent},
	colordia_{new QColorDialog{}}
{
	TRACEPOINT;
	auto layout=util::make_unique<QVBoxLayout>();
	auto button=util::make_unique<QPushButton>("Color Dialog");

	layout->setMargin(0);

	connect(colordia_,SIGNAL(currentColorChanged(const QColor &)),
		this,SLOT(updateColor(const QColor &)));
	connect(button.get(),SIGNAL(clicked(bool)),this,SLOT(colorButtonClicked()));

	layout->addWidget(button.release());
	setLayout(layout.release());

	TRACEPOINT;
}

void SingleColorKeyPen::setSettings(CVVKeyPoint &keypoint)
{
	TRACEPOINT;
	keypoint.setPen(pen_);
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
