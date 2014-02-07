#include <exception>
#include <memory>
#include <iostream> //for debugging
#include <string>
#include <unordered_map>

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
	filterMap_.insert(std::make_pair<std::string, std::function<void(void)>>
		("Difference Image - hue",
		[this](){DualFilterView::applyDiffFilter(DiffFilterType::HUE);}));
	filterMap_.insert(std::make_pair<std::string, std::function<void(void)>>
		("Difference Image - saturation",
		[this](){DualFilterView::applyDiffFilter(DiffFilterType::SATURATION);}));
	filterMap_.insert(std::make_pair<std::string, std::function<void(void)>>
		("Difference Image - value",
		[this](){DualFilterView::applyDiffFilter(DiffFilterType::VALUE);}));
	filterMap_.insert(std::make_pair<std::string, std::function<void(void)>>
		("Difference Image - grayscale",
		[this](){DualFilterView::applyDiffFilter(DiffFilterType::GRAYSCALE);}));
	
	//Register filter names at comboBox
	comboBox -> addItems(DualFilterView::extractStringListfromMap());
	printQStringList(DualFilterView::extractStringListfromMap()); //debugging
	connect(comboBox.get(), SIGNAL(currentIndexChanged(const QString&)), this,
		SLOT(updateFilterImg(const QString&)));
	
	accor->insert("Filter selection", std::move(comboBox));
	accor->setMinimumSize(150,0);
	layout->addWidget(accor);

	auto lambda = [this, imageLayout, accor](cv::Mat image, size_t count)
	{
		auto info = util::make_unique<qtutil::MatInfoWidget>(image);

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
	DualFilterView::applyDiffFilter(DiffFilterType::GRAYSCALE);

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

void DualFilterView::applyDiffFilter(DiffFilterType filterType)
{
	TRACEPOINT;

	auto check = checkDiffInput(filterType);
	if(!check.first)
	{
		throw std::invalid_argument{check.second.toStdString()};
	}

	if(filterType == DiffFilterType::GRAYSCALE)
	{
		zoomImages_.at(1).updateMat(cv::abs(rawImages_.at(0) - rawImages_.at(1)));
		return;
	}

	cv::Mat originalHSV, filteredHSV;
	cv::cvtColor(rawImages_.at(0), originalHSV, CV_BGR2HSV);
	cv::cvtColor(rawImages_.at(1), filteredHSV, CV_BGR2HSV);
	auto diffHSV = cv::abs(originalHSV - filteredHSV);

	std::array<cv::Mat, 3> splitVector;
	cv::split(diffHSV, splitVector.data());

	zoomImages_.at(1).updateMat(splitVector.at(static_cast<size_t>(filterType)));

	TRACEPOINT;
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
	selectedFilter();
}

}
} //namespaces
