#include "dual_filter_view.hpp"

namespace cvv
{
namespace view
{
	DualFilterView::DualFilterView(std::vector<cv::Mat> images, QWidget* parent)
		: FilterView{parent}, rawImages_(images)
	{

	}


}
} //namespaces
