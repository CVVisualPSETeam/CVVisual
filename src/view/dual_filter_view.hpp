#ifndef CVVISUAL_DUAL_FILTER_VIEW_HPP
#define CVVISUAL_DUAL_FILTER_VIEW_HPP

//STD
//OpenCV
//QT
#include <QWidget>
//CVV
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



}} //namespaces

#endif
