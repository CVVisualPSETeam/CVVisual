#include <array>

#include <QHBoxLayout>
#include <QWidget>

#include "singlefilterview.hpp"
#include "../qtutil/accordion.hpp"
#include "../qtutil/matinfowidget.hpp"


namespace cvv{ namespace view{

	SingleFilterView::SingleFilterView(const std::vector<cv::Mat>& images,QWidget *parent):
		FilterView{parent},images_(images)
	{
		QWidget 			*imwid		= new QWidget{};
		qtutil::Accordion 	*accor 		= new qtutil::Accordion{};
		QHBoxLayout			*layout 	= new QHBoxLayout{};
		QHBoxLayout			*imageLayout 	= new QHBoxLayout{};

		auto filterSelector	= util::make_unique<qtutil::FilterSelectorWidget<1,1>>();
		filterSelector_ = filterSelector.get();
		
		accor->setMinimumSize(250,0);
		connect(&(filterSelector->sigFilterSettingsChanged_),SIGNAL(signal()),this,SLOT(applyFilter()));
		accor->insert("Select a Filter",std::move(filterSelector));
		int count = 0;
		for(auto image:images_)
		{
			zoomImages_.push_back(new qtutil::ZoomableImage{});
			auto info = util::make_unique<qtutil::MatInfoWidget>(image);

			connect(zoomImages_[count],SIGNAL(updateConversionResult(ImageConversionResult)),info.get(),
				SLOT(updateConvertStatus(ImageConversionResult)));
			zoomImages_[count]->updateMat(image);

			imageLayout->addWidget(zoomImages_[count]);
			accor->insert("ImageInformation",std::move(info));
			count++;
		}
		
		imwid->setLayout(imageLayout);

		layout->addWidget(accor);
		layout->addWidget(imwid);		
		setLayout(layout);
	}
	
	void SingleFilterView::applyFilter()
	{
		int count = 0;
		for(auto image : images_)
		{
			const std::array<const cv::Mat,1> input{image};
			auto result = filterSelector_->checkInput(input);
			if(result.first){
				std::array<cv::Mat,1> out;
				filterSelector_->applyFilter(input,out);
				zoomImages_[count]->updateMat(out[0]);
			}
			count++;
		}
	}

}}//namespaces
