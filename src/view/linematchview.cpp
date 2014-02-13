#include <QHBoxLayout>

#include "../qtutil/accordion.hpp"
#include "../qtutil/matchscene.hpp"
#include "../qtutil/singlecolorpen.hpp"
#include "../qtutil/cvvkeypoint.hpp"
#include "../qtutil/cvvmatch.hpp"
#include "../qtutil/singlecolorpen.hpp"
#include "../qtutil/singlecolorkeypointpen.hpp"
#include "../util/util.hpp"

#include "linematchview.hpp"

namespace cvv{ namespace view{

LineMatchView::LineMatchView(std::vector<cv::KeyPoint> leftKeyPoints,
		std::vector<cv::KeyPoint> rightKeyPoints,
		std::vector<cv::DMatch> matches,
		cv::Mat leftIm,
		cv::Mat rightIm,
		QWidget *parent):
	MatchView{parent}
{
	auto layout	= util::make_unique<QHBoxLayout>();
	auto accor	= util::make_unique< qtutil::Accordion>();
	auto matchscene	= util::make_unique<qtutil::MatchScene>(leftIm,rightIm);
	auto matchpen	= util::make_unique<qtutil::SingleColorPen>();
	auto keypen	= util::make_unique<qtutil::SingleColorKeyPen>();

	accor->setMinimumWidth(250);
	accor->setMaximumWidth(250);

	std::vector<qtutil::CVVKeyPoint*> leftKeys;
	std::vector<qtutil::CVVKeyPoint*> rightKeys;

	for(auto& keypoint:leftKeyPoints)
	{
		auto key = util::make_unique<qtutil::CVVKeyPoint>(keypoint);
		connect(keypen.get(),SIGNAL(settingsChanged(const KeyPointPen&)),key.get(),SLOT(updatePen(const KeyPointPen&)));
		matchscene->addLeftKeypoint(key.get());
		leftKeys.push_back(key.release());
	}
	for(auto& keypoint:rightKeyPoints)
	{
		auto key = util::make_unique<qtutil::CVVKeyPoint>(keypoint);
		connect(keypen.get(),SIGNAL(settingsChanged(const KeyPointPen&)),key.get(),SLOT(updatePen(const KeyPointPen&)));
		matchscene->addRightKeyPoint(key.get());
		rightKeys.push_back(key.release());
	}
	for(auto& match:matches)
	{
		auto cvmatch = util::make_unique<qtutil::CVVMatch>(leftKeys.at(match.queryIdx),rightKeys.at(match.trainIdx),match.distance);
		connect(matchpen.get(),SIGNAL(settingsChanged(const MatchPen&)),cvmatch.get(),SLOT(updatePen(const MatchPen&)));
		matchscene->addMatch(cvmatch.release());
	}

	accor->insert("Match Color",std::move(matchpen));
	accor->insert("KeyPoint Color",std::move(keypen));
	layout->addWidget(accor.release());
	layout->addWidget(matchscene.release());
	setLayout(layout.release());
}
}}
