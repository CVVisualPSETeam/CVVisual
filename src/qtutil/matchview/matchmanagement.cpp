
#include <algorithm>

#include <QVBoxLayout>

#include "matchmanagement.hpp"

namespace cvv
{
namespace qtutil
{

MatchManagement::MatchManagement(std::vector<cv::DMatch> univers,QWidget *parent) :
	MatchSettings{parent},
	univers_{univers}
{
	auto basicLayout=util::make_unique<QVBoxLayout>();
	auto settingsLayout=util::make_unique<QVBoxLayout>();
	auto selectorLayout=util::make_unique<QVBoxLayout>();

	settingsLayout_=settingsLayout.get();
	selectorLayout_=selectorLayout.get();

	basicLayout->addLayout(settingsLayout.release());
	basicLayout->addLayout(selectorLayout.release());

	setLayout(basicLayout.release());
}

void MatchManagement::setSettings(CVVMatch &match)
{
	if (std::find_if(selection_.begin(), selection_.end(),
			 [&](const cv::DMatch &ma1)
		{ return match == ma1; }) != selection_.end())
	{
		for(auto setting: settingsList_)
		{
			match.updateSettings(*setting);
		}
	}
}

void MatchManagement::addToSelection(const cv::DMatch &match)
{
	selection_.push_back(match);
	updateAll();
}

void MatchManagement::singleSelection(const cv::DMatch &match)
{
	selection_.erase(selection_.begin());
	selection_.push_back(match);
	updateAll();
}

void MatchManagement::setSelection(
    const std::vector<cv::DMatch> &selection)
{
	selection_.erase(selection_.begin());
	for (auto &match : selection)
	{
		selection_.push_back(match);
	}
	updateAll();
}

/*
void MatchManagement::addSetting(std::unique_ptr<MatchSettings> setting)
{
	settingsList_.push_back(setting.get());
	settingsLayout_->addWidget(setting.release());
}

void MatchManagement::removeSetting(MatchSettings *setting)
{
	std::remove_if(settingsList_.begin(),settingsList_.end(),
		[=](const MatchSettings* other)
		{return other==setting;}
	);
	settingsLayout_->removeWidget(setting);
	setting->deleteLater();
}*/

void MatchManagement::addSelection(MatchSelectionSelector *selection)
{
	selectorList_.push_back(selection);
	selectorLayout_->addWidget(selection);
}

void MatchManagement::removeSelection(MatchSelectionSelector *selector)
{
	std::remove_if(selectorList_.begin(),selectorList_.end(),
		[=](const MatchSelectionSelector* other)
		{return other==selector;}
	);
	selectorLayout_->removeWidget(selector);

	selector->deleteLater();
}

}
}
