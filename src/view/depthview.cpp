#include <algorithm>

#include <QHBoxLayout>

#include "../qtutil/accordion.hpp"
#include "../qtutil/matchview/matchscene.hpp"
#include "../qtutil/matchview/singlecolormatchpen.hpp"
#include "../qtutil/matchview/cvvkeypoint.hpp"
#include "../qtutil/matchview/cvvpointmatch.hpp"
#include "../util/util.hpp"

#include "depthview.hpp"

namespace cvv{ namespace view{

DepthMatchView::DepthMatchView(	std::vector<cv::KeyPoint> leftKeyPoints,
				std::vector<cv::KeyPoint> rightKeyPoints,
				std::vector<cv::DMatch> matches,
				cv::Mat leftIm,
				cv::Mat rightIm,
				QWidget *parent):
			MatchView{parent}
{
	TRACEPOINT;
	auto layout	= util::make_unique<QHBoxLayout>();
	auto accor	= util::make_unique<qtutil::Accordion>();
	auto matchscene	= util::make_unique<qtutil::MatchScene>(leftIm,rightIm);
	auto matchpen	= util::make_unique<qtutil::SingleColorMatchPen>();

	qtutil::MatchScene *matchscene_ptr		= matchscene.get();
	qtutil::SingleColorMatchPen *matchpen_ptr	= matchpen.get();
	accor->setMinimumWidth(250);
	accor->setMaximumWidth(250);


	accor->insert("Match Color",std::move(matchpen));

	layout->addWidget(accor.release());
	layout->addWidget(matchscene.release());

	setLayout(layout.release());

	std::vector<qtutil::CVVKeyPoint*> leftKeys;
	std::vector<qtutil::CVVKeyPoint*> leftinvisibleKeys;
	std::vector<qtutil::CVVKeyPoint*> rightKeys;
	std::vector<qtutil::CVVKeyPoint*> rightinvisibleKeys;

	for(auto& keypoint:leftKeyPoints)
	{
		auto key = util::make_unique<qtutil::CVVKeyPoint>(keypoint);
		key->setShow(false);
		leftKeys.push_back(key.get());
		matchscene_ptr->addLeftKeypoint(key.release());

		auto keyinvisible = util::make_unique<qtutil::CVVKeyPoint>(keypoint);
		keyinvisible->setShow(false);
		rightinvisibleKeys.push_back(keyinvisible.get());
		matchscene_ptr->addRightKeyPoint(keyinvisible.release());
	}
	for(auto& keypoint:rightKeyPoints)
	{
		auto key = util::make_unique<qtutil::CVVKeyPoint>(keypoint);
		key->setShow(false);
		rightKeys.push_back(key.get());
		matchscene_ptr->addRightKeyPoint(key.release());

		auto keyinvisible = util::make_unique<qtutil::CVVKeyPoint>(keypoint);
		keyinvisible->setShow(false);
		leftinvisibleKeys.push_back(keyinvisible.get());
		matchscene_ptr->addLeftKeypoint(keyinvisible.release());

	}
	for(auto& match:matches)
	{
		auto cvmatchleft = util::make_unique<qtutil::CVVPointMatch>(
				leftKeys.at(match.queryIdx),
				leftinvisibleKeys.at(match.trainIdx),
				match.distance);
		connect(matchpen_ptr,SIGNAL(settingsChanged(MatchSettings&)),
			cvmatchleft.get(),SLOT(updateSettings(MatchSettings&)));
		matchscene_ptr->addMatch(cvmatchleft.release());

		auto cvmatchright = util::make_unique<qtutil::CVVPointMatch>(
					rightinvisibleKeys.at(match.queryIdx),
					rightKeys.at(match.trainIdx),
					match.distance,false);
		connect(matchpen_ptr,SIGNAL(settingsChanged(MatchSettings&)),
			cvmatchright.get(),SLOT(updateSettings(MatchSettings&)));
		matchscene_ptr->addMatch(cvmatchright.release());
	}
	TRACEPOINT;
}

}}
