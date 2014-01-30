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
		std::vector<cvv::qtutil::ZoomableImage> zoomImages_;
		cvv::qtutil::FilterSelectorWidget<2,1> selectorWidget_;
		cvv::qtutil::Accordion accordion_;

	public:
		DualFilterView(std::vector<cv::Mat> images, QWidget* parent = nullptr);
};

}} //namespaces

#endif
