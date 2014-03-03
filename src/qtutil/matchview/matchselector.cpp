
#include <algorithm>

#include "matchselector.hpp"

namespace cvv{ namespace qtutil{

MatchSelector::MatchSelector(QWidget *parent):
	MatchSettings{parent}
{

}

void cvv::qtutil::MatchSelector::setSettings(CVVMatch &match)
{
	if(std::find_if(selection_.begin(), selection_.end(),
			[&](const cv::DMatch& ma1){
			return match==ma1;})
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
