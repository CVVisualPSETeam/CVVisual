
#include <opencv2/core/core.hpp>

#include <Qt>
#include "QLabel"
#include "QSlider"
#include "QVBoxLayout"

#include "../../dbg/dbg.hpp"
#include "../../util/util.hpp"
#include "overlayfilterwidget.hpp"

namespace cvv
{
namespace qtutil
{

OverlayFilterWidget::OverlayFilterWidget(QWidget *parent)
    : FilterFunctionWidget<2, 1>{ parent }, opacityOfOriginalImg_{ 0.5 }
{
	TRACEPOINT;

	auto layout = util::make_unique<QVBoxLayout>();
	auto slider = util::make_unique<QSlider>(Qt::Horizontal);

	slider->setRange(0, 100);
	slider->setTickPosition(QSlider::TicksAbove);
	slider->setTickInterval(10);

	connect(slider.get(), SIGNAL(valueChanged(int)), this,
	        SLOT(updateOpacity(int)));

	// Add title of slider and slider to the layout
	layout->addWidget(util::make_unique<QLabel>(
	    "Select opacity of original image").release());
	layout->addWidget(slider.release());
	setLayout(layout.release());

	TRACEPOINT;
}

void OverlayFilterWidget::applyFilter(InputArray in, OutputArray out) const
{
	TRACEPOINT;

	auto check = checkInput(in);
	if (!check.first)
	{
		TRACEPOINT;
		return;
	}

	cv::addWeighted(in.at(0).get(), opacityOfOriginalImg_, in.at(1).get(),
	                1 - opacityOfOriginalImg_, 0, out.at(0).get());

	TRACEPOINT;
}

std::pair<bool, QString> OverlayFilterWidget::checkInput(InputArray in) const
{
	TRACEPOINT;
	// check whether images have same size
	if (in.at(0).get().size() != in.at(1).get().size())
	{
		return std::make_pair(false, "images need to have same size");
	}

	// check whether images have same number of channels
	if (in.at(0).get().channels() != in.at(1).get().channels())
	{
		return std::make_pair(
		    false, "images need to have same number of channels");
	}

	TRACEPOINT;

	return std::make_pair(true, "images can be converted");
}

void OverlayFilterWidget::updateOpacity(int newOpacity)
{
	TRACEPOINT;
	opacityOfOriginalImg_ = newOpacity / 100.0;
	signalFilterSettingsChanged().emitSignal();
	TRACEPOINT;
}
}
}
