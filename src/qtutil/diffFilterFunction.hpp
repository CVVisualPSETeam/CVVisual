#ifndef CVVISUAL_DIFF_FILTER_FUNCTION_HPP
#define CVVISUAL_DIFF_FILTER_FUNCTION_HPP

#include "filterselectorwidget.hpp"

namespace cvv {
namespace qtutil {

enum class DiffFilterType
{
	HUE = 0,
	SATURATION = 1,
	VALUE = 2,
	LUMINANCE=VALUE
};

class DiffFilterFunction: public FilterFunctionWidget<2,1>
{
	private:
		DiffFilterType filterType_;

	public:
		DiffFilterFunction(DiffFilterType filterType, QWidget* parent = nullptr);

		const std::array<cv::Mat,1>& applyFilter(const std::array<cv::Mat,2>& in,
					std::array<cv::Mat,1>& out) const override;

		std::pair<bool, QString> checkInput(const std::array<cv::Mat,2>& in) const override;
};

}}

#endif
