#include <array>

#include <QHBoxLayout>
#include <QWidget>

#include "singlefilterview.hpp"
#include "../qtutil/accordion.hpp"
#include "../qtutil/zoomableimageoptpanel.hpp"
#include "../qtutil/autofilterwidget.hpp"
#include "../qtutil/zoomableimage.hpp"
#include "../util/util.hpp"


namespace cvv{ namespace view{

SingleFilterView::SingleFilterView(const std::vector<cv::Mat>& images,QWidget *parent):
	FilterView{parent}
{
	TRACEPOINT;
	QWidget 		*imwid		= new QWidget{this};
	qtutil::Accordion 	*accor 		= new qtutil::Accordion{this};
	QHBoxLayout		*layout 	= new QHBoxLayout{this};
	QHBoxLayout		*imageLayout 	= new QHBoxLayout{this};

	accor->setMinimumWidth(250);
	accor->setMaximumWidth(250);

	auto filterSelector = util::make_unique<qtutil::AutoFilterWidget<1,1>>(this);
	qtutil::AutoFilterWidget<1,1> *filterSel = filterSelector.get();

	accor->insert("Select a Filter",std::move(filterSelector));
	int count = 0;
	for(auto& image:images)
	{
		qtutil::ZoomableImage *zoomIm = new qtutil::ZoomableImage{};

		filterSel->addEntry(QString("image: ")+QString::number(count),
			{{util::makeRef<const cv::Mat>(image)}},
			{{util::makeRef<cv::Mat>(zoomIm->mat())}});
		zoomIm->setMat(image);

		//connect entry zoomableimage
		imageLayout->addWidget(zoomIm);
		accor->insert("ImageInformation",std::move(util::make_unique<qtutil::ZoomableOptPanel>(*zoomIm)));
		count++;
	}
	imwid->setLayout(imageLayout);

	layout->addWidget(accor);
	layout->addWidget(imwid);
	setLayout(layout);

	TRACEPOINT;
}

}}//namespaces
