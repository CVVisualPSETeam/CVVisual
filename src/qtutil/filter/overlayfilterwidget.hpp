#ifndef CVVISUAL_OVERLAY_FILTER_WIDGET_HPP
#define CVVISUAL_OVERLAY_FILTER_WIDGET_HPP

#include <unordered_map>

#include "../../util/observer_ptr.hpp"
#include "../filterselectorwidget.hpp"

namespace cvv {
namespace qtutil {

class OverlayFilterWidget: public FilterFunctionWidget<2,1>
{
Q_OBJECT
public:
	/**
	 * @brief The input type.
	 */
	using InputArray  = FilterFunctionWidget<2,1>::InputArray;
	//std::array<util::Reference<const cv::Mat>,2>

	/**
	 * @brief The output type.
	 */
	using OutputArray = FilterFunctionWidget<2,1>::OutputArray;
	//std::array<util::Reference<cv::Mat>,1>
	
	
	OverlayFilterWidget(QWidget* parent = nullptr);
	
	~OverlayFilterWidget()
	{
		TRACEPOINT;
	}

	void applyFilter(InputArray in,OutputArray out) const;

	std::pair<bool, QString> checkInput(InputArray in) const;
	
private:
	double opacityOfOriginalImg_;
	
private slots:
	void updateOpacity(int);
};

}}

#endif
