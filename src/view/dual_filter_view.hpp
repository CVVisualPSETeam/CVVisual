#ifndef CVVISUAL_DUAL_FILTER_VIEW_HPP
#define CVVISUAL_DUAL_FILTER_VIEW_HPP

//STD
//OpenCV
//QT
#include <QWidget>
//CVV
#include "../qtutil/filterselectorwidget.hpp"
#include "../qtutil/zoomableimage.hpp"
#include "filter_view.hpp"

namespace cvv {namespace view {

class DualFilterView: public FilterView
{
Q_OBJECT
	private:
		std::array<cv::Mat, 2> rawImages_;
		std::array<cvv::qtutil::ZoomableImage, 3> zoomImages_;
		qtutil::FilterSelectorWidget<2,1> *filterSelector_;

		std::array<cv::Mat, 2> convertToArray(const std::vector<cv::Mat>& );

	public:
		DualFilterView(std::array<cv::Mat, 2> images, QWidget* parent = nullptr);
		DualFilterView(const std::vector<cv::Mat>& images, QWidget* parent = nullptr);
	public slots:
		void applyFilter();
};



}} //namespaces

#endif
