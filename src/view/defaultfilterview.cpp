#include <QHBoxLayout>
#include <QWidget>

#include "defaultfilterview.hpp"
#include "../qtutil/accordion.hpp"
#include "../qtutil/zoomableimageoptpanel.hpp"
#include "../qtutil/zoomableimage.hpp"
#include "../qtutil/synczoomwidget.hpp"
#include "../util/util.hpp"

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

	std::vector<qtutil::ZoomableImage*> syncVec;

	for (auto image : images)
	{
		auto zoomIm = util::make_unique<qtutil::ZoomableImage>();

		syncVec.push_back(zoomIm.get());

		accor->insert(
		    "ImageInformation",
		    std::move(
			util::make_unique<qtutil::ZoomableOptPanel>(*zoomIm)));

		zoomIm->setMat(image);

		imageLayout->addWidget(zoomIm.release());
	}

	accor->insert("Zoom synchronization",
		util::make_unique<qtutil::SyncZoomWidget>(syncVec), true, 0);

	imwid->setLayout(imageLayout.release());

	layout->addWidget(accor.release());
	layout->addWidget(imwid.release());

	setLayout(layout.release());
	//images should be seen fully at beginning
	for(auto& zoomableImage: syncVec)
	{
		zoomableImage->showFullImage();
	}
	TRACEPOINT;
}
}
} // namespaces
