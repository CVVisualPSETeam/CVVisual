#include <array>

#include <QHBoxLayout>
#include <QWidget>

#include "singlefilterview.hpp"
#include "../qtutil/accordion.hpp"
#include "../qtutil/zoomableimageoptpanel.hpp"
#include "../qtutil/autofilterwidget.hpp"
#include "../qtutil/zoomableimage.hpp"
#include "../util/util.hpp"

namespace cvv
{
namespace view
{

SingleFilterView::SingleFilterView(const std::vector<cv::Mat> &images,
                                   QWidget *parent)
    : FilterView{ parent }
{
	TRACEPOINT;
	auto imwid = util::make_unique<QWidget>();
	auto accor = util::make_unique<qtutil::Accordion>();
	auto layout = util::make_unique<QHBoxLayout>();
	auto imageLayout = util::make_unique<QHBoxLayout>();

	accor->setMinimumWidth(250); // ggf anpassen
	accor->setMaximumWidth(250);

	auto filterSelector =
	    util::make_unique<qtutil::AutoFilterWidget<1, 1>>(this);

	qtutil::AutoFilterWidget<1, 1> *filterSel = filterSelector.get();

	accor->insert("Select a Filter", std::move(filterSelector));

	int count = 0;
	for (auto &image : images)
	{
		auto zoomIm = util::make_unique<qtutil::ZoomableImage>();

		// für das AutofilterWidget muss die output Mat gespeichert
		// werden dies übernimmt
		// das ZoomableImage
		auto filterSignals = filterSel->addEntry(
		    QString("image: ") + QString::number(count),
		    { { util::makeRef<const cv::Mat>(image) } },
		    { { util::makeRef<cv::Mat>(zoomIm->mat()) } });

		// connect entry=> zoomableimage
		connect(filterSignals.front().getPtr(),
		        SIGNAL(signal(cv::Mat &)), zoomIm.get(),
		        SLOT(setMatR(cv::Mat &)));

		accor->insert(
		    QString("ImageInformation: ") + QString::number(count),
		    std::move(
		        util::make_unique<qtutil::ZoomableOptPanel>(*zoomIm)));

		zoomIm->setMat(image);
		imageLayout->addWidget(zoomIm.release());
		count++;
	}

	imwid->setLayout(imageLayout.release());

	layout->addWidget(accor.release());
	layout->addWidget(imwid.release());

	setLayout(layout.release());

	TRACEPOINT;
}
}
} // namespaces
