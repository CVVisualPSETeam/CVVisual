#include <array>

#include <QHBoxLayout>
#include <QWidget>

#include "singlefilterview.hpp"
#include "../qtutil/accordion.hpp"
#include "../qtutil/matinfowidget.hpp"
#include "../qtutil/autofilterwidget.hpp"
#include "../qtutil/zoomableimage.hpp"
#include "../util/util.hpp"


namespace cvv{ namespace view{

SingleFilterView::SingleFilterView(const std::vector<cv::Mat>& images,QWidget *parent):
	FilterView{parent}
{
	TRACEPOINT;
	QWidget 		*imwid		= new QWidget{};
	qtutil::Accordion 	*accor 		= new qtutil::Accordion{};
	QHBoxLayout		*layout 	= new QHBoxLayout{};
	QHBoxLayout		*imageLayout 	= new QHBoxLayout{};

	accor->setMinimumWidth(250);
	accor->setMaximumWidth(250);

	auto filterSelector = util::make_unique<qtutil::AutoFilterWidget<1,1>>();
	qtutil::AutoFilterWidget<1,1> *filterSel = filterSelector.get();

	accor->insert("Select a Filter",std::move(filterSelector));
	int count = 0;
	for(auto& image:images)
	{
		qtutil::ZoomableImage *zoomIm=(new qtutil::ZoomableImage{});
		auto info = util::make_unique<qtutil::MatInfoWidget>(image);
		outputs_.emplace_back();
		filterSel->addEntry("image",{{util::makeRef<const cv::Mat>(image)}},{{util::makeRef<cv::Mat>(outputs_.back())}});
		connect(zoomIm,SIGNAL(updateConversionResult(ImageConversionResult)),info.get(),
			SLOT(updateConvertStatus(ImageConversionResult)));

		connect(info.get(),SIGNAL(getZoom(qreal)),zoomIm,
			SLOT(updateZoom(qreal)));

		zoomIm->setMat(image);

		imageLayout->addWidget(zoomIm);
		accor->insert("ImageInformation",std::move(info));
		count++;
	}
	imwid->setLayout(imageLayout);

	layout->addWidget(accor);
	layout->addWidget(imwid);
	setLayout(layout);

	TRACEPOINT;
}

}}//namespaces
