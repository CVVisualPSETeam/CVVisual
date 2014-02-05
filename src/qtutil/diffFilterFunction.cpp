#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

#include <QString>

#include "../dbg/dbg.hpp"
#include "diffFilterFunction.hpp"

namespace cvv{
namespace qtutil{

DiffFilterFunction::DiffFilterFunction(DiffFilterType filterType, QWidget* parent)
	: QWidget{parent}, filterType_{filterType} {}

const std::array<cv::Mat,1>& DiffFilterFunction::applyFilter(const std::array<cv::Mat,2>& in,
			std::array<cv::Mat,1>& out) const
{
	TRACEPOINT;

	auto check = checkInput(in);
	if(!check.first)
	{
		throw std::invalid_argument{check.second.toStdString()};
	}

	if(filterType_ == DiffFilterType::GRAYSCALE)
	{
		out.at(0) = cv::abs(in.at(0) - in.at(1));
		return out;
	}

	cv::Mat originalHSV, filteredHSV;
	cv::cvtColor(in.at(0), originalHSV, CV_BGR2HSV);
	cv::cvtColor(in.at(1), filteredHSV, CV_BGR2HSV);
	auto diffHSV = cv::abs(originalHSV - filteredHSV);

	std::array<cv::Mat, 3> splitVector;
	cv::split(diffHSV, splitVector.data());

	out.at(0) = splitVector.at(static_cast<size_t>(filterType_));

	TRACEPOINT;

	return out;
}

std::pair<bool, QString> DiffFilterFunction::checkInput(const std::array<cv::Mat,2>& in) const
{
	TRACEPOINT;

	size_t inChannels = in.at(0).channels();

	if (inChannels != static_cast<size_t>(in.at(1).channels()))
	{
		return std::make_pair(false, "images need to have same number of channels");
	}

	if (inChannels == 1 && filterType_ != DiffFilterType::GRAYSCALE)
	{
		return std::make_pair(false,
			"image is grayscale, but selected Filter can only progress 3-channel images");
	}

	if (inChannels != 1 && filterType_ == DiffFilterType::GRAYSCALE)
	{
		return std::make_pair(true,
			"filter is grayscale, but image has more channels."
			"Result will have as many channels as image");
	}

	TRACEPOINT;

	return std::make_pair(true, "images can be converted");
}

}}
