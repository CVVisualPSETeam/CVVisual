#include "changed_pixels_widget.hpp"

namespace cvv
{
namespace qtutil
{

void ChangedPixelsWidget::applyFilter(ChangedPixelsWidget::InputArray in,
                                      ChangedPixelsWidget::OutputArray out) const
{
	auto& original = in.at(0);
	auto& filtered = in.at(1);
	(void) original;
	(void) filtered;
	(void) out;
	//TODO: implement the actual work
}

std::pair<bool, QString> ChangedPixelsWidget::checkInput(InputArray in) const
{
	if (in.at(0).get().size() != in.at(1).get().size())
	{
		return std::make_pair(false, "images need to have same size");
	}

	size_t inChannels = in.at(0).get().channels();

	if (inChannels != static_cast<size_t>(in.at(1).get().channels()))
	{
		return std::make_pair(
		    false, "images need to have same number of channels");
	}
	return std::make_pair(true, "images can be converted");
}


} // qtutil
} // cvv
