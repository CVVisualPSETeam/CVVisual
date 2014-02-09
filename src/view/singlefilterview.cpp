#include <array>

#include <QHBoxLayout>
#include <QWidget>

#include "singlefilterview.hpp"
#include "../qtutil/accordion.hpp"
#include "../qtutil/zoomableimage.hpp"
#include "../qtutil/matinfowidget.hpp"
#include "../qtutil/autofilterwidget.hpp"
#include "../util/util.hpp"

namespace cvv{ namespace view{

SingleFilterView::SingleFilterView(std::vector<cv::Mat> images,QWidget *parent):
	FilterView{parent}
{
	TRACEPOINT;

	QWidget 		*imwid		= new QWidget{};
	qtutil::Accordion 	*accor 		= new qtutil::Accordion{};
	QHBoxLayout		*layout 	= new QHBoxLayout{};
	QHBoxLayout		*imageLayout 	= new QHBoxLayout{};

	auto filterSelector	= util::make_unique<qtutil::AutoFilterWidget<1,1>>();
	qtutil::AutoFilterWidget<1,1> *filterSelectorptr = filterSelector.get();

	accor->setMinimumWidth(250);
	accor->setMaximumWidth(250);
	accor->insert("Select a Filter",std::move(filterSelector));
	for(auto& image:images)
	{
		//AutoFilter
		cv::Mat out;
		cv::Mat outMat{image};
		std::array<util::Reference<const cv::Mat>,1> inArray{{util::makeRef<const cv::Mat>(image)}};
		std::array<util::Reference<cv::Mat>,1> outArray{{util::makeRef<cv::Mat>(outMat)}};
		
		const std::vector<util::Reference<qtutil::SignalMatRef>>& matSignals=filterSelectorptr->addEntry("Image ", inArray, outArray);

		//ZoomableIamge
		qtutil::ZoomableImage *zoomIm=new qtutil::ZoomableImage{};

		//MatInfoWidget
		auto info = util::make_unique<qtutil::MatInfoWidget>(image);

		//#Signals MatInfoWidget <=> ZoomableImage
		connect(zoomIm,SIGNAL(updateConversionResult(ImageConversionResult)),info.get(),
			SLOT(updateConvertStatus(ImageConversionResult)));

		connect(info.get(),SIGNAL(getZoom(qreal)),zoomIm,SLOT(updateZoom(qreal)));	

		//Signals AutoFilter

		connect(matSignals.front().getPtr(),SIGNAL(signal(cv::Mat)),zoomIm,SLOT(updateMat(cv::Mat)));

		zoomIm->updateMat(image);
		//Layout
		imageLayout->addWidget(zoomIm);
		accor->insert("Image Information",std::move(info));
	}

	imwid->setLayout(imageLayout);

	layout->addWidget(accor);
	layout->addWidget(imwid);
	setLayout(layout);

	TRACEPOINT;
}

}}//namespaces
