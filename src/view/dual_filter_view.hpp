#ifndef CVVISUAL_DUAL_FILTER_VIEW_HPP
#define CVVISUAL_DUAL_FILTER_VIEW_HPP

//STD
//OpenCV
//QT
#include <QWidget>
//CVV
#include "../qtutil/filterselectorwidget.hpp"
#include "../qtutil/zoomableimage.hpp"
#include "../qtutil/accordion.hpp"
#include "filter_view.hpp"

namespace cvv {namespace view {

class DualFilterView: public FilterView
{
Q_OBJECT
	private:
		std::vector<cv::Mat> rawImages_;

	public:
		DualFilterView(std::vector<cv::Mat> images, QWidget* parent = nullptr);
};

class DiffFilter: public qtutil::FilterFunctionWidget<2,1>
{
	const std::array<cv::Mat&,1>& applyFilter(const std::array<const cv::Mat&,2>& in,
				const std::array<cv::Mat&,1>& out) const;

	std::pair<bool, QString> checkInput(const std::array<const cv::Mat&,2>& in) const;
};

}} //namespaces

#endif
