#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

#include <QString>

#include "diffFilterFunction.hpp"

namespace cvv{
namespace qtutil{

DiffFilterFunction::DiffFilterFunction(DiffFilterType filterType, QWidget* parent)
	: QWidget{parent}, filterType_{filterType} {}

const std::array<cv::Mat,1>& DiffFilterFunction::applyFilter(const std::array<cv::Mat,2>& in,
			std::array<cv::Mat,1>& out) const
{
	cv::Mat originalHSV = in.at(0), filteredHSV = in.at(1);
	cv::cvtColor(in.at(0), originalHSV, CV_BGR2HSV);
	cv::cvtColor(in.at(1), filteredHSV, CV_BGR2HSV);
	auto diffHSV = cv::abs(originalHSV - filteredHSV);

	std::array<cv::Mat, 3> splitVector;
	cv::split(diffHSV, splitVector.data());

	out.at(0) = splitVector.at(static_cast<size_t>(filterType_));

	return out;
}

std::pair<bool, QString> DiffFilterFunction::checkInput(const std::array<cv::Mat,2>& in) const
{
	(void)in;
	return std::make_pair(true, "your images can be converted");
}

}}
