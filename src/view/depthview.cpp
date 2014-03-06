#include <algorithm>

#include <QHBoxLayout>

#include "../qtutil/accordion.hpp"
#include "../qtutil/matchview/matchscene.hpp"
#include "../qtutil/matchview/matchmanagement.hpp"
#include "../qtutil/matchview/cvvkeypoint.hpp"
#include "../qtutil/matchview/cvvpointmatch.hpp"
#include "../util/util.hpp"

#include "depthview.hpp"

namespace cvv
{
namespace view
{

DepthMatchView::DepthMatchView(std::vector<cv::KeyPoint> leftKeyPoints,
			       std::vector<cv::KeyPoint> rightKeyPoints,
			       std::vector<cv::DMatch> matches, cv::Mat leftIm,
			       cv::Mat rightIm, bool usetrainIdx,
			       QWidget *parent)
    : MatchView{ parent }
{
	TRACEPOINT;
	auto layout = util::make_unique<QHBoxLayout>();
	auto accor = util::make_unique<qtutil::Accordion>();
	auto matchscene =
	    util::make_unique<qtutil::MatchScene>(leftIm, rightIm);
	auto matchmnt = util::make_unique<qtutil::MatchManagement>(matches);

	qtutil::MatchScene *matchscene_ptr = matchscene.get();
	matchManagment_ = matchmnt.get();

	accor->setMinimumWidth(350);
	accor->setMaximumWidth(350);

	accor->insert("Match Settings", std::move(matchmnt));
	accor->insert("Left Image ",
		      std::move(matchscene_ptr->getLeftMatInfoWidget()));
	accor->insert("Right Image ",
		      std::move(matchscene_ptr->getRightMatInfoWidget()));
	accor->insert("Sync Zoom ",
		      std::move(matchscene_ptr->getSyncZoomWidget()));

	layout->addWidget(accor.release());
	layout->addWidget(matchscene.release());

	setLayout(layout.release());

	std::vector<qtutil::CVVKeyPoint *> leftKeys;
	std::vector<qtutil::CVVKeyPoint *> leftinvisibleKeys;
	std::vector<qtutil::CVVKeyPoint *> rightKeys;
	std::vector<qtutil::CVVKeyPoint *> rightinvisibleKeys;

	for (auto &keypoint : leftKeyPoints)
	{
		auto key = util::make_unique<qtutil::CVVKeyPoint>(keypoint);
		key->setShow(false);
		leftKeys.push_back(key.get());
		matchscene_ptr->addLeftKeypoint(std::move(key));

		auto keyinvisible =
		    util::make_unique<qtutil::CVVKeyPoint>(keypoint);
		keyinvisible->setShow(false);
		rightinvisibleKeys.push_back(keyinvisible.get());
		matchscene_ptr->addRightKeyPoint(std::move(keyinvisible));
	}
	for (auto &keypoint : rightKeyPoints)
	{
		auto key = util::make_unique<qtutil::CVVKeyPoint>(keypoint);
		key->setShow(false);
		rightKeys.push_back(key.get());
		matchscene_ptr->addRightKeyPoint(std::move(key));

		auto keyinvisible =
		    util::make_unique<qtutil::CVVKeyPoint>(keypoint);
		keyinvisible->setShow(false);
		leftinvisibleKeys.push_back(keyinvisible.get());
		matchscene_ptr->addLeftKeypoint(std::move(keyinvisible));
	}
	for (auto &match : matches)
	{
		auto cvmatchleft = util::make_unique<qtutil::CVVPointMatch>(
		    leftKeys.at(match.queryIdx),
		    leftinvisibleKeys.at(
			(usetrainIdx ? match.trainIdx : match.imgIdx)),
		    match);
		connect(matchManagment_, SIGNAL(settingsChanged(MatchSettings &)),
			cvmatchleft.get(),
			SLOT(updateSettings(MatchSettings &)));
		matchscene_ptr->addMatch(std::move(cvmatchleft));

		auto cvmatchright = util::make_unique<qtutil::CVVPointMatch>(
		    rightinvisibleKeys.at(match.queryIdx),
		    rightKeys.at((usetrainIdx ? match.trainIdx : match.imgIdx)),
		    match, false);
		connect(matchManagment_, SIGNAL(settingsChanged(MatchSettings &)),
			cvmatchright.get(),
			SLOT(updateSettings(MatchSettings &)));
		matchscene_ptr->addMatch(std::move(cvmatchright));
	}
	matchManagment_->updateAll();
	TRACEPOINT;
}
}
}
