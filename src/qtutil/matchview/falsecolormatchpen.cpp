
#include <QPushButton>
#include <QDoubleSpinBox>
#include <QLabel>
#include <QVBoxLayout>

#include "falsecolormatchpen.hpp"
#include "colorutil.hpp"
#include "cvvmatch.hpp"
#include "../../util/util.hpp"

namespace cvv
{
namespace qtutil
{

FalseColorMatchPen::FalseColorMatchPen(double max, double min, QWidget *parent)
    : MatchSettings{ parent }, max_{ max }, min_{ min }
{
	TRACEPOINT;
	auto layout = util::make_unique<QVBoxLayout>();
	auto button = util::make_unique<QPushButton>("use false color");
	auto minLabel = util::make_unique<QLabel>("min");
	auto minSpin = util::make_unique<QDoubleSpinBox>();
	auto maxLabel = util::make_unique<QLabel>("max");
	auto maxSpin = util::make_unique<QDoubleSpinBox>();

	minSpin->setValue(min_);
	maxSpin->setValue(max_);

	connect(button.get(), SIGNAL(clicked()), this, SLOT(emitSignal));

	connect(minSpin.get(), SIGNAL(valueChanged(double)), this,
	        SLOT(updateMin(double)));
	connect(maxSpin.get(), SIGNAL(valueChanged(double)), this,
	        SLOT(updateMax(double)));

	layout->addWidget(minLabel.release());
	layout->addWidget(minSpin.release());
	layout->addWidget(maxLabel.release());
	layout->addWidget(maxSpin.release());
	layout->addWidget(button.release());
	TRACEPOINT;
}

void FalseColorMatchPen::setSettings(CVVMatch &match)
{
	TRACEPOINT;
	match.setPen(QPen{ getFalseColor(
	    static_cast<double>(match.match().distance), max_, min_) });
	TRACEPOINT;
}
}
}
