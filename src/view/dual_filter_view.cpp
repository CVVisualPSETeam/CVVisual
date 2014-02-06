#include <exception>
#include <memory>
#include <iostream> //for debugging

#include <QHBoxLayout>
#include <QLabel>
#include <QImage>
#include <QPixmap>
#include <QApplication>
#include <QtGui>
#include <QString>

#include "../dbg/dbg.hpp"
#include "../qtutil/accordion.hpp"
#include "../qtutil/diffFilterFunction.hpp"
#include "../qtutil/filterselectorwidget.hpp"
#include "../qtutil/matinfowidget.hpp"
#include "../qtutil/signalslot.hpp"
#include "../qtutil/util.hpp"
#include "../util/util.hpp"
#include "dual_filter_view.hpp"

namespace cvv
{
namespace view
{
	DualFilterView::DualFilterView(std::array<cv::Mat, 2> images, QWidget* parent)
		: FilterView{parent}, rawImages_(images),
		slotFilterSelectedChanged_{[this](){
			std::cout << "slotFilterSelectedChanged was called" << std::endl;
			auto result = filterSelector_->checkInput(rawImages_);
			std::cout << "checkInput(rawImages_): " << result.second.toStdString() << std::endl;
			if(result.first){
				std::array<cv::Mat,1> out;
				out = filterSelector_-> applyFilter(rawImages_,out);
				zoomImages_.at(1).updateMat(out.at(0));
			}
		}}
	{
		TRACEPOINT;
		QHBoxLayout* layout = new QHBoxLayout{};
		QHBoxLayout* imageLayout = new QHBoxLayout{};
		QWidget *imwid = new QWidget{};
		qtutil::Accordion *accor = new qtutil::Accordion{};



		auto filterWidget = util::make_unique<qtutil::FilterSelectorWidget<2,1>>();
		filterSelector_ = filterWidget.get();

		cvv::qtutil::FilterSelectorWidget<2,1>::registerElement("Difference image - grayscale",
		[](QWidget* parent)
		{
			return std::unique_ptr<cvv::qtutil::FilterFunctionWidget<2,1>>
				{new qtutil::DiffFilterFunction{qtutil::DiffFilterType::GRAYSCALE, parent}};
		});

		cvv::qtutil::FilterSelectorWidget<2,1>::registerElement("Difference image - hue",
		[](QWidget* parent)
		{
			return std::unique_ptr<cvv::qtutil::FilterFunctionWidget<2,1>>
				{new qtutil::DiffFilterFunction{qtutil::DiffFilterType::HUE, parent}};
		});

		cvv::qtutil::FilterSelectorWidget<2,1>::registerElement("Difference image - saturation",
		[](QWidget* parent)
		{
			return std::unique_ptr<cvv::qtutil::FilterFunctionWidget<2,1>>
				{new qtutil::DiffFilterFunction{qtutil::DiffFilterType::SATURATION, parent}};
		});

		cvv::qtutil::FilterSelectorWidget<2,1>::registerElement("Difference image - value",
		[](QWidget* parent)
		{
			return std::unique_ptr<cvv::qtutil::FilterFunctionWidget<2,1>>
				{new qtutil::DiffFilterFunction{qtutil::DiffFilterType::VALUE, parent}};
		});

	//	connect(&(filterSelector_->sigSelect_),SIGNAL(signal()),&slotFilterSelectedChanged_,SLOT(slot()));
		connect(&(filterSelector_->sigSelect_),&qtutil::Signal::signal,&slotFilterSelectedChanged_,&qtutil::Slot::slot);

		accor->insert("Filter selection", std::move(filterWidget));
		accor->setMinimumSize(150,0);
		layout->addWidget(accor);

		auto lambda = [this, imageLayout, accor](cv::Mat image, size_t count)
			{
				auto info = util::make_unique<qtutil::MatInfoWidget>(image);

				connect(&zoomImages_.at(count),SIGNAL(updateConversionResult(ImageConversionResult)),info.get(),
					SLOT(updateConvertStatus(ImageConversionResult)));

				connect(info.get(),SIGNAL(getZoom(qreal)),&zoomImages_.at(count),
					SLOT(updateZoom(qreal)));

				zoomImages_.at(count).updateMat(image);

				imageLayout->addWidget(&(zoomImages_.at(count)));
				accor->insert("ImageInformation",std::move(info));
			};

		lambda(rawImages_.at(0), 0);

		std::array<cv::Mat, 1> imageArray;
		qtutil::DiffFilterFunction defaultFilter {qtutil::DiffFilterType::GRAYSCALE};
		defaultFilter.applyFilter(rawImages_, imageArray);
		lambda(imageArray.at(0), 1);

		lambda(rawImages_.at(1), 2);

		imwid->setLayout(imageLayout);
		layout->addWidget(imwid);

		setLayout(layout);

		TRACEPOINT;
	}

	DualFilterView::DualFilterView(const std::vector<cv::Mat>& images, QWidget* parent)
	:DualFilterView(convertToArray(images), parent)
	{}

	void DualFilterView::applyFilter()
	{
		TRACEPOINT;

		auto result = filterSelector_->checkInput(rawImages_);
		if(result.first){
			std::array<cv::Mat,1> out;
			out = filterSelector_-> applyFilter(rawImages_,out);
			zoomImages_.at(1).updateMat(out.at(0));
		}

		TRACEPOINT;
	}

	std::array<cv::Mat, 2> DualFilterView::convertToArray(const std::vector<cv::Mat>& matVec)
	{
		TRACEPOINT;

		if(matVec.size() != 2)
		{
			throw std::runtime_error("Wrong number of elements in vector");
		}

		TRACEPOINT;

		return {matVec.at(0), matVec.at(1)};
	}


}
} //namespaces
