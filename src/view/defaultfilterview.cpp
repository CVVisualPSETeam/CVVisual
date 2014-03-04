#include <QHBoxLayout>
#include <QWidget>

#include "defaultfilterview.hpp"
#include "../qtutil/accordion.hpp"
#include "../qtutil/zoomableimageoptpanel.hpp"
#include "../qtutil/zoomableimage.hpp"

namespace cvv
{
namespace view
{

DefaultFilterView::DefaultFilterView(const std::vector<cv::Mat> &images,
                                     QWidget *parent)
    : FilterView{ parent }
{
	TRACEPOINT;

	auto layout = util::make_unique<QHBoxLayout>();
	auto accor = util::make_unique<qtutil::Accordion>();
	auto imwid = util::make_unique<QWidget>();
	auto imageLayout = util::make_unique<QHBoxLayout>();

	accor->setMinimumWidth(250);
	accor->setMaximumWidth(250);

	for (auto image : images)
	{
		auto zoomIm = util::make_unique<qtutil::ZoomableImage>();

		accor->insert(
		    "ImageInformation",
		    std::move(
		        util::make_unique<qtutil::ZoomableOptPanel>(*zoomIm)));

		zoomIm->setMat(image);

		imageLayout->addWidget(zoomIm.release());
	}

	imwid->setLayout(imageLayout.release());

	layout->addWidget(accor.release());
	layout->addWidget(imwid.release());

	setLayout(layout.release());
	TRACEPOINT;
}
}
} // namespaces
