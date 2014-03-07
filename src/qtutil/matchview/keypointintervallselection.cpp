
#include <QVBoxLayout>

#include <algorithm>

#include "keypointintervallselection.hpp"
#include "../../util/util.hpp"

namespace cvv{ namespace qtutil{

KeyPointIntervallSelector::KeyPointIntervallSelector(std::vector<cv::KeyPoint> keypoints, QWidget *parent):
	KeyPointSelection{parent}
{
	double min=0.0;
	double max=0.0;

	auto valueChooser=util::make_unique<KeyPointValueChooser>();
	valueChooser_=valueChooser.get();

	for(auto& key:keypoints)
	{
		min=std::min(valueChooser->getChoosenValue(key),min);
		max=std::max(valueChooser->getChoosenValue(key),max);
	}

	auto layout=util::make_unique<QVBoxLayout>();
	auto selector=util::make_unique<IntervallSelector>(min,max);

	selector_=selector.get();
	connect(&(selector->signalSettingsChanged()),SIGNAL(signal()),this,SIGNAL(settingsChanged()));

	layout->addWidget(selector.release());
	setLayout(layout.release());
}

std::vector<cv::KeyPoint> KeyPointIntervallSelector::select(const std::vector<cv::KeyPoint> &selection)
{
	return selector_->select(selection, [&](const cv::KeyPoint& key)
				{return this->valueChooser_->getChoosenValue(key);}
			);
}


}}
