
#include <algorithm>

#include "matchselector.hpp"

namespace cvv{ namespace qtutil{

void cvv::qtutil::MatchSelector::setSettings(CVVMatch &match)
{

	if(std::find_if(selection_.begin(), selection_.end(),
			[&](const cv::DMatch& ma1){
			return (ma1.queryIdx==match.match().queryIdx)
			&&(ma1.imgIdx==match.match().imgIdx)
			&&(ma1.trainIdx==match.match().trainIdx);})
	!= selection_.end())
	{
		match.setSelected(true);
	} else {
		match.setSelected(false);
	}
}

void cvv::qtutil::MatchSelector::addToSelection(const cv::DMatch & match)
{
	selection_.push_back(match);
	updateAll();
}

void cvv::qtutil::MatchSelector::singleSelection(const cv::DMatch & match)
{
	selection_.erase(selection_.begin());
	selection_.push_back(match);
	updateAll();
}

void cvv::qtutil::MatchSelector::setSelection(const std::vector<cv::DMatch> & selection)
{
	selection_.erase(selection_.begin());
	for(auto& match:selection)
	{
		selection_.push_back(match);
	}
	updateAll();
}

}}
