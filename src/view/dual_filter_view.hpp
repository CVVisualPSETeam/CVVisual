#ifndef CVVISUAL_DUAL_FILTER_VIEW_HPP
#define CVVISUAL_DUAL_FILTER_VIEW_HPP

#include "filter_view.hpp"

namespace cvv {

namespace qtutil{
	class ZoomableImage{}; //dummy class

	template<size_t bla, size_t blubb> //dummy class
	class FilterSelectorWidget{};
}

namespace view {

class DualFilterView: public FilterView
{
Q_OBJECT
	private:
		std::vector<cvv::qtutil::ZoomableImage> images;
		cvv::qtutil::FilterSelectorWidget<2,1> selectorWidget;
		cvv::qtutil::Accordion accordion;
};

}} //namespaces

#endif
