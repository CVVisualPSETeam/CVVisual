#include <QHBoxLayout>
#include <QWidget>

#include "defaultfilterview.hpp"
#include "../qtutil/accordion.hpp"
#include "../qtutil/matinfowidget.hpp"
#include "../qtutil/zoomableimage.hpp"

namespace cvv{ namespace view{

	DefaultFilterView::DefaultFilterView(const std::vector<cv::Mat>& images,QWidget *parent):
		FilterView{parent}
	{
		TRACEPOINT;		

		QHBoxLayout* layout = new QHBoxLayout{};
		qtutil::Accordion *accor = new qtutil::Accordion{};
		accor->insert("this is an accordion", util::make_unique<QWidget>());
		accor->setMinimumSize(250,0);
		QWidget *imwid = new QWidget{};
		QHBoxLayout* imageLayout = new QHBoxLayout{};
		layout->addWidget(accor);

		for(auto image:images)
		{
			qtutil::ZoomableImage *zoomim =new qtutil::ZoomableImage{};
			auto info = util::make_unique<qtutil::MatInfoWidget>(image);

			connect(zoomim,SIGNAL(updateConversionResult(ImageConversionResult)),info.get(),
				SLOT(updateConvertStatus(ImageConversionResult)));
			

			connect(info.get(),SIGNAL(getZoom(qreal)),zoomim,
				SLOT(updateZoom(qreal)));


			zoomim->updateMat(image);

			imageLayout->addWidget(zoomim);
			accor->insert("ImageInformation", std::move(info));
		}
		imwid->setLayout(imageLayout);
		layout->addWidget(imwid);

		setLayout(layout);

		TRACEPOINT;
	}
	
}}//namespaces
