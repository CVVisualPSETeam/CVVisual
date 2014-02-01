#include <QHBoxLayout>
#include <QWidget>

#include "defaultfilterview.hpp"
#include "../qtutil/accordion.hpp"
#include "../qtutil/matinfowidget.hpp"
#include "../qtutil/zoomableimage.hpp"

namespace cvv{ namespace view{

	DefaultFilterView::DefaultFilterView(const std::vector<cv::Mat>& images,QWidget *parent):
		FilterView{parent},images_(images)
	{
		QHBoxLayout* layout = new QHBoxLayout{};
		qtutil::Accordion *accor = new qtutil::Accordion{};
		accor->insert("this is a accordion",*(new QWidget{}));
		accor->setMinimumSize(150,0);
		QWidget *imwid = new QWidget{};
		QHBoxLayout* imageLayout = new QHBoxLayout{};
		layout->addWidget(accor);

		for(auto image:images_)
		{
			qtutil::ZoomableImage *zoomim =new qtutil::ZoomableImage{};
			qtutil::MatInfoWidget *info=new qtutil::MatInfoWidget{image};

			connect(zoomim,SIGNAL(updateConversionResult(ImageConversionResult)),info,
				SLOT(updateConvertStatus(ImageConversionResult)));
			zoomim->updateMat(image);

			imageLayout->addWidget(zoomim);
			accor->insert("ImageInformation",*info);
		}
		imwid->setLayout(imageLayout);
		layout->addWidget(imwid);

		setLayout(layout);
	}

/*	static DefaultFilterView create(const std::vecotr<InputArray>& input)
	{
		return DefaultFilterView{input};
	};
*/	
}}//namespaces
