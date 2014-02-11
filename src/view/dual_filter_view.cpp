#include <exception>
#include <memory>
#include <iostream> //for debugging
#include <string>
#include <unordered_map>

#include "opencv2/core/core.hpp"

#include <QApplication>
#include <QComboBox>
#include <QHBoxLayout>
#include <QImage>
#include <QLabel>
#include <QPixmap>
#include <QString>
#include <QStringList>
#include <QtGui>

#include "../dbg/dbg.hpp"
#include "../qtutil/accordion.hpp"
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

/** Debugging method. Prints a QStringList. **/
void printQStringList(const QStringList& liste)
{
	for(auto iter = liste.begin(); iter != liste.end(); iter++)
	{
		std::cout << iter -> toStdString() << std::endl;
	}
}

//neuer Konstruktor
DualFilterView::DualFilterView(std::array<cv::Mat, 2> images, QWidget* parent)
	: FilterView{parent}, rawImages_(images), filterMap_()
{
	TRACEPOINT;
	QHBoxLayout* layout = new QHBoxLayout{};
	QHBoxLayout* imageLayout = new QHBoxLayout{};
	QWidget *imwid = new QWidget{};
	qtutil::Accordion *accor = new qtutil::Accordion{};
	auto comboBox = util::make_unique<QComboBox>();
	
	//register filters at map
	filterMap_.insert(std::make_pair<std::string, std::function<cv::Mat(void)>>
		("Overlay Image", [this](){return DualFilterView::applyOverlayFilter();}));
	filterMap_.insert(std::make_pair<std::string, std::function<cv::Mat(void)>>
		("Difference Image - hue",
		[this](){return DualFilterView::applyDiffFilter(DiffFilterType::HUE);}));
	filterMap_.insert(std::make_pair<std::string, std::function<cv::Mat(void)>>
		("Difference Image - saturation",
		[this](){return DualFilterView::applyDiffFilter(DiffFilterType::SATURATION);}));
	filterMap_.insert(std::make_pair<std::string, std::function<cv::Mat(void)>>
		("Difference Image - value",
		[this](){return DualFilterView::applyDiffFilter(DiffFilterType::VALUE);}));
	filterMap_.insert(std::make_pair<std::string, std::function<cv::Mat(void)>>
		("Difference Image - grayscale",
		[this](){return DualFilterView::applyDiffFilter(DiffFilterType::GRAYSCALE);}));
	
	//Register filter names at comboBox
	comboBox -> addItems(DualFilterView::extractStringListfromMap());
	connect(comboBox.get(), SIGNAL(currentIndexChanged(const QString&)), this,
		SLOT(updateFilterImg(const QString&)));
	
	accor->insert("Filter selection", std::move(comboBox));
	accor->setMinimumSize(150,0);
	layout->addWidget(accor);

	auto lambda = [this, imageLayout, accor](cv::Mat image, size_t count)
	{
		auto info = util::make_unique<qtutil::MatInfoWidget>(image);
		
		if (count == 1)
		{
			filterImgInfo_ = info.get();
		}

		connect(&zoomImages_.at(count),
			SIGNAL(updateConversionResult(ImageConversionResult)),info.get(),
			SLOT(updateConvertStatus(ImageConversionResult)));

		connect(info.get(),SIGNAL(getZoom(qreal)),&zoomImages_.at(count),
			SLOT(updateZoom(qreal)));

		zoomImages_.at(count).updateMat(image);

		imageLayout->addWidget(&(zoomImages_.at(count)));
		accor->insert("ImageInformation",std::move(info));
	};
	
	lambda(rawImages_.at(0), 0);
	
	lambda(rawImages_.at(0), 1);
	cv::Mat filteredImg = DualFilterView::applyDiffFilter(DiffFilterType::GRAYSCALE);
	zoomImages_.at(1).updateMat(filteredImg);
	filterImgInfo_ -> updateMat(filteredImg);

	lambda(rawImages_.at(1), 2);

	imwid->setLayout(imageLayout);
	layout->addWidget(imwid);

	setLayout(layout);

	TRACEPOINT;
}

//Vektorkonstruktor
DualFilterView::DualFilterView(const std::vector<cv::Mat>& images, QWidget* parent)
:DualFilterView(convertToArray(images), parent)
{}

cv::Mat DualFilterView::applyDiffFilter(DiffFilterType filterType)
{
	TRACEPOINT;

	auto check = checkDiffInput(filterType);
	if(!check.first)
	{
		throw std::invalid_argument{check.second.toStdString()};
	}

	if(filterType == DiffFilterType::GRAYSCALE)
	{
		return cv::abs(rawImages_.at(0) - rawImages_.at(1));
	}

	cv::Mat originalHSV, filteredHSV;
	cv::cvtColor(rawImages_.at(0), originalHSV, CV_BGR2HSV);
	cv::cvtColor(rawImages_.at(1), filteredHSV, CV_BGR2HSV);
	auto diffHSV = cv::abs(originalHSV - filteredHSV);

	std::array<cv::Mat, 3> splitVector;
	cv::split(diffHSV, splitVector.data());

	TRACEPOINT;
	
	return splitVector.at(static_cast<size_t>(filterType));
}

std::pair<bool, QString> DualFilterView::checkDiffInput(DiffFilterType filterType) const
{
	TRACEPOINT;
	size_t inChannels = rawImages_.at(0).channels();

	if (inChannels != static_cast<size_t>(rawImages_.at(1).channels()))
	{
		return std::make_pair(false, "images need to have same number of channels");
	}

	if (inChannels != 1 && inChannels != 3 && inChannels != 4)
	{
		return std::make_pair(false,
			"images must have one, three or four channels");
	}

	if (inChannels == 1 && filterType != DiffFilterType::GRAYSCALE)
	{
		return std::make_pair(false,
			"images are grayscale, but selected Filter can only progress 3-channel images");
	}

	TRACEPOINT;
	return std::make_pair(true, "images can be converted");
}

cv::Mat DualFilterView::applyOverlayFilter()
{
	cv::Mat out;
	
	cv::addWeighted(rawImages_.at(0), 0.5, rawImages_.at(1), 0.5, 0, out);
	
	return out;
}

std::array<cv::Mat, 2> DualFilterView::convertToArray(const std::vector<cv::Mat>& matVec) const
{
	TRACEPOINT;
	if(matVec.size() != 2)
	{
		throw std::runtime_error("Wrong number of elements in vector");
	}
	TRACEPOINT;
	return {matVec.at(0), matVec.at(1)};
}

QStringList DualFilterView::extractStringListfromMap() const
{
	QStringList stringList {};
	for(auto mapElem : filterMap_)
	{
		stringList << QString::fromStdString(mapElem.first);
	}
	return stringList;
}

void DualFilterView::updateFilterImg(const QString& selectedString)
{
	auto selectedFilter = filterMap_.find(selectedString.toStdString()) -> second;
	cv::Mat filteredImg = selectedFilter();
	zoomImages_.at(1).updateMat(filteredImg);
	filterImgInfo_ -> updateMat(filteredImg);
}

}
} //namespaces
