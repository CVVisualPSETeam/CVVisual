
#include <algorithm>

#include "keypointselector.hpp"

namespace cvv
{
namespace qtutil
{

KeyPointSelector::KeyPointSelector(QWidget *parent) : KeyPointSettings{ parent }
{
}

void KeyPointSelector::setSettings(CVVKeyPoint &key)
{
	if (std::find_if(selection_.begin(), selection_.end(),
	                 [&](const cv::KeyPoint &keypoint)
	{ return key == keypoint; }) != selection_.end())
	{
		key.setSelected(true);
	}
	else
	{
		key.setSelected(false);
	}
}

void KeyPointSelector::addToSelection(const cv::KeyPoint &keypoint)
{
	selection_.push_back(keypoint);
	updateAll();
}

void KeyPointSelector::singleSelection(const cv::KeyPoint &keypoint)
{
	selection_.erase(selection_.begin());
	selection_.push_back(keypoint);
	updateAll();
}

void KeyPointSelector::setSelection(const std::vector<cv::KeyPoint> &selection)
{
	selection_.erase(selection_.begin());
	for (auto &key : selection)
	{
		selection_.push_back(key);
	}
	updateAll();
}
}
}
