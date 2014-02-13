#include <QHBoxLayout>
#include <QWidget>

#include "defaultfilterview.hpp"
#include "../qtutil/accordion.hpp"
#include "../qtutil/zoomableimageoptpanel.hpp"
#include "../qtutil/zoomableimage.hpp"

namespace cvv{ namespace view{

DefaultFilterView::DefaultFilterView(const std::vector<cv::Mat>& images,QWidget *parent):
	FilterView{parent}
{
	TRACEPOINT;

	QHBoxLayout* layout = new QHBoxLayout{this};
	qtutil::Accordion *accor = new qtutil::Accordion{this};
	accor->setMinimumWidth(250);
	accor->setMaximumWidth(250);

	QWidget *imwid = new QWidget{};
	QHBoxLayout* imageLayout = new QHBoxLayout{this};
	layout->addWidget(accor);

	for(auto image:images)
	{
		qtutil::ZoomableImage *zoomIm =new qtutil::ZoomableImage{};

		imageLayout->addWidget(zoomIm);
		accor->insert("ImageInformation",std::move(util::make_unique<qtutil::ZoomableOptPanel>(*zoomIm,this)));
		zoomIm->setMat(image);
	}
	imwid->setLayout(imageLayout);
	layout->addWidget(imwid);

	setLayout(layout);

	TRACEPOINT;
}

}}//namespaces
